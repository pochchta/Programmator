#line 1 "C:/Programmator/prog.c"
#line 1 "c:/programmator/variables.h"
char rx_pack_n = 0;
signed char rx_n = 0;
char rx_buf[3] = {0, 0, 0};

char rx_buf_copy[3];
signed char rx_n_copy;
char rx_pack_n_prev = 0;
char rx_synch_f = 0;
char rx_pream_n;


char *tx_pointer;
char tx_n = 0;
char tx_buf[3];

char crc_state = 0;








char code_ready = 0;




char code_buf[2];

char code_state =  99 ;
#line 1 "c:/programmator/functions.h"
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

void tx_start(char tx_start_input, char *tx_start_pointer){
 tx_n = tx_start_input;
 tx_pointer = tx_start_pointer;
 if (tx_n > 3) SBUF =  'i' ;
 else SBUF = *(tx_pointer + 3 - tx_n);
 tx_n--;
}
#line 7 "C:/Programmator/prog.c"
char db,db_i, db_y=1;
char *db_p;




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
 if (tx_n > 0) {
 if (tx_n > 3) SBUF =  'i' ;
 else SBUF = *(tx_pointer + 3 - tx_n);
 tx_n--;
 }
 SCON.TI = 0;
 }
}


volatile const char tx_msg_rom[2][4] = {"ERR" , "OK!"};



void tx_msg_to_buf(const char *tx_msg_input){
 char tx_msg_i;
 for (tx_msg_i = 0 ; tx_msg_i < 3 ; tx_msg_i++){
 tx_buf[tx_msg_i] = tx_msg_input[tx_msg_i];
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
#line 100 "C:/Programmator/prog.c"
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
 code_ready =  1 ;
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
 code_state = 0;
 } else {
 code_state = 10;
 }
 code_ready =  0 ;
 }
 }

 switch (code_state){
 case 0: tx_start(3 , rx_buf_copy);
 code_state =  99 ;
 break;

 case 10: tx_msg_to_buf(tx_msg_rom[ 0 ]);
 tx_start(3 , tx_buf);
 code_state =  99 ;
 break;
 }
 }



}
