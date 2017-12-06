/**
@addtogroup LIS2DS12Driver LIS2DS12 Acceleration Sensor Driver
@{
@file       LIS2DS12.c

Implementation of the LIS2DS12 driver.

For a detailed description see the detailed description in @ref LIS2DS12.h

* @}
***************************************************************************************************/

/* INCLUDES ***************************************************************************************/
#include "LIS2DS12.h"
#include "LIS2DS12_registers.h"
#include "spi.h"
#include "nrf_drv_gpiote.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf.h"
#include "app_timer.h"
#include "bsp.h"
#include "boards.h"


#include <string.h>

/* CONSTANTS **************************************************************************************/
/** Maximum Size of SPI Addresses */
#define ADR_MAX 0x3FU

/** Number of maximum SPI Transfer retries */
#define RETRY_MAX 3U

/** Size of raw sensor data for all 3 axis */
#define SENSOR_DATA_SIZE 6U

/** Max number of registers to read at once. To read all axis at once, 6bytes are neccessary */
#define READ_MAX SENSOR_DATA_SIZE

/** Bit Mask to set read bit for SPI Transfer */
#define SPI_READ 0x80U

/** Bit Mask to enable auto address incrementation for multi read */
#define SPI_ADR_INC 0x40U

/* MACROS *****************************************************************************************/

APP_TIMER_DEF(LIS2DS12_timer_id);                           /** Creates timer id for our program **/
#define SCHED_MAX_EVENT_DATA_SIZE       MAX(APP_TIMER_SCHED_EVT_SIZE, sizeof(nrf_drv_gpiote_pin_t))
#define SCHED_QUEUE_SIZE                10

/* TYPES ******************************************************************************************/
/** Structure containing sensor data for all 3 axis */
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
} acceleration_t;

/** Union to split raw data to values for each axis */
typedef union
{
    uint8_t raw[SENSOR_DATA_SIZE];
    acceleration_t sensor;
} sensor_buffer_t;

/* PROTOTYPES *************************************************************************************/

static LIS2DS12_Ret selftest(void);

LIS2DS12_Ret readRegister(uint8_t address, uint8_t* const p_toRead, uint8_t count);

static LIS2DS12_Ret writeRegister(uint8_t address, uint8_t dataToWrite);

void timer_LIS2DS12_event_handler(void* p_context);


/* VARIABLES **************************************************************************************/
static LIS2DS12_drdy_event_t g_fp_drdyCb = NULL;        /**< Data Ready Callback */
static sensor_buffer_t g_sensorData;                    /**< Union to covert raw data to value for each axis */
static LIS2DS12_PowerMode g_powerMode = LIS2DS12_POWER_DOWN; /**< Current power mode */
static LIS2DS12_Scale g_scale = LIS2DS12_SCALE2G;       /**< Selected scale */
static uint8_t g_mgpb = 1;                              /**< milli-g per bit */
static uint8_t g_resolution = 10;                        /**< milli-g nb of bits */
static bool g_drdy = false;                             /**< Data Ready flag */

/* EXTERNAL FUNCTIONS *****************************************************************************/

extern LIS2DS12_Ret LIS2DS12_init(LIS2DS12_PowerMode powerMode, LIS2DS12_Scale scale, LIS2DS12_drdy_event_t drdyCB)
{
    LIS2DS12_Ret retVal = LIS2DS12_RET_OK;
    uint32_t err_code = 0;
    

    /* Remember Callback. Note: NULL Pointer check not necessary, callback is optional */
    g_fp_drdyCb = drdyCB;

    /* Initialize SPI */
    if (false == spi_isInitialized())
    {
        spi_init();
    }
    
    // Initialize the LIS2DS12 timer module.
    // Requires the low-frequency clock initialized
    // Create timer
    err_code = app_timer_create(&LIS2DS12_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                 timer_LIS2DS12_event_handler);
    APP_ERROR_CHECK(err_code);
    //Timer is started when power mode is set

    // Start Selftest
    retVal |= selftest();

    if (LIS2DS12_RET_OK == retVal)
    {
        /* Set Power Mode */
        retVal |= LIS2DS12_setPowerMode(powerMode);

        /* save Scale, Scale is set together with resolution in setPowerMode (CRTL4) */
        g_scale = scale;
    }

    return retVal;
}

