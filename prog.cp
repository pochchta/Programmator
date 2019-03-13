#line 1 "C:/Programmator/prog.c"

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
#line 40 "C:/Programmator/prog.c"
char rx_n;
char rx_buf[3] = {0, 0, 0};
char rx_buf_pre = 0;

char rx_buf_f = 0;
char rx_synch_f = 0;

char crc_state = 0;




char code_work_f = 0;
char code_rx_n = 0;
char code_in_n = 0;


void UART(void) {
 if (SCON.RI) {
 if (crc_state ==  3 ) {
 rx_synch_f = 0;
 rx_buf_pre = 0;
 }
 if (rx_buf_f == 0) {
 if (rx_synch_f == 1) {
 rx_buf[rx_n] = SBUF;
 if (++rx_n == 3) {
 rx_n = 0;
 rx_buf_f = 1;
 }
 } else {
 if (rx_buf_pre ==  0xFF  && SBUF ==  0xFF ) {
 rx_synch_f = 1;
 rx_n = 0;
 crc_state = 0;
 }
 rx_buf_pre = SBUF;
 }
 } else {
 rx_buf_f = 0;
 rx_synch_f = 0;
 rx_buf_pre = 0;
 }

 SCON.RI = 0;
 }
 if (SCON.TI) {
#line 90 "C:/Programmator/prog.c"
 }
}



void main() {


 while (1) {
SCON.RI = 1;
 UART();
 }

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
#line 141 "C:/Programmator/prog.c"
}
