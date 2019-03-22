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

void tx_start(char tx_start_input, char *tx_start_pointer){
    tx_n = tx_start_input;
    tx_pointer = tx_start_pointer;
    if (tx_n > 3) SBUF = rx_PREAM;
    else SBUF = *(tx_pointer + 3 - tx_n);
    tx_n--;
}


