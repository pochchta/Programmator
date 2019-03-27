char rx_pack_n = 0;          // ����� �������� �������
signed char rx_n = 0;        // ����� ��������� �����; 0 - ������ ������, (-1) - ������� ���������
char rx_buf[3] = {0, 0, 0};  // ����� ��� ����� ������

#define rx_pack_n_MAX 10     // ���������� �������, ����� ������ ��� ����������
char rx_buf_copy[3];         // ����� ��� ����������� ������ �� ���������� ����������� ���������� uart
signed char rx_n_copy;       // --//--
char rx_pack_n_copy;         // --//--
//char rx_pack_calc;           // ��� ������������� ��������
char rx_pack_n_prev = 0;     // ����� �������� ���������� �������
char rx_pack_n_mc;           // ����� ������� ������ �� (��� �������� �������)
char rx_synch_f = 0;         // ���� �������������, �������� ��� ������ ������ �� � �������� ����� � �� � �������� ������
char rx_pream_n;             // ���-�� ���������� ���������
#define rx_PREAM 'i'//0xFF        // ���������
char rx_crc;                 // ����� ��� ������������� ���������� crc �������� ������

char tx_crc[3];              // ����� ��� �������� �������, ������������ ��� ���������� crc
char *tx_pointer;            // ��������� �� ������ ������ ��������
char tx_n = 0;               // ���������� ������������ ���� (3 ��� 5)
char tx_buf[3];              // ����� ��� ������
const char tx_msg_rom[2][4] = {"ERR" , "OK!"};
#define tx_msg_ERR 0
#define tx_msg_OK 1

char crc_state = 0;          // ��������� crc8(), ��. help
#define crc_state_0 0          // ������ �����
#define crc_state_OK 3         // �������
#define crc_state_ERR 4        // ������

char code_ready = 0;         // ���������� ���� ��� ����������
#define code_ready_NOT 0
#define code_ready_OK 1
#define code_ready_ERR_CRC 2
#define code_ready_ERR_N 3
#define code_ready_ERR_C 4

char code_buf[2];            // ��� ��� ����������
char code_num;               // �����, ����������� �� �������� �������
#define code_state_RD_P1 0                    // | --- ���������� �������� ---
#define code_state_WR_P1 1                    // | ������ p1
#define code_state_IM_P1 2                    // | ������� p1
#define code_state_RD_P3 3                    // | ������ p3
#define code_state_WR_P3 4                    // |
#define code_state_IM_P3 5                    // |
#define code_state_WR_R  6                    // | ������ � �������
#define code_state_IM_R  7                    // | ---------------------------

#define code_state_ERR         8               // ������ (������� �� ������)
#define code_state_ERR_PREAM     40              // ������ ���������
#define code_state_ERR_CRC       41              // ������ crc
#define code_state_ERR_N         42              // ������ ������� ������
#define code_state_ERR_C         43              // ������ ������� ����

#define code_state_OK          9               // ������������� (������� �� �������������)
#define code_state_OK_ST         50              // ������������� ������ ������
#define code_state_OK_END        51              // ������������� ����� ������

#define code_state_SES        10               // ���������� �������
#define code_state_SES_ST        60              // ������ ������
#define code_state_SES_END       61              // ����� ������

#define code_state_MSG_ERR_PREAM 70            // �������� ��������� � ������/���������
#define code_state_MSG_ERR_CRC   71            // -//- ������/crc
#define code_state_MSG_ERR_N     72            // -//- ������/����� ��������� ���������
#define code_state_MSG_ERR_C     73            // -//- ������/������� ��������� ���������

#define code_state_MSG_OK_ST     80            // �������� ��������� � ��������������/����� ������
#define code_state_MSG_OK_END    81            // -//- ��������������/����� ������

#define code_state_CRC0          90            // crc, ���������� ����� 0
#define code_state_CRC1          91            // crc, ���������� ����� 1
#define code_state_CRC2          92            // crc, �������� � �������� ����� �����������
#define code_state_STOP       99                   // ���� ��������
char code_state = code_state_STOP;  // ��������� ����������� ����
#define code_state_rx_MAX 10 << 4              // ������������ ���������� �������� ����� �������, ��������� �����
