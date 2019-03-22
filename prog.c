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
            if (rx_pack_n == 10) rx_pack_n = 0;
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


volatile const char tx_msg_rom[2][4] = {"ERR" , "OK!"};
#define tx_msg_ERR 0
#define tx_msg_OK 1

void tx_msg_to_buf(const char *tx_msg_input){
    char tx_msg_i;
    for (tx_msg_i = 0 ; tx_msg_i < 3 ; tx_msg_i++){
        tx_buf[tx_msg_i] = tx_msg_input[tx_msg_i];
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


      //------------------------------------------------------------delete

    
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
            IE.ES = 1;                                  //  |-- разрешено прерывание по UART
            if (crc_state == 0 && rx_n_copy > 0) {
                crc = rx_buf_copy[0];
                crc8();
                crc_state++;
            }
            if (crc_state == 1 && rx_n_copy > 1) {
                crc ^= rx_buf_copy[1];
                crc8();
                crc_state++;
            }
            if (crc_state == 2 && rx_n_copy > 2) {
                if (crc == rx_buf_copy[2] /*&& формат посылки*/) {
                    crc_state = crc_state_OK;
                    code_ready = code_ready_OK;
                } else {
                    crc_state = crc_state_ERR;
                    code_ready = code_ready_ERR;
                    rx_synch_f = 0;
                }
            }
            if (code_ready != code_ready_NOT && code_state == code_state_STOP) {
                code_buf[0] = rx_buf_copy[0];
                code_buf[1] = rx_buf_copy[1];
                if (code_ready == code_ready_OK) {
                    code_state = 0;
                } else {
                    code_state = 10;
                }
                code_ready = code_ready_NOT;
            }
        }

        switch (code_state){                     // исполнитель кода
            case 0: tx_start(3 , rx_buf_copy);
                code_state = code_state_STOP;
                break;
            
            case 10: tx_msg_to_buf(tx_msg_rom[tx_msg_ERR]);
                tx_start(3 , tx_buf);
                code_state = code_state_STOP;
                break;
        }
    }
      


}