extern LIS2DS12_Ret LIS2DS12_setPowerMode(LIS2DS12_PowerMode powerMode)
{
    LIS2DS12_Ret retVal = LIS2DS12_RET_OK;
    uint8_t ctrl1RegVal = 0;
    uint8_t ctrl4RegVal = 0;
    uint32_t time_ms = 0;
    uint32_t err_code; 

    /* reset data ready flag, after changing power mode it needs some time till new data is available */
    g_drdy = false;

    /* Set Scale */
    ctrl4RegVal = ((uint8_t)g_scale)<<4U;
    /*Enable all axis */
    ctrl1RegVal = LIS2DS_XYZ_EN_MASK;

    switch(powerMode)
    {
    case LIS2DS12_POWER_NORMAL:
        ctrl1RegVal |= LIS2DS_ODR_MASK_100HZ;
        g_mgpb = 4 << g_scale; //4mg per bits at normal power/2g, adjust by scaling
        g_resolution = 10;
        time_ms = 10U;
        break;
    case LIS2DS12_POWER_LOW:
        ctrl1RegVal |= (LIS2DS_ODR_MASK_1HZ); //Power consumption is same for low-power and normal mode at 1 Hz
        g_mgpb = 4 << g_scale; //4 bits per mg at normal power/2g, adjust by scaling
        g_resolution = 10;
        time_ms = 1000U;

        break;
    case LIS2DS12_POWER_FAST:
        ctrl1RegVal |= (LIS2DS_ODR_MASK_1620HZ | LIS2DS_LPEN_MASK);
        g_mgpb = 16 << g_scale; //16 mg per bit at low power/2g, adjust by scaling
        g_resolution = 8;
        time_ms = 1;
        break;
    case LIS2DS12_POWER_HIGHRES:
        ctrl1RegVal |= LIS2DS_ODR_MASK_HIGH_RES;
        ctrl4RegVal |= LIS2DS_HR_MASK;
        g_mgpb = 1 << g_scale; //1 mg bits per mg at high power/2g, adjust by scaling
        g_resolution = 12;
        time_ms = 1;
        break;
    case LIS2DS12_POWER_DOWN:
        ctrl1RegVal = 0;
        time_ms = 0;
        break;
    default:
        retVal = LIS2DS12_RET_ERROR;
    }

    if (LIS2DS12_RET_OK == retVal)
    {
        retVal = writeRegister(LIS2DS_CTRL_REG1, ctrl1RegVal);
        retVal |= writeRegister(LIS2DS_CTRL_REG4, ctrl4RegVal);
    }

    /* save power mode to check in get functions if power is enabled */
    g_powerMode = powerMode;

    if (time_ms > 0)
    {
        /* start sample timer with sample time according to selected sample frequency */
        err_code = app_timer_start(LIS2DS12_timer_id, APP_TIMER_TICKS(time_ms), NULL); //time_ms, 0=APP_TIMER_PRESCALER
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        err_code = app_timer_stop(LIS2DS12_timer_id);
        APP_ERROR_CHECK(err_code);
    }

    return retVal;
}

extern LIS2DS12_Ret LIS2DS12_getXmG(int32_t* const accX)
{
    LIS2DS12_Ret retVal = LIS2DS12_RET_OK;

    if (NULL == accX)
    {
        retVal = LIS2DS12_RET_NULL;
    }
    else
    {
        //Scale value, note: values from accelerometer are 16-bit 2's complement left-justified in all cases. "Extra" LSBs will be noise 
        //Add 32768 (1<<(16-1) to get positive, shift, substract (1<<(resolution-1), scale voila!
        *accX = (((32768+g_sensorData.sensor.x)>>(16-g_resolution))-(1<<(g_resolution-1)))*g_mgpb ;
        //*accX = g_sensorData.sensor.x / (16 << (g_scale)) * 1000 / 1024;
    }

    return retVal;
}

extern LIS2DS12_Ret LIS2DS12_getYmG(int32_t* const accY)
{
    LIS2DS12_Ret retVal = LIS2DS12_RET_OK;

    if (NULL == accY)
    {
        retVal = LIS2DS12_RET_NULL;
    }
    else
    {
        //Scale value, note: values from accelerometer are 16-bit 2's complement left-justified in all cases. "Extra" LSBs will be noise 
        //Add 32768 (1<<(16-1) to get positive, shift, substract (1<<(resolution-1), scale voila!
        *accY = (((32768+g_sensorData.sensor.y)>>(16-g_resolution))-(1<<(g_resolution-1)))*g_mgpb ;
        //*accY = g_sensorData.sensor.y / (16 << (g_scale)) * 1000 / 1024;
    }

    return retVal;
}

extern LIS2DS12_Ret LIS2DS12_getZmG(int32_t* const accZ)
{
    LIS2DS12_Ret retVal = LIS2DS12_RET_OK;
    
    if (NULL == accZ)
    {
        retVal = LIS2DS12_RET_NULL;
    }
    else
    {
        //Scale value, note: values from accelerometer are 16-bit 2's complement left-justified in all cases. "Extra" LSBs will be noise 
        //Add 32768 (1<<(16-1) to get positive, shift, substract (1<<(resolution-1), scale voila!
        *accZ = (((32768+g_sensorData.sensor.z)>>(16-g_resolution))-(1<<(g_resolution-1)))*g_mgpb ;
        //*accZ = g_sensorData.sensor.z / (16 << (g_scale)) * 1000 / 1024;
    }

    return retVal;
}

extern LIS2DS12_Ret LIS2DS12_getALLmG(int32_t* const accX, int32_t* const accY, int32_t* const accZ)
{
    LIS2DS12_Ret retVal = LIS2DS12_RET_OK;

    if ((NULL == accX) || (NULL == accY) || (NULL == accZ))
    {
        retVal = LIS2DS12_RET_NULL;
    }
    else
    {
        LIS2DS12_getXmG(accX);
        LIS2DS12_getYmG(accY);
        LIS2DS12_getZmG(accZ);
    }

    return retVal;
}

