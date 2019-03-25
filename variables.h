char rx_pack_n = 0;          // ����� �������� �������
signed char rx_n = 0;        // ����� ��������� �����; 0 - ������ ������, (-1) - ������� ���������
char rx_buf[3] = {0, 0, 0};  // ����� ��� ����� ������

char rx_buf_copy[3];         // ����� ��� ����������� ������ �� ���������� ����������� ���������� uart
signed char rx_n_copy;       // --//--
char rx_pack_n_prev = 0;     // ����� �������� ���������� �������
char rx_synch_f = 0;         // ���� �������������, �������� ��� ������ ������ �� � �������� ����� � �� � �������� ������
char rx_pream_n;             // ���-�� ���������� ���������
#define rx_PREAM 'i'//0xFF        // ���������
char rx_crc;

char tx_crc;
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

//char code_work_f = 0;      // ���� � ������
//char code_rx_n = 0;        // ����� �������  0...9 �� �������� �������
//char code_in_n = 0;        // ����� ������� ������ ��

char code_ready = 0;         // ���������� ���� ��� ����������
#define code_ready_NOT 0
#define code_ready_OK 1
#define code_ready_ERR 2

char code_buf[2];            // ��� ��� ����������
#define code_state_rd_p1 0                    // | --- ���������� �������� ---
#define code_state_wr_p1 1                    // | ������ p1
#define code_state_im_p1 2                    // | ������� p1
#define code_state_rd_p3 3                    // | ������ p3
#define code_state_wr_p3 4                    // |
#define code_state_im_p3 5                    // |
#define code_state_wr_r  6                    // | ������ � �������
#define code_state_im_r  7                    // | ---------------------------

#define code_state_err         8               // ������ (������� �� ������)
#define code_state_err_pream     40              // ������ ���������
#define code_state_err_crc       41              // ������ crc
#define code_state_err_for       42              // ������ �������

#define code_state_ok          9               // ������������� (������� �� �������������)
#define code_state_ok_st         50              // ������������� ������ ������
#define code_state_ok_end        51              // ������������� ����� ������
#define code_state_ok_end        52              // ������������� ����� ������

#define code_state_ses        10               // ���������� �������
#define code_state_ses_st        60              // ������ ������
#define code_state_ses_end       61              // ����� ������

#define code_state_msg_err_pream 70            // �������� ��������� � ������/���������
#define code_state_msg_err_crc   71            // -//- ������/crc
#define code_state_msg_err_for   72            // -//- ������/������

#define code_state_msg_ok_st     80            // �������� ��������� � ��������������/����� ������
#define code_state_msg_ok_end    81            // -//- ��������������/����� ������

#define code_state_crc0          90            // crc, ���������� ����� 0
#define code_state_crc1          91            // crc, ���������� ����� 1
#define code_state_crc2          92            // crc, �������� � �������� �����
#define code_state_STOP       99                   // ���� ��������
char code_state = code_state_STOP;  // ��������� ����������� ����


