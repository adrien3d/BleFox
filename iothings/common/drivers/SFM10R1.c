#include "SFM10R1.h"

#include "boards.h"

#ifdef FACTORY_TESTS
#define IOT_ERROR(...)
#define IOT_WARN(...)
#else
#include "iot_trace.h"
#endif

#define MAX_TEST_DATA_BYTES     (15U)

/*
    Test de Communication:      AT
    Récupérer le Module ID:     AT$I=10
    Récupérer le code PAC:      AT$I=11
    Envoyer un message SIGFOX:  AT$SF=XXXXXXXXXXXX (Valeur Hexadécimale)
    Envoyer un message SIGFOX avec trame de reception:  AT$SF=XXXXXXXXXXXX,1 (Valeur Hexadécimale)
*/

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void uart_init(void)
{
    uint32_t err_code;

    /*uint8_t * rx_buf;
    uint8_t * tx_buf;*/
    //bsp_board_leds_init();

    const app_uart_comm_params_t comm_params =
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud9600
      };

    /*const app_uart_buffers_t buffers =
        {
            rx_buf,
            UART_RX_BUF_SIZE,
            tx_buf,
            UART_TX_BUF_SIZE,  
        };*/
    //app_uart_init(&comm_params, &buffers, uart_error_handle, APP_IRQ_PRIORITY_LOWEST);

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);


    APP_ERROR_CHECK(err_code);
}


void SFM10R1_begin() {
    //_serial.begin(9600);

    //Remove un-ended commands from TST's buffer
    app_uart_put((uint8_t)'\0');
    app_uart_put((uint8_t)';');

    //Wait for the "KO;"
    //while(_serial.available() < 3);

    app_uart_get(&dummy); //'K'
    app_uart_get(&dummy); //'O'
    app_uart_get(&dummy); //';'
    _lastSend = -1;
}


bool SFM10R1_isReady() {
    /*unsigned long currentTime = millis();
    if(currentTime >= _lastSend && (currentTime - _lastSend) <= 600000) {
        return false;
    }

    // Time is ok, ask the modem's status
    app_uart_put((uint8_t)'\0');
    app_uart_put((uint8_t)'S');
    app_uart_put((uint8_t)'F');
    app_uart_put((uint8_t)'P');
    app_uart_put((uint8_t)';');

    return SFM10R1_nextReturn() == OK;*/

    //app_uart_put((uint8_t)'\0');
    app_uart_put((uint8_t)'A');   
    //nrf_delay_ms(2);
    app_uart_put((uint8_t)'T');
    app_uart_put((uint8_t)'\r');

    uint8_t response[8] = {0};
    uint8_t i = 0;
    //while(!_serial.available());
    //while(_serial.peek() != ';') {
    for (i=0; i==2; i++) {
        response[i] = app_uart_get(&dummy);
        //while(!_serial.available());
        ++i;
    }
    if ((response[0]=='O') && (response[1]=='K'))
        return OK;
    else return KO;
}

void SFM10R1_send_test(){
    app_uart_put((uint8_t)'A');
    app_uart_put((uint8_t)'T');
    app_uart_put((uint8_t)'$');
    app_uart_put((uint8_t)'S');
    app_uart_put((uint8_t)'F');
    app_uart_put((uint8_t)'=');
    app_uart_put((uint8_t)'6');
    app_uart_put((uint8_t)'9');
    app_uart_put((uint8_t)'\r');
}

bool SFM10R1_send(const void* data, uint8_t len) {
	uint8_t* bytes = (uint8_t*)data;

    /*if(!isReady()) {
        return false;
    }*/

    // See comment in isReady()
    //_lastSend = millis();

    //app_uart_put((uint8_t)'\0');
    app_uart_put((uint8_t)'A');
    app_uart_put((uint8_t)'T');
    app_uart_put((uint8_t)'$');
    app_uart_put((uint8_t)'S');
    app_uart_put((uint8_t)'F');
    app_uart_put((uint8_t)'=');
    for(uint8_t i = 0; i < len; ++i) {
        app_uart_put(bytes[i]);
    }
    app_uart_put(';');

    uint8_t ok = SFM10R1_nextReturn();
    if(ok == OK) {
        SFM10R1_nextReturn(); //SENT
        return true;
    }
    return false;
}

unsigned long SFM10R1_getPac() {
    //app_uart_put((uint8_t)'\0');
    app_uart_put((uint8_t)'A');
    app_uart_put((uint8_t)'T');
    app_uart_put((uint8_t)'$');
    app_uart_put((uint8_t)'I');
    app_uart_put((uint8_t)'=');
    app_uart_put((uint8_t)'1');
    app_uart_put((uint8_t)'1');
    app_uart_put((uint8_t)';');
    app_uart_put((uint8_t)'\r');
    /*uint8_t * tx_data = (uint8_t *)("AT$I=11;");

    // Start sending one byte and see if you get the same
    for (uint32_t i = 0; i < MAX_TEST_DATA_BYTES; i++)
    {
        while (app_uart_put(tx_data[i]) != NRF_SUCCESS);

        nrf_delay_ms(2);
    }*/

    //Response is [byte1, byte2, ..., byteN, 'O', 'K']
    /*uint8_t response[8] = {0};
    uint8_t i = 0;
    //while(!_serial.available());
    //while(_serial.peek() != ';') {
    for (int i=0; i==2; i++) {
        response[i] = app_uart_get(&dummy);
        //while(!_serial.available());
        ++i;
    }
    app_uart_get(&dummy); //';'


    for(uint8_t j = 0; j < i-2; ++j) {
        id += response[j] << ((i-3-j) * 16);
    }*/

    unsigned long id = 20;
    return id;
}

unsigned long SFM10R1_getID() {
    //app_uart_put((uint8_t)'\0');
    app_uart_put((uint8_t)'A');
    app_uart_put((uint8_t)'T');
    app_uart_put((uint8_t)'$');
    app_uart_put((uint8_t)'I');
    app_uart_put((uint8_t)'=');
    app_uart_put((uint8_t)'1');
    app_uart_put((uint8_t)'0');
    app_uart_put((uint8_t)';');
    app_uart_put((uint8_t)'\r');

    //Response is [byte1, byte2, ..., byteN, 'O', 'K']
    uint8_t response[8] = {0};
    uint8_t i = 0;
    //while(!_serial.available());
    //while(_serial.peek() != ';') {

    for (int i=0; i==2; i++) {
        response[i] = app_uart_get(&dummy);
        //while(!_serial.available());
        ++i;
    }
    app_uart_get(&dummy); //';'

    unsigned long id = 0;

    for(uint8_t j = 0; j < i-2; ++j) {
        id += response[j] << ((i-3-j) * 8);
    }

    return id;
}

//Power value:
//0 -25 -30 dBm
//1 0dBm
//2 14dBm
//3 16dBm
//4 18dBm
//5 Max (18-19dBm)
bool SFM10R1_setPower(uint8_t power) {
    power = power % 6; //It's 0-5
    app_uart_put((uint8_t)'\0');
    app_uart_put((uint8_t)'S');
    app_uart_put((uint8_t)'F');
    app_uart_put((uint8_t)'G');
    app_uart_put(power);
    app_uart_put((uint8_t)';');
    app_uart_put((uint8_t)'\r');

    return SFM10R1_nextReturn() == OK;
}

uint8_t SFM10R1_nextReturn() {
    //while(!_serial.available());
    char fstChar = app_uart_get(&dummy);
    while(app_uart_get(&dummy) != ';');
    return fstChar;
}
