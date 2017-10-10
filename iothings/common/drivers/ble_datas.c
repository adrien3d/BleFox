#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "ble_datas.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "SFM10R1.h"
//#include "led.h"
//#include "SEGGER_RTT.h"

//int32_t taste = 0;

/**@brief Function for handling BLE GATTS EVENTS
 * 
 * This function prints out data that is received when you try to write to your characteristic or CCCD. 
 * In general it is a bad idea to to so much printf stuff and UART transfer inside the BLE handler,
 * but this is just for demonstrate purposes.
 *
 * @param[in]   p_our_service        Our Service structure.
 * @param[in]   p_ble_evt            BLE event passed from BLE stack
 *
 */
static void on_ble_write(ble_datas_t * p_our_service, ble_evt_t * p_ble_evt)
{
    // Declare buffer variable to hold received data. The data can only be 32 bit long.
    uint32_t data_buffer;
    // Pupulate ble_gatts_value_t structure to hold received data and metadata.
    ble_gatts_value_t rx_data;
    rx_data.len = sizeof(uint32_t);
    rx_data.offset = 0;
    rx_data.p_value = (uint8_t*)&data_buffer;
    
    // Check if write event is performed on our characteristic or the CCCD
    if(p_ble_evt->evt.gatts_evt.params.write.handle == p_our_service->char_handles.value_handle)
    {
        // Get data
        sd_ble_gatts_value_get(p_our_service->conn_handle, p_our_service->char_handles.value_handle, &rx_data);
        // Print handle and value 
        //printf("Value received on handle %#06x: %#010x\r\n", p_ble_evt->evt.gatts_evt.params.write.handle, data_buffer);

        //data1_characteristic_update(p_our_service, &taste);

        uint32_t new_attr_val = 0;


        switch(*rx_data.p_value)
        {
            case PERIPHERAL_GET_IMU_POSITION://21
                new_attr_val=3752;
                data1_characteristic_update(p_our_service, &new_attr_val);
            break;
            case PERIPHERAL_GET_UWB_HEART_RATE:
                data1_characteristic_update(p_our_service, &data_buffer);
            break;
            case PERIPHERAL_GET_UWB_RESPIRATORY_RATE:
                data1_characteristic_update(p_our_service, &data_buffer);
                //nrf_gpio_pin_toggle(LED_BLUE);
            break;
        }

        //switch ()
    }

    else if(p_ble_evt->evt.gatts_evt.params.write.handle == p_our_service->char_handles.cccd_handle)
    {
        // Get data
        sd_ble_gatts_value_get(p_our_service->conn_handle, p_our_service->char_handles.cccd_handle, &rx_data);
        // Print handle and value 
        //printf("Value received on handle %#06x: %#06x\r\n", p_ble_evt->evt.gatts_evt.params.write.handle, data_buffer);
        if(data_buffer == 0x0001)
        {
            printf("Notification enabled\r\n");
        }
        else if(data_buffer == 0x0000)
        {
            printf("Notification disabled\r\n");
        }
    }
    //taste ++;
}

static void on_ble_connected(void){
    /*colors_t blue = COLOR_BLUE;

    led_set_color_struct(blue);*/
}

static void on_ble_disconnected(void){
    /*colors_t red = COLOR_RED;

    led_set_color_struct(red);
    nrf_delay_ms(1000);
    stop_led();*/
}

