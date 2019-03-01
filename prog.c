/*char data_rx;
char data_rx_buf, data_rx_crc;



unsigned int test = 10;*/
char test_i;


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

//-------------------------------------UART---------------------------------------------------------------
/* Формат пакета       _
* - FF                |  преамбула
* - FF                |_ только для синхронизации
0 - N + (code << 4)
1 - data
2 - crc
*/

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
char rx_n;                 // номер принятого байта; 0 - начало приема
char rx_buf[3] = {0, 0, 0};// буфер под пакет данных
char rx_buf_pre = 0;       // буфер под 1 байт преамбулы
char rx_buf_f = 0;         // флаг буфер занят
char rx_synch_f = 0;       // флаг синхронизации, гарантия что дальше данные не с середины байта и не с середины пакета

char crc_state = 0;        // состояние crc8(), см. help

char code_work_f = 0;      // флаг в работе
char code_rx_n = 0;        // номер команды  0...9 из принятой посылки
char code_in_n = 0;        // номер команды внутри МК

void UART(void) org 0x0023 {
    if (SCON.RI) {    // закончился приём
  
        if (rx_buf_f == 1);
  
  
  
    rx_buf[2] = rx_buf[1];
    rx_buf[1] = rx_buf[0];
    rx_buf[0] = SBUF;
    
    if (rx_synch_f == 0) {
      if ( (rx_buf[1] & rx_buf[2]) == 0xFF ) { // преамбула 0xffff не должна встречаться в пакете данных
         rx_synch_f = 1;
         rx_n = 0;
      }
      
    } else {

    }
    
    SCON.RI = 0;
  }
  if (SCON.TI) {    // закончилась передача
/*TX_wait_data = 1;
    SBUF = SBUF_buf;
    SCON.TI = 0;*/
  }
}
//--------------------------------------------------------------------------------------------------------
      
      volatile char test1 = 1;
      volatile char test2 = 2;
void main() {
      if (test1 == 1 && test2 == 2) p


      if (rx_n == 0) {
         crc = SBUF;
         crc8();
      }
      if (rx_n == 1) {
         crc ^= SBUF;
         crc8();
      }
      if (rx_n == 2) {
//         if (crc == SBUF) rx_code_check = 1;
//         else rx_synch = 0;
      }
      rx_n++;
      if (rx_n == 3) {
         rx_n = 0;
         code_rx_n++;
         if (code_rx_n > 9) code_rx_n = 0;
      }
      


     for (test_i = 255; test_i; ) {
         crc = test_i;
         crc8();
         test_i--;
     }
     crc ^= 0x10;
     crc8();

/*crc = 0xF0;    // 0xF010  -> 0x43
     for (test_i = 8; test_i; test_i--)
          crc = crc.F7 ? (crc << 1) ^ 0xd5 : crc << 1;
     crc = crc ^ 0x10;
     for (test_i = 8; test_i; test_i--)
         crc = crc.F7 ? (crc << 1) ^ 0xd5 : crc << 1;*/


//     test = 10;
}
