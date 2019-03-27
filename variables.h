char rx_pack_n = 0;          // номер принятой посылки
signed char rx_n = 0;        // номер принятого байта; 0 - начало приема, (-1) - найдена преамбула
char rx_buf[3] = {0, 0, 0};  // буфер под пакет данных

#define rx_pack_n_MAX 10     // количество пакетов, сброс номера при превышении
char rx_buf_copy[3];         // буфер для копирования данных из переменных обработчика прерывания uart
signed char rx_n_copy;       // --//--
char rx_pack_n_copy;         // --//--
//char rx_pack_calc;           // для промежуточных расчетов
char rx_pack_n_prev = 0;     // номер принятой предыдущей посылки
char rx_pack_n_mc;           // номер посылки внутри МК (для входящих посылок)
char rx_synch_f = 0;         // флаг синхронизации, гарантия что дальше данные не с середины байта и не с середины пакета
char rx_pream_n;             // кол-во совпадений преамбулы
#define rx_PREAM 'i'//0xFF        // преамбула
char rx_crc;                 // буфер под промежуточные вычисления crc входящих данных

char tx_crc[3];              // буфер для ответной посылки, используется при вычислении crc
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

char code_ready = 0;         // готовность кода для выполнения
#define code_ready_NOT 0
#define code_ready_OK 1
#define code_ready_ERR_CRC 2
#define code_ready_ERR_N 3
#define code_ready_ERR_C 4

char code_buf[2];            // код для выполнения
char code_num;               // номер, вычлененный из принятой посылки
#define code_state_RD_P1 0                    // | --- управление выходами ---
#define code_state_WR_P1 1                    // | запись p1
#define code_state_IM_P1 2                    // | импульс p1
#define code_state_RD_P3 3                    // | чтение p3
#define code_state_WR_P3 4                    // |
#define code_state_IM_P3 5                    // |
#define code_state_WR_R  6                    // | запись в регистр
#define code_state_IM_R  7                    // | ---------------------------

#define code_state_ERR         8               // ошибки (реакция на ошибки)
#define code_state_ERR_PREAM     40              // ошибка преамбулы
#define code_state_ERR_CRC       41              // ошибка crc
#define code_state_ERR_N         42              // ошибка формата номера
#define code_state_ERR_C         43              // ошибка формата кода

#define code_state_OK          9               // подтверждения (реакция на подтверждения)
#define code_state_OK_ST         50              // подтверждение старта сессии
#define code_state_OK_END        51              // подтверждение конца сессии

#define code_state_SES        10               // управление сессией
#define code_state_SES_ST        60              // начало сессии
#define code_state_SES_END       61              // конец сессии

#define code_state_MSG_ERR_PREAM 70            // отправка сообщения о ошибке/преамбула
#define code_state_MSG_ERR_CRC   71            // -//- ошибке/crc
#define code_state_MSG_ERR_N     72            // -//- ошибке/номер входящего сообщения
#define code_state_MSG_ERR_C     73            // -//- ошибке/команда входящего сообщения

#define code_state_MSG_OK_ST     80            // отправка сообщения с подтверждением/старт сессии
#define code_state_MSG_OK_END    81            // -//- подтверждением/конец сессии

#define code_state_CRC0          90            // crc, вычисление байта 0
#define code_state_CRC1          91            // crc, вычисление байта 1
#define code_state_CRC2          92            // crc, отправка в выходной буфер передатчика
#define code_state_STOP       99                   // стоп автомата
char code_state = code_state_STOP;  // состояние исполнителя кода
#define code_state_rx_MAX 10 << 4              // максимальный допустимый принятый номер команды, сдвинутый влево
