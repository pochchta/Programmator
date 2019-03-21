
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

char rx_pack_n = 0;          // ����� �������� �������
signed char rx_n = 0;        // ����� ��������� �����; 0 - ������ ������, (-1) - ������� ���������
char rx_buf[3] = {0, 0, 0};  // ����� ��� ����� ������

char rx_buf_copy[3];         // ����� ��� ����������� ������ �� ���������� ����������� ���������� uart
signed char rx_n_copy;       // --//--
char rx_pack_n_prev = 0;     // ����� �������� ���������� �������
char rx_synch_f = 0;         // ���� �������������, �������� ��� ������ ������ �� � �������� ����� � �� � �������� ������
char rx_pream_n;             // ���-�� ���������� ���������
#define rx_PREAM 'i'//0xFF        // ���������

char *tx_pointer;
char tx_n = 0;

char crc_state = 0;          // ��������� crc8(), ��. help
#define crc_state_0 0          // ������ �����
#define crc_state_OK 3         // �������
#define crc_state_ERR 4        // ������

//char code_work_f = 0;      // ���� � ������
//char code_rx_n = 0;        // ����� �������  0...9 �� �������� �������
//char code_in_n = 0;        // ����� ������� ������ ��

char code_ready_f = 0;       // ���� ���������� ���� ��� ����������
char code_buf[2];            // ��� ��� ����������
#define code_state_STOP 10
char code_state = code_state_STOP;  // ��������� ����������� ����


/*char db,db_i, db_y=1;              // debug
void db_start(void){
    SBUF = db;
}*/


void UART(void) org 0x0023 {
    if (SCON.RI) {    // ���������� ����
        if (rx_n == 3 || rx_n < 0) {
            rx_n = 0;
            rx_pack_n++;
            if (rx_pack_n == 10) rx_pack_n = 0;
        }
        rx_buf[rx_n] = SBUF;
        rx_n++;
        SCON.RI = 0;
    }
    if (SCON.TI) {    // ����������� ��������
        if (tx_n > 0) {
            if (tx_n > 3) SBUF = rx_PREAM;
            else SBUF = *(tx_pointer + 3 - tx_n);
            tx_n--;
        }
        SCON.TI = 0;
    }
}

void tx_start(char tx_start_input, char *tx_start_pointer){
    tx_n = tx_start_input;
    tx_pointer = tx_start_pointer;
    if (tx_n > 3) SBUF = rx_PREAM;
    else SBUF = *(tx_pointer + 3 - tx_n);
    tx_n--;
}
//--------------------------------------------------------------------------------------------------------






void main() {

    P1 = 0XFF;
    P3 = 0XFF;
    PCON.F7 = 0;             // �� ��������� �������� ��������
    TMOD = 0b00100001;       // ��������� ��������
    //       |||||||+-----     M0 (T0) = 1, ����� 1 (0b01) ������ 0; ����� 1 - 16 ������ �/�
    //       ||||||+------     M1 (T0) = 0, ����� 1 (0b01) ������ 0; ����� 1 - 16 ������ �/�
    //       |||||+-------     ���������� �������� ������ 0
    //       ||||+--------     ���������� ����., ������ 0
    //       |||+---------     M0 (T1) = 0, ����� 2 (0b10) ������ 1; ����� 2 - ����������������� ������
    //       ||+----------     M1 (T1) = 1, ����� 2 (0b10) ������ 1; ����� 2 - ����������������� ������
    //       |+-----------     ���������� �������� ������ 1
    //       +------------     ���������� ����., ������ 1
    TL0 = 0x89;                // ��������� ������� 0 ��
    TH0 = 0xE6;                // 1500 �� - �.�. �������� ������� �����
    TH1 = 0xFA;              // ����������������� ����� ��� 4800 ����
    SCON = 0;
    SCON.SM0 = 0;              // ����� 1 - 8 ���, �� ������� 1
    SCON.SM1 = 1;              // ����� 1
    SCON.REN = 1;              // ���������� ������
    TCON = 0;
    TCON.TR1 = 1;            // ���� ������� 1
    TCON.TR0 = 1;            // ���� ������� 0
    IE.EA = 1;               // ��������� ��� ����������
    IE.ES = 1;                // ��������� ���������� �� UART
  //  IE.ET0 = 1;               // ��������� ���������� �� ������� 0
  //  IP.PT0 = 1;               // ��������� ���������� ������� 0

  //db_start();


    while (1) {
    
    //  delete
    
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

    //  delete

    
        if (rx_synch_f == 0) {                   // �������������
            rx_pream_n = 0;
            IE.ES = 0;                                                                   //  |-- ��������� ���������� �� UART
            for (rx_n_copy = 0; rx_n_copy < 3; rx_n_copy++) if (rx_buf[rx_n_copy] == rx_PREAM) rx_pream_n++; //  |
            if (rx_pream_n > 1) {                                                        //  |
                rx_synch_f = 1;                                                          //  |
                rx_n = -1;                                                               //  |
                crc_state = 0;                                                           //  |
            }                                                                            //  |
            IE.ES = 1;                                                                   //  |-- ��������� ���������� �� UART
        }

        if (rx_pack_n != rx_pack_n_prev) {       // ��������� ����� �������
            crc_state = 0;
            rx_pack_n_prev = rx_pack_n;
        }

        if (rx_synch_f == 1) {                   // �������� ������
            IE.ES = 0;                                  //  |-- ��������� ���������� �� UART
            rx_buf_copy[0] = rx_buf[0];                 //  |
            rx_buf_copy[1] = rx_buf[1];                 //  |
            rx_buf_copy[2] = rx_buf[2];                 //  |
            rx_n_copy = rx_n;                           //  |
            IE.ES = 1;                                  //  |-- ��������� ���������� �� UART
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
                if (crc == rx_buf_copy[2] /*&& ������ �������*/) {
                    crc_state = crc_state_OK;
                    code_ready_f = 1;
                } else {
                    crc_state = crc_state_ERR;
                    rx_synch_f = 0;
                }
            }
            if (code_ready_f == 1 && code_state == code_state_STOP) {
                code_buf[0] = rx_buf_copy[0];
                code_buf[1] = rx_buf_copy[1];
                code_state = 0;
                code_ready_f = 0;
            }
        }

        switch (code_state){                     // ����������� ����
            case 0: tx_start(3 , & rx_buf_copy[0]);
                code_state = code_state_STOP;
                break;
            
        
        }
    }
      


}
