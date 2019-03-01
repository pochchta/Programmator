#line 1 "C:/Programmator/prog.c"
#line 7 "C:/Programmator/prog.c"
char test_i;


char crc;

void crc8(){
 char i_crc;
 for (i_crc = 8 ; i_crc ; i_crc--){
 asm {
 mov A, _crc
 clr C
 rlc A
 mov _crc, A
 JNC M_CRC_END
 xrl A,  #0xD5 
 mov _crc, A
 M_CRC_END:
 }
 }
}
#line 47 "C:/Programmator/prog.c"
char rx_n;
char rx_buf[3] = {0, 0, 0};
char rx_buf_pre = 0;
char rx_buf_f = 0;
char rx_synch_f = 0;

char crc_state = 0;

char code_work_f = 0;
char code_rx_n = 0;
char code_in_n = 0;

void UART(void) org 0x0023 {
 if (SCON.RI) {

 if (rx_buf_f == 1);



 rx_buf[2] = rx_buf[1];
 rx_buf[1] = rx_buf[0];
 rx_buf[0] = SBUF;

 if (rx_synch_f == 0) {
 if ( (rx_buf[1] & rx_buf[2]) == 0xFF ) {
 rx_synch_f = 1;
 rx_n = 0;
 }

 } else {

 }

 SCON.RI = 0;
 }
 if (SCON.TI) {
#line 86 "C:/Programmator/prog.c"
 }
}


 volatile char test1 = 1;
 volatile char test2 = 2;
void main() {
 if (


 if (rx_n == 0) {
 crc = SBUF;
 crc8();
 }
 if (rx_n == 1) {
 crc ^= SBUF;
 crc8();
 }
 if (rx_n == 2) {


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
#line 134 "C:/Programmator/prog.c"
}
