#line 1 "C:/Programmator/prog.c"
#line 1 "c:/programmator/variables.h"
char rx_pack_n = 0;
signed char rx_n = 0;
char rx_buf[3] = {0, 0, 0};


char rx_buf_copy[3];
signed char rx_n_copy;
char rx_pack_n_copy;

char rx_pack_n_prev = 0;
char rx_pack_n_mc;
char rx_synch_f = 0;
char rx_pream_n;

char rx_crc;

char tx_crc;
char *tx_pointer;
char tx_n = 0;
char tx_buf[3];
const char tx_msg_rom[2][4] = {"ERR" , "OK!"};



char crc_state = 0;




char code_ready = 0;






char code_buf[2];
#line 72 "c:/programmator/variables.h"
char code_state =  99 ;
#line 1 "c:/programmator/functions.h"
char crc;

char crc8(char crc8_input){
 char i_crc;
 crc = crc8_input;
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
 return crc;
}

void tx_start(char tx_start_input, char *tx_start_pointer){
 tx_n = tx_start_input;
 tx_pointer = tx_start_pointer;
 if (tx_n > 3) SBUF =  'i' ;
 else SBUF = *(tx_pointer + 3 - tx_n);
 tx_n--;
}

void tx_msg_to_buf(const char *tx_msg_input){
 char tx_msg_i;
 for (tx_msg_i = 0 ; tx_msg_i < 3 ; tx_msg_i++){
 tx_buf[tx_msg_i] = tx_msg_input[tx_msg_i];
 }
}
#line 7 "C:/Programmator/prog.c"
char db,db_i, db_y=1;
char *db_p;




void UART(void) org 0x0023 {
 if (SCON.RI) {
 if (rx_n == 3 || rx_n < 0) {
 rx_n = 0;
 rx_pack_n++;
 if (rx_pack_n ==  10 ) rx_pack_n = 0;
 }
 rx_buf[rx_n] = SBUF;
 rx_n++;
 SCON.RI = 0;
 }
 if (SCON.TI) {
 if (tx_n > 0) {
 if (tx_n > 3) SBUF =  'i' ;
 else SBUF = *(tx_pointer + 3 - tx_n);
 tx_n--;
 }
 SCON.TI = 0;
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
#line 88 "C:/Programmator/prog.c"
 if (rx_synch_f == 0) {
 rx_pream_n = 0;
 IE.ES = 0;
 for (rx_n_copy = 0; rx_n_copy < 3; rx_n_copy++) if (rx_buf[rx_n_copy] ==  'i' ) rx_pream_n++;
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
 rx_pack_n_copy = rx_pack_n;
 IE.ES = 1;
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
 crc_state =  3 ;
 code_ready =  1 ;
 if ((rx_buf_copy[0] & 0b00001111) >=  10  || (rx_buf_copy[0] & 0b00001111) != rx_pack_n_copy) {
 if (code_state !=  60 )
 code_ready =  3 ;
 }
 if (rx_buf_copy[0] & 0b11110000 >  10 << 4 ) {
 code_ready =  4 ;
 }
 } else {
 crc_state =  4 ;
 code_ready =  2 ;
 rx_synch_f = 0;
 }
 }
 if (code_ready !=  0  && code_state ==  99 ) {
 code_buf[0] = rx_buf_copy[0];
 code_buf[1] = rx_buf_copy[1];
 if (code_ready ==  1 ) {

 }
 if (code_ready ==  2 ) {
 code_state =  71 ;
 }
 if (code_ready ==  3 ) {
 code_state =  72 ;
 }
 if (code_ready ==  4 ) {
 code_state =  73 ;
 }
 code_ready =  0 ;
 }
 }

 switch (code_state){
#line 166 "C:/Programmator/prog.c"
 case  0 :

 break;
 case  1 :

 break;
 case  2 :

 break;
 case  3 :

 break;
 case  4 :

 break;
 case  5 :

 break;
 case  6 :

 break;
 case  7 :

 break;

 case  8 :

 break;
 case  40 :

 break;
 case  41 :

 break;
 case  42 :

 break;

 case  9 :

 break;
 case  50 :

 break;
 case  51 :

 break;

 case  10 :

 break;
 case  60 :

 break;
 case  61 :

 break;

 case  70 :

 break;
 case  71 :

 break;
 case  72 :

 break;
 case  73 :

 break;

 case  80 :

 break;
 case  81 :

 break;

 case  90 :

 break;
 case  91 :

 break;
 case  92 :

 break;
 }
 }
}
