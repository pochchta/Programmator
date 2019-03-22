char rx_pack_n = 0;          // номер принятой посылки
signed char rx_n = 0;        // номер принятого байта; 0 - начало приема, (-1) - найдена преамбула
char rx_buf[3] = {0, 0, 0};  // буфер под пакет данных

char rx_buf_copy[3];         // буфер для копирования данных из переменных обработчика прерывания uart
signed char rx_n_copy;       // --//--
char rx_pack_n_prev = 0;     // номер принятой предыдущей посылки
char rx_synch_f = 0;         // флаг синхронизации, гарантия что дальше данные не с середины байта и не с середины пакета
char rx_pream_n;             // кол-во совпадений преамбулы
#define rx_PREAM 'i'//0xFF        // преамбула

char *tx_pointer;            // указатель на начало пакета передачи
char tx_n = 0;               // количество передаваемых байт (3 или 5)
char tx_buf[3];              // буфер под данные

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
#define code_state_STOP 99
char code_state = code_state_STOP;  // состояние исполнителя кода
