
char crc;
#define CRC8_POLYNOM #0xD5
void crc8(){  // CRC-8/DVB-S2
     char i_crc;
     for (i_crc = 8 ; i_crc ; i_crc--){
          asm {
               mov A, _crc
               clr C
               rlc A
               mov _crc, A
               JNC M_CRC_END
               xrl A, CRC8_POLYNOM
               mov _crc, A
               M_CRC_END:
          }
     }
}

/* Последовательность приема
0 - ловим преамбулу
1 - устанавливаем флаг синхронизации, сбросим потом (вместе с rx_buf[]) если ошибка crc или номер команды 0
2 - пишем в буфер данные с приемника
3 - для байтов № 0,1 выполняем crc8(), предварительно обнулив
4 - сравниваем вычисленную crc8 с байтом № 2
5 - если предыдущая команда выполнена и crc8 подтверждена, то вносится новая команда
6 - команды начала и конца дублируются на передатчик, команда начала перезаписывает номер команды в МК
7 - на запрос повторной передачи ПК начинает с преамбулы
*/



char rx_n = 0;               // номер принятого байта; 0 - начало приема
char rx_buf[3] = {0, 0, 0};  // буфер под пакет данных
char rx_pack_n = 0;          // номер принятой посылки

char rx_pack_n_prev = 0;     // номер принятой предыдущей посылки
char rx_synch_f = 0;         // флаг синхронизации, гарантия что дальше данные не с середины байта и не с середины пакета
char rx_pream_n;             // кол-во совпадений преамбулы
#define rx_PREAM 0xFF        // преамбула

char crc_state = 0;          // состояние crc8(), см. help
#define crc_state_0 0          // начало счета
#define crc_state_OK 3         // успешно
#define crc_state_ERR 4        // ошибка

//char code_work_f = 0;      // флаг в работе
//char code_rx_n = 0;        // номер команды  0...9 из принятой посылки
//char code_in_n = 0;        // номер команды внутри МК

char code_ready_f = 0;       // флаг готовности кода для выполнения
char code_buf[2];            // код для выполнения
#define code_state_STOP 10
char code_state = code_state_STOP;  // состояние исполнителя кода

void UART(void) org 0x0023 {
    if (SCON.RI) {    // закончился приём
        if (rx_n == 3) {
            rx_n = 0;
            rx_pack_n++;
            if (rx_pack_n == 10) rx_pack_n = 0;
        }
        rx_buf[rx_n] = SBUF;
        rx_n++;
        SCON.RI = 0;
    }
    if (SCON.TI) {    // закончилась передача
      /*TX_wait_data = 1;
        SBUF = SBUF_buf;
        SCON.TI = 0;*/
    }
}
//--------------------------------------------------------------------------------------------------------


void main() {
    while (1) {
        if (rx_synch_f == 0) {                   // синхронизация
            rx_pream_n = 0;
            for (rx_n = 3; rx_n == 0; rx_n --) if (rx_buf[rx_n] == rx_PREAM) rx_pream_n++;
            if (rx_pream_n > 1) {
                rx_synch_f = 1;
                rx_n = 0;
                crc_state = 0;
            }
        }

        if (rx_pack_n != rx_pack_n_prev) {       // получение новой посылки
            crc_state = 0;
            rx_pack_n_prev = rx_pack_n;
        }

        if (rx_synch_f == 1) {                   // проверка данных
            if (crc_state == 0 && rx_n > 0) {
                crc = rx_buf[0];
                crc8();
                crc_state++;
            }
            if (crc_state == 1 && rx_n > 1) {
                crc ^= rx_buf[1];
                crc8();
                crc_state++;
            }
            if (crc_state == 2 && rx_n > 2) {
                if (crc == rx_buf[2] /*&& формат посылки*/) {
                    crc_state = crc_state_OK;
                    code_ready_f = 1;
                } else rx_synch_f = 0;
            }
            if (code_ready_f == 1 && code_state == code_state_STOP) {
                code_buf[0] = rx_buf[0];
                code_buf[1] = rx_buf[1];
                code_state = 0;
                code_ready_f = 0;
            }
        }

        /* исполнитель кода */
    }
      


}
