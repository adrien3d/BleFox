#ifndef DATAS_SERVICE_H__
#define DATAS_SERVICE_H__

#include <stdint.h>
#include "boards.h"
#include "ble.h"
#include "ble_srv_common.h"

typedef uint8_t datas_opcode_t;
typedef struct __attribute__((packed))
{
    datas_opcode_t opcode;
    uint8_t len;
    uint8_t data[18];
} datas_control_t;

//#define BLE_UUID_DATA_SERVICE            "ca4e5357-4cbb-11e6-beb8-9e71128cae77"

//#define BLE_UUID_DATAS_BASE_UUID              {0xCA, 0x4E, 0x53, 0x57, 0x4C, 0xBB, 0x11, 0xE6, 0xBE, 0xB8, 0x9E, 0x71, 0x12, 0x8C, 0xAE, 0x77} // 128-bit base UUID
#define BLE_UUID_DATAS_BASE_UUID              {{0x77, 0xAE, 0x8C, 0x12, 0x71, 0x9E, 0xB8, 0xBE, 0xE6, 0x11, 0xBB, 0x4C, 0x00, 0x00, 0x00, 0x00}} 
#define BLE_UUID_DATAS_SERVICE                0x5357 // Just a random, but recognizable value

#define BLE_UUID_DATA_CHARACTERISTC_UUID    0xC4A5 // Just a random, but recognizable value

/**
 * @brief This structure contains various status information for our service. 
 * It only holds one entry now, but will be populated with more items as we go.
 * The name is based on the naming convention used in Nordic's SDKs. 
 * 'ble’ indicates that it is a Bluetooth Low Energy relevant structure and 
 * ‘os’ is short for Our Service). 
 */
typedef struct
{
    uint16_t                    conn_handle;    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t                    service_handle;     /**< Handle of Our Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    char_handles;
} ble_datas_t;

/**@brief Function for handling BLE Stack events related to our service and characteristic.
 *
 * @details Handles all events from the BLE stack of interest to Our Service.
 *
 * @param[in]   p_our_service       Our Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_datas_on_ble_evt(ble_datas_t * p_our_service, ble_evt_t * p_ble_evt);

/**@brief Function for initializing our new service.
 *
 * @param[in]   p_ble_datas       Pointer to Our Service structure.
 */
void ble_datas_init(ble_datas_t * p_ble_datas);

/**@brief Function for updating and sending new characteristic values
 *
 * @details The application calls this function whenever our timer_timeout_handler triggers
 *
 * @param[in]   p_our_service                     Our Service structure.
 * @param[in]   characteristic_value     New characteristic value.
 */
void data1_characteristic_update(ble_datas_t *p_our_service, uint32_t *new_alue);

#endif /* _ DATAS_SERVICE_H__ */