// ALREADY_DONE_FOR_YOU: Declaration of a function that will take care of some housekeeping of ble connections related to our service and characteristic
void ble_datas_on_ble_evt(ble_datas_t * p_our_service, ble_evt_t * p_ble_evt)
{
    // OUR_JOB: Step 3.D Implement switch case handling BLE events related to our service. 
    switch (p_ble_evt->header.evt_id)
    {        
        case BLE_GATTS_EVT_WRITE:
            SFM10R1_getPac();
            nrf_delay_ms(50);
            on_ble_write(p_our_service, p_ble_evt);
            break;
        case BLE_GAP_EVT_CONNECTED:
            on_ble_connected();
            p_our_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_ble_disconnected();
            p_our_service->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for adding our new characterstic to "Our service" that we initiated in the previous tutorial. 
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
static uint32_t datas_add(ble_datas_t * p_our_service)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    // OUR_JOB: Step 2.A, Add a custom characteristic UUID
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = BLE_UUID_DATAS_BASE_UUID;
    char_uuid.uuid      = BLE_UUID_DATA_CHARACTERISTC_UUID;
    sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
    
    // OUR_JOB: Step 2.F Add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    
    // OUR_JOB: Step 3.A, Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
   
    
    // OUR_JOB: Step 2.B, Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md)); 
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;   
    
    
    // OUR_JOB: Step 2.G, Set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    
    // OUR_JOB: Step 2.C, Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));        
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // OUR_JOB: Step 2.H, Set characteristic length in number of bytes
    /*attr_char_value.max_len     = 4;
    attr_char_value.init_len    = 4;
    uint8_t value[4]            = {0x00,0x00,0x00,0x00};
    attr_char_value.p_value     = value;*/
    attr_char_value.max_len     = 20;
    attr_char_value.init_len    = 6;
    uint8_t value[20]            = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    attr_char_value.p_value     = value;

    // OUR_JOB: Step 2.E, Add our new characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_our_service->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_our_service->char_handles);
    APP_ERROR_CHECK(err_code);
    
    printf("\r\nService handle: %#x\n\r", p_our_service->service_handle);
    printf("Char value handle: %#x\r\n", p_our_service->char_handles.value_handle);
    printf("Char cccd handle: %#x\r\n\r\n", p_our_service->char_handles.cccd_handle);

    return NRF_SUCCESS;
}


/**@brief Function for initiating our new service.
 *
 * @param[in]   p_ble_datas        Our Service structure.
 *
 */
void ble_datas_init(ble_datas_t * p_ble_datas)
{
    uint32_t   err_code; // Variable to hold return codes from library and softdevice functions

    // OUR_JOB: Declare 16-bit service and 128-bit base UUIDs and add them to the BLE stack
    ble_uuid_t        service_uuid;
    ble_uuid128_t     base_uuid = BLE_UUID_DATAS_BASE_UUID;
    service_uuid.uuid = BLE_UUID_DATAS_SERVICE;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);    

    p_ble_datas->conn_handle = BLE_CONN_HANDLE_INVALID;

    // OUR_JOB: Add our service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_ble_datas->service_handle);
    APP_ERROR_CHECK(err_code);

    datas_add(p_ble_datas);

    // Print messages to Segger Real Time Terminal
    // UNCOMMENT THE FOUR LINES BELOW AFTER INITIALIZING THE SERVICE OR THE EXAMPLE WILL NOT COMPILE.
//    SEGGER_RTT_WriteString(0, "Executing our_service_init().\n"); // Print message to RTT to the application flow
//    SEGGER_RTT_printf(0, "Service UUID: 0x%#04x\n", service_uuid.uuid); // Print service UUID should match definition BLE_UUID_DATAS_SERVICE
//    SEGGER_RTT_printf(0, "Service UUID type: 0x%#02x\n", service_uuid.type); // Print UUID type. Should match BLE_UUID_TYPE_VENDOR_BEGIN. Search for BLE_UUID_TYPES in ble_types.h for more info
//    SEGGER_RTT_printf(0, "Service handle: 0x%#04x\n", p_ble_datas->service_handle); // Print out the service handle. Should match service handle shown in MCP under Attribute values
}

void data1_characteristic_update(ble_datas_t *p_our_service, uint32_t *new_value)
{
    // OUR_JOB: Step 3.E, Update characteristic value
    if (p_our_service->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t               len = 20;
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_our_service->char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len  = &len;
        hvx_params.p_data = (uint8_t*)new_value;  

        sd_ble_gatts_hvx(p_our_service->conn_handle, &hvx_params);
    }   
}