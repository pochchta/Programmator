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
#line 34 "C:/Programmator/prog.c"
char rx_pack_n = 0;
signed char rx_n = 0;
char rx_buf[3] = {0, 0, 0};

char rx_buf_copy[3];
signed char rx_n_copy;
char rx_pack_n_prev = 0;
char rx_synch_f = 0;
char rx_pream_n;


char crc_state = 0;








char code_ready_f = 0;
char code_buf[2];

char code_state =  10 ;


void UART(void) org 0x0023 {
 if (SCON.RI) {
 if (rx_n == 3 || rx_n < 0) {
 rx_n = 0;
 rx_pack_n++;
 if (rx_pack_n == 10) rx_pack_n = 0;
 }
 rx_buf[rx_n] = SBUF;
 rx_n++;
 SCON.RI = 0;
 }
 if (SCON.TI) {
#line 75 "C:/Programmator/prog.c"
 }
}



void main() {

 P1 = 0XFF;
 P3 = 0XFF;
 PCON.F7 = 0;
 TMOD = 0b00100001;








 TL0 = 0x89;
 TH0 = 0xE6;
 TH1 = 0xFA;
 SCON = 0;
 SCON.SM0 = 0;
 SCON.SM1 = 1;
 SCON.REN = 1;
 TCON = 0;
 TCON.TR1 = 1;
 TCON.TR0 = 1;
 IE.EA = 1;
 IE.ES = 1;



 while (1) {


 if (rx_synch_f == 0) {
 rx_pream_n = 0;
 IE.ES = 0;
 for (rx_n = 0; rx_n < 3; rx_n++) if (rx_buf[rx_n] ==  0xFF ) rx_pream_n++;
 if (rx_pream_n > 1) {
 rx_synch_f = 1;
 rx_n = -1;
 crc_state = 0;
 }
 IE.ES = 1;
 }

 if (rx_pack_n != rx_pack_n_prev) {
 crc_state = 0;
 rx_pack_n_prev = rx_pack_n;
 }

 if (rx_synch_f == 1) {
 IE.ES = 0;
 rx_buf_copy[0] = rx_buf[0];
 rx_buf_copy[1] = rx_buf[1];
 rx_buf_copy[2] = rx_buf[2];
 rx_n_copy = rx_n;
 IE.ES = 1;
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
 if (crc == rx_buf_copy[2] ) {
 crc_state =  3 ;
 code_ready_f = 1;
 } else rx_synch_f = 0;
 }
 if (code_ready_f == 1 && code_state ==  10 ) {
 code_buf[0] = rx_buf_copy[0];
 code_buf[1] = rx_buf_copy[1];
 code_state = 0;
 code_ready_f = 0;
 }
 }


 }



}
