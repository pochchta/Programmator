
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

/* ������������������ ������
0 - ����� ���������
1 - ������������� ���� �������������, ������� ����� (������ � rx_buf[]) ���� ������ crc ��� ����� ������� 0
2 - ����� � ����� ������ � ���������
3 - ��� ������ � 0,1 ��������� crc8(), �������������� �������
4 - ���������� ����������� crc8 � ������ � 2
5 - ���� ���������� ������� ��������� � crc8 ������������, �� �������� ����� �������
6 - ������� ������ � ����� ����������� �� ����������, ������� ������ �������������� ����� ������� � ��
7 - �� ������ ��������� �������� �� �������� � ���������
*/



char rx_n = 0;               // ����� ��������� �����; 0 - ������ ������
char rx_buf[3] = {0, 0, 0};  // ����� ��� ����� ������
char rx_pack_n = 0;          // ����� �������� �������

char rx_pack_n_prev = 0;     // ����� �������� ���������� �������
char rx_synch_f = 0;         // ���� �������������, �������� ��� ������ ������ �� � �������� ����� � �� � �������� ������
char rx_pream_n;             // ���-�� ���������� ���������
#define rx_PREAM 0xFF        // ���������

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

void UART(void) org 0x0023 {
    if (SCON.RI) {    // ���������� ����
        if (rx_n == 3) {
            rx_n = 0;
            rx_pack_n++;
            if (rx_pack_n == 10) rx_pack_n = 0;
        }
        rx_buf[rx_n] = SBUF;
        rx_n++;
        SCON.RI = 0;
    }
    if (SCON.TI) {    // ����������� ��������
      /*TX_wait_data = 1;
        SBUF = SBUF_buf;
        SCON.TI = 0;*/
    }
}
//--------------------------------------------------------------------------------------------------------


void main() {
    while (1) {
        if (rx_synch_f == 0) {                   // �������������
            rx_pream_n = 0;
            for (rx_n = 3; rx_n == 0; rx_n --) if (rx_buf[rx_n] == rx_PREAM) rx_pream_n++;
            if (rx_pream_n > 1) {
                rx_synch_f = 1;
                rx_n = 0;
                crc_state = 0;
            }
        }

        if (rx_pack_n != rx_pack_n_prev) {       // ��������� ����� �������
            crc_state = 0;
            rx_pack_n_prev = rx_pack_n;
        }

        if (rx_synch_f == 1) {                   // �������� ������
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
                if (crc == rx_buf[2] /*&& ������ �������*/) {
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

        /* ����������� ���� */
    }
      


}
