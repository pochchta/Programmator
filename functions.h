char crc;                    // глобальная, т.к. используется в ассемблерной вставке
#define CRC8_POLYNOM #0x07   // был #0xD5, у полинома 0x07 лучше перемешивание ?
char crc8(char crc8_input){  // CRC-8/DVB-S2
     char i_crc;
     crc = crc8_input;
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
     return crc;
}

void tx_start(char tx_start_input, char *tx_start_pointer){     // начало передачи
    tx_n = tx_start_input;
    tx_pointer = tx_start_pointer;
    if (tx_n > 3) SBUF = rx_PREAM;
    else SBUF = *(tx_pointer + 3 - tx_n);
    tx_n--;
}

void tx_msg_to_buf(const char *tx_msg_input){                  // запись текста rom в tx_buf[]
    char tx_msg_i;
    for (tx_msg_i = 0 ; tx_msg_i < 3 ; tx_msg_i++){
        tx_buf[tx_msg_i] = tx_msg_input[tx_msg_i];
    }
}
