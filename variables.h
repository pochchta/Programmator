char rx_pack_n = 0;          // ����� �������� �������
signed char rx_n = 0;        // ����� ��������� �����; 0 - ������ ������, (-1) - ������� ���������
char rx_buf[3] = {0, 0, 0};  // ����� ��� ����� ������

char rx_buf_copy[3];         // ����� ��� ����������� ������ �� ���������� ����������� ���������� uart
signed char rx_n_copy;       // --//--
char rx_pack_n_prev = 0;     // ����� �������� ���������� �������
char rx_synch_f = 0;         // ���� �������������, �������� ��� ������ ������ �� � �������� ����� � �� � �������� ������
char rx_pream_n;             // ���-�� ���������� ���������
#define rx_PREAM 'i'//0xFF        // ���������

char *tx_pointer;            // ��������� �� ������ ������ ��������
char tx_n = 0;               // ���������� ������������ ���� (3 ��� 5)
char tx_buf[3];              // ����� ��� ������

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
#define code_state_STOP 99
char code_state = code_state_STOP;  // ��������� ����������� ����
