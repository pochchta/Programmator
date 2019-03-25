char rx_pack_n = 0;          // номер принятой посылки
signed char rx_n = 0;        // номер принятого байта; 0 - начало приема, (-1) - найдена преамбула
char rx_buf[3] = {0, 0, 0};  // буфер под пакет данных

char rx_buf_copy[3];         // буфер для копирования данных из переменных обработчика прерывания uart
signed char rx_n_copy;       // --//--
char rx_pack_n_prev = 0;     // номер принятой предыдущей посылки
char rx_synch_f = 0;         // флаг синхронизации, гарантия что дальше данные не с середины байта и не с середины пакета
char rx_pream_n;             // кол-во совпадений преамбулы
#define rx_PREAM 'i'//0xFF        // преамбула
char rx_crc;

char tx_crc;
char *tx_pointer;            // указатель на начало пакета передачи
char tx_n = 0;               // количество передаваемых байт (3 или 5)
char tx_buf[3];              // буфер под данные
const char tx_msg_rom[2][4] = {"ERR" , "OK!"};
#define tx_msg_ERR 0
#define tx_msg_OK 1

char crc_state = 0;          // состояние crc8(), см. help
#define crc_state_0 0          // начало счета
#define crc_state_OK 3         // успешно
#define crc_state_ERR 4        // ошибка

//char code_work_f = 0;      // флаг в работе
//char code_rx_n = 0;        // номер команды  0...9 из принятой посылки
//char code_in_n = 0;        // номер команды внутри МК

char code_ready = 0;         // готовность кода для выполнения
#define code_ready_NOT 0
#define code_ready_OK 1
#define code_ready_ERR 2

char code_buf[2];            // код для выполнения
#define code_state_rd_p1 0                    // | --- управление выходами ---
#define code_state_wr_p1 1                    // | запись p1
#define code_state_im_p1 2                    // | импульс p1
#define code_state_rd_p3 3                    // | чтение p3
#define code_state_wr_p3 4                    // |
#define code_state_im_p3 5                    // |
#define code_state_wr_r  6                    // | запись в регистр
#define code_state_im_r  7                    // | ---------------------------

#define code_state_err         8               // ошибки (реакция на ошибки)
#define code_state_err_pream     40              // ошибка преамбулы
#define code_state_err_crc       41              // ошибка crc
#define code_state_err_for       42              // ошибка формата

#define code_state_ok          9               // подтверждения (реакция на подтверждения)
#define code_state_ok_st         50              // подтверждение старта сессии
#define code_state_ok_end        51              // подтверждение конца сессии
#define code_state_ok_end        52              // подтверждение конца сессии

#define code_state_ses        10               // управление сессией
#define code_state_ses_st        60              // начало сессии
#define code_state_ses_end       61              // конец сессии

#define code_state_msg_err_pream 70            // отправка сообщения о ошибке/преамбула
#define code_state_msg_err_crc   71            // -//- ошибке/crc
#define code_state_msg_err_for   72            // -//- ошибке/формат

#define code_state_msg_ok_st     80            // отправка сообщения с подтверждением/старт сессии
#define code_state_msg_ok_end    81            // -//- подтверждением/конец сессии

#define code_state_crc0          90            // crc, вычисление байта 0
#define code_state_crc1          91            // crc, вычисление байта 1
#define code_state_crc2          92            // crc, отправка в выходной буфер
#define code_state_STOP       99                   // стоп автомата
char code_state = code_state_STOP;  // состояние исполнителя кода