/* INTERNAL FUNCTIONS *****************************************************************************/

/**
 * Execute LIS2DS12 Selftest
 *
 * @return LIS2DS12_RET_OK Selftest passed
 * @return LIS2DS12_RET_ERROR_SELFTEST Selftest failed
 */
static LIS2DS12_Ret selftest(void)
{
    uint8_t value[1] = {0};
    readRegister(LIS2DS_WHO_AM_I, value, 1);
    return (LIS2DS_I_AM_MASK == value[0]) ? LIS2DS12_RET_OK : LIS2DS12_RET_ERROR;
}

/**
 * Read registers
 *
 * Read one or more registers from the sensor
 *
 * @param[in] address Start address to read from
 * @param[out] p_toRead Pointer to result buffer
 * @param[in] cound Number of bytes to read
 *
 * @return LIS2DS12_RET_OK No Error
 * @return LIS2DS12_RET_NULL Result buffer is NULL Pointer
 * @return LIS2DS12_RET_ERROR Read attempt was not successful
 */
LIS2DS12_Ret readRegister(uint8_t address, uint8_t* const p_toRead, uint8_t count)
{
    NRF_LOG_DEBUG("LIS2DS12 Register read started'\r\n");
    LIS2DS12_Ret retVal = LIS2DS12_RET_ERROR;
    SPI_Ret retValSpi = SPI_RET_ERROR;
    uint8_t writeBuf[READ_MAX + 1U] = {0}; /* Bytes to read + 1 for address */
    uint8_t readBuf[READ_MAX + 1U] = {0};  /* Bytes to read + 1 for address */
    uint8_t ii = 0; /* retry counter */

    if (NULL == p_toRead)
    {
        retVal = LIS2DS12_RET_NULL;
    }
    else if (count > READ_MAX)
    {
        retVal = LIS2DS12_RET_ERROR;
    }
    else
    {
        do
        {
        writeBuf[0] = address | SPI_READ | SPI_ADR_INC;

        retValSpi = spi_transfer_LIS2DS12(writeBuf, (count + 1U), readBuf);
        ii++;
        }
        while ((SPI_RET_BUSY == retValSpi) && (ii < RETRY_MAX)); /* Retry if SPI is busy */


        if (SPI_RET_OK == retValSpi)
        {
            retVal = LIS2DS12_RET_OK;
            /* Transfer was ok, copy result */
            memcpy(p_toRead, readBuf + 1U, count);
        }
        else
        {
            retVal = LIS2DS12_RET_ERROR;
        }
    }
    NRF_LOG_DEBUG("LIS2DS12 Register read complete'\r\n");
    return retVal;
}

/**
 * Write a register
 *
 * @param[in] address Register address to write, address is 5bit, so max value is 0x1F
 * @param[in] dataToWrite Data to write to register
 *
 * @return LIS2DS12_RET_OK No Error
 * @return LIS2DS12_RET_ERROR Address is lager than allowed
 */
static LIS2DS12_Ret writeRegister(uint8_t address, uint8_t dataToWrite)
{
    LIS2DS12_Ret retVal = LIS2DS12_RET_ERROR;
    SPI_Ret retValSpi = SPI_RET_ERROR;
    uint8_t to_read[2] = {0U}; /* dummy, not used for writing */
    uint8_t to_write[2] = {0U};
    uint8_t ii = 0; /* retry counter */

    /* SPI Addresses are 5bit only */
    if (address <= ADR_MAX)
    {
        to_write[0] = address;
        to_write[1] = dataToWrite;

        do
        {
            retValSpi = spi_transfer_LIS2DS12(to_write, 2, to_read);
            ii++;
        }
        while ((SPI_RET_BUSY == retValSpi) && (ii < RETRY_MAX)); /* Retry if SPI is busy */

        if (SPI_RET_OK == retValSpi)
        {
            retVal = LIS2DS12_RET_OK;
        }
        else
        {
            retVal = LIS2DS12_RET_ERROR;
        }
    }
    else
    {
        retVal = LIS2DS12_RET_ERROR;
    }

    return retVal;
}

/**
 * Event Handler that is called by the timer to read the sensor values.
 *
 * This is a workaround because data ready interrupt from LIS2DS12 is not working
 *
 * @param [in] pContext Timer Context
 */
void timer_LIS2DS12_event_handler(void* p_context)
{
    NRF_LOG_DEBUG("LIS2DS12 Timer event'\r\n");
    //nrf_gpio_pin_toggle(19);

    if (LIS2DS12_RET_OK == readRegister(LIS2DS_OUT_X_L, g_sensorData.raw, SENSOR_DATA_SIZE))
    {
        /* if read was successfull set data ready */
        g_drdy = true;

        /* call data ready event callback if registered */
        if (NULL != g_fp_drdyCb)
        {
            g_fp_drdyCb();
        }
    }
}
