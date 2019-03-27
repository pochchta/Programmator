#include "variables.h"
#include "functions.h"



///////////--------------------------------------------удалить
char db,db_i, db_y=1;              // debug
char *db_p;

///---------------------------------------------------------удалить


void UART(void) org 0x0023 {
    if (SCON.RI) {    // закончился приём
        if (rx_n == 3 || rx_n < 0) {
            rx_n = 0;
            rx_pack_n++;
            if (rx_pack_n == rx_pack_n_MAX) rx_pack_n = 0;
        }
        rx_buf[rx_n] = SBUF;
        rx_n++;
        SCON.RI = 0;
    }
    if (SCON.TI) {    // закончилась передача
        if (tx_n > 0) {
            if (tx_n > 3) SBUF = rx_PREAM;
            else SBUF = *(tx_pointer + 3 - tx_n);
            tx_n--;
        }
        SCON.TI = 0;
    }
}

void main() {

    P1 = 0XFF;
    P3 = 0XFF;
    PCON.F7 = 0;             // НЕ удвоенная скорость передачи
    TMOD = 0b00100001;       // настройка таймеров
    //       |||||||+-----     M0 (T0) = 1, режим 1 (0b01) таймер 0; режим 1 - 16 битный С/Т
    //       ||||||+------     M1 (T0) = 0, режим 1 (0b01) таймер 0; режим 1 - 16 битный С/Т
    //       |||||+-------     внутренний источник таймер 0
    //       ||||+--------     блокировка откл., таймер 0
    //       |||+---------     M0 (T1) = 0, режим 2 (0b10) таймер 1; режим 2 - автоперегружаемый таймер
    //       ||+----------     M1 (T1) = 1, режим 2 (0b10) таймер 1; режим 2 - автоперегружаемый таймер
    //       |+-----------     внутренний источник таймер 1
    //       +------------     блокировка откл., таймер 1
    TL0 = 0x89;                // настройка таймера 0 на
    TH0 = 0xE6;                // 1500 мс - т.е. половину периода серво
    TH1 = 0xFA;              // автоперегружаемое число для 4800 кбит
    SCON = 0;
    SCON.SM0 = 0;              // режим 1 - 8 бит, по таймеру 1
    SCON.SM1 = 1;              // режим 1
    SCON.REN = 1;              // разрешение приема
    TCON = 0;
    TCON.TR1 = 1;            // пуск таймера 1
    TCON.TR0 = 1;            // пуск таймера 0
    IE.EA = 1;               // разрешены все прерывания
    IE.ES = 1;                // разрешено прерывание по UART
  //  IE.ET0 = 1;               // разрешено прерывание по таймеру 0
  //  IP.PT0 = 1;               // приоритет прерывания таймера 0


    //--------------------------------------------------  delete
  //db_start();
    //--------------------------------------------------  delete

    while (1) {
    
    //--------------------------------------------------  delete
    
   //     crc = '0';
   //     crc8();
 //       crc ^= 'p';
//        crc8();       // 0pn

        //p1 = rx_n + '0';
        //db = crc_state + '0';
        //db_i++;
        //if (db_i == 0) db_y++;
        //if (db_i == 0) SBUF = 'y';
        //db = rx_buf[0];


      //------------------------------------------------- delete

    
        if (rx_synch_f == 0) {                   // синхронизация
            rx_pream_n = 0;
            IE.ES = 0;                                                                   //  |-- запрещено прерывание по UART
            for (rx_n_copy = 0; rx_n_copy < 3; rx_n_copy++) if (rx_buf[rx_n_copy] == rx_PREAM) rx_pream_n++; //  |
            if (rx_pream_n > 1) {                                                        //  |
                rx_synch_f = 1;                                                          //  |
                rx_n = -1;                                                               //  |
                crc_state = 0;                                                           //  |
            }                                                                            //  |
            IE.ES = 1;                                                                   //  |-- разрешено прерывание по UART
        }

        if (rx_pack_n != rx_pack_n_prev) {       // получение новой посылки
            crc_state = 0;
            rx_pack_n_prev = rx_pack_n;
        }

        if (rx_synch_f == 1) {                   // проверка данных
            IE.ES = 0;                                  //  |-- запрещено прерывание по UART
            rx_buf_copy[0] = rx_buf[0];                 //  |
            rx_buf_copy[1] = rx_buf[1];                 //  |
            rx_buf_copy[2] = rx_buf[2];                 //  |
            rx_n_copy = rx_n;                           //  |
            rx_pack_n_copy = rx_pack_n;                 //  |
            IE.ES = 1;                                  //  |-- разрешено прерывание по UART
            if (crc_state == 0 && rx_n_copy > 0) {
                rx_crc = crc8(rx_buf_copy[0]);
                crc_state++;
            }
            if (crc_state == 1 && rx_n_copy > 1) {
                rx_crc = crc8(rx_crc ^= rx_buf_copy[1]);
                crc_state++;
            }
            if (crc_state == 2 && rx_n_copy > 2) {
                if (rx_crc == rx_buf_copy[2]) {
                    crc_state = crc_state_OK;
                    code_ready = code_ready_OK;
                    if ((rx_buf_copy[0] & 0b00001111) >= rx_pack_n_MAX || (rx_buf_copy[0] & 0b00001111) != rx_pack_n_copy) {
                        if (code_state != code_state_SES_ST) // проверка на правильность номера, исключение - старт сессии
                          code_ready = code_ready_ERR_N;
                    }
                    if ((rx_buf_copy[0] & 0b11110000) > code_state_rx_MAX) { // проверка на правильность кода
                        code_ready = code_ready_ERR_C;
                    }
                } else {
                    crc_state = crc_state_ERR;
                    code_ready = code_ready_ERR_CRC;
                    rx_synch_f = 0;
                }
            }
            if (code_ready != code_ready_NOT && code_state == code_state_STOP) {
                //code_buf[0] = rx_buf_copy[0];
                code_buf[1] = rx_buf_copy[1];
                code_num = rx_buf_copy[0] & 0b00001111;
                if (code_ready == code_ready_OK) {
                    code_state = rx_buf_copy[0] >> 4;
                }
                if (code_ready == code_ready_ERR_CRC) {
                    code_state = code_state_MSG_ERR_CRC;
                }
                if (code_ready == code_ready_ERR_N) {
                    code_state = code_state_MSG_ERR_N;
                }
                if (code_ready == code_ready_ERR_C) {
                    code_state = code_state_MSG_ERR_C;
                }
                code_ready = code_ready_NOT;
            }
        }

        switch (code_state){                     // исполнитель кода
        /*
            case 111: tx_start(3 , rx_buf_copy);
                code_state = code_state_STOP;
                break;
            case 112: tx_msg_to_buf(tx_msg_rom[tx_msg_ERR]);
                tx_start(3 , tx_buf);
                code_state = code_state_STOP;
                break;                      */
            case code_state_RD_P1:
            
                break;
            case code_state_WR_P1:

                break;
            case code_state_IM_P1:

                break;
            case code_state_RD_P3:

                break;
            case code_state_WR_P3:

                break;
            case code_state_IM_P3:

                break;
            case code_state_WR_R:

                break;
            case code_state_IM_R:

                break;
            //---------------------------
            case code_state_ERR:

                break;
            case code_state_ERR_PREAM:

                break;
            case code_state_ERR_CRC:
            
                break;
            case code_state_ERR_N:

                break;
            case code_state_ERR_C:

                break;
            //---------------------------
            case code_state_OK:

                break;
            case code_state_OK_ST:

                break;
            case code_state_OK_END:

                break;
            //---------------------------
            case code_state_SES:

                break;
            case code_state_SES_ST:

                break;
            case code_state_SES_END:

                break;
            //---------------------------
            case code_state_MSG_ERR_PREAM:
                tx_crc[0] = code_num + (code_state_ERR_PREAM << 4);
                tx_crc[1] = code_state_MSG_ERR_PREAM & 0b00001111;
                code_state = code_state_CRC0;
                break;
            case code_state_MSG_ERR_CRC:

                break;
            case code_state_MSG_ERR_N:

                break;
            case code_state_MSG_ERR_C:

                break;
            //---------------------------
            case code_state_MSG_OK_ST:

                break;
            case code_state_MSG_OK_END:

                break;
            //---------------------------
            case code_state_CRC0:
                tx_crc[2] = crc8(tx_crc[0]);
                break;
            case code_state_CRC1:
                tx_crc[2] = crc8(tx_crc[2] ^ tx_buf[1]);
                break;
            case code_state_CRC2:
                if (tx_n == 0) {
                    tx_buf[0] = tx_crc[0];
                    tx_buf[1] = tx_crc[1];
                    tx_buf[2] = tx_crc[2];
                    tx_start(5 , rx_buf_copy);
                    code_state = code_state_STOP;
                }
                break;
        }
    }
}
