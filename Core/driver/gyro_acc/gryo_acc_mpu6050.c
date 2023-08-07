/**
 * @file gryo_acc_mpu6050.c
 * @brief MPU6050 driver source file
 * @author Théo Magne
 * @date 30/07/2023
 * @see gryo_acc_mpu6050.h
 */

/* ************************************* Includes *********************************************** */
#include "bus.h"
#include "gyro_acc.h"
#include "gryo_acc_mpu6050.h"


/* ************************************* Private macros ***************************************** */
/* Default I2C address */
#define MPU6050_I2C_ADDR			0xD0

/* MPU6050 registers */
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75
#define MPU6050_READ				0x80

/* Gyroscope sensitivities in °/s */
#define MPU6050_GYRO_SENS_250		((float) 131)
#define MPU6050_GYRO_SENS_500		((float) 65.5)
#define MPU6050_GYRO_SENS_1000		((float) 32.8)
#define MPU6050_GYRO_SENS_2000		((float) 16.4)

/* Accelerometer sensitivities in g */
#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)

/* ************************************* Private type definition ******************************** */

/**
 * @brief Accelerometer range enumeration definition
 */
typedef enum
{
	MPU_ACC_2G = 0,
	MPU_ACC_4G,
	MPU_ACC_8G,
	MPU_ACC_16G
} mpu6050_acc_range_t;

/**
 * @brief Gyroscope range enumeration definition
 */
typedef enum
{
	MPU_GYRO_250s = 0,
	MPU_GYRO_500s,
	MPU_GYRO_1000s,
	MPU_GYRO_2000s
} mpu6050_gyro_range_t;


/* ************************************* Private functions prototypes *************************** */
static int mpu6050_init(gyro_acc_t* gyro_acc, gyro_acc_t_config* config, bus_t* bus);

/* ************************************* Private variables ************************************** */
const static float mpu5050_gyro_data_to_rad_s[] = {
    (1 / MPU6050_GYRO_SENS_250) * M_PI / 180.0f,
    (1 / MPU6050_GYRO_SENS_500) * M_PI / 180.0f,
    (1 / MPU6050_GYRO_SENS_1000) * M_PI / 180.0f,
    (1 / MPU6050_GYRO_SENS_2000) * M_PI / 180.0f
};

const static float mpu5050_acc_data_to_g[] = {
    (1 / MPU6050_ACCE_SENS_2),
    (1 / MPU6050_ACCE_SENS_4),
    (1 / MPU6050_ACCE_SENS_8),
    (1 / MPU6050_ACCE_SENS_16)
};

/* ************************************* Public variables *************************************** */

/* ************************************* Private functions ************************************** */
static mpu6050_acc_range_t mpu6050_get_acc_range_from_config(gyro_range_t* acc_sensi)
{
    mpu6050_acc_range_t range = MPU_ACC_2G;
    switch(acc_sensi->sensi)
    {
        case ACC_RANGE_LOW:
            range = MPU_ACC_4G;
            break;
        case ACC_RANGE_MEDIUM:
            range = MPU_ACC_8G;
            break;
        case ACC_RANGE_HIGH:
            range = MPU_ACC_16G;
            break;
        default:
            break;
    }
    return range;
}

static mpu6050_gyro_range_t mpu6050_get_gyro_range_from_config(gyro_range_t* gyro_sensi)
{
    mpu6050_gyro_range_t range = MPU_GYRO_250s;
    switch(gyro_sensi->sensi)
    {
        case GYRO_RANGE_LOW:
            range = MPU_GYRO_500s;
            break;
        case GYRO_RANGE_MEDIUM:
            range = MPU_GYRO_1000s;
            break;
        case GYRO_RANGE_HIGH:
            range = MPU_GYRO_2000s;
            break;
        default:
            break;
    }
    return range;
}

static int mpu6050_init(gyro_acc_t* gyro_acc, bus_t* bus, gyro_acc_config_t* config)
{
    int ret = 0;
    uint8_t data = 0;
    /* Configure the gyro_acc structure */
    gyro_acc->device.addr = MPU6050_I2C_ADDR;
    gyro_acc->device.bus = bus;
    gyro_acc->config = *config;
    mpu6050_acc_range_t acc_mpu_range = mpu6050_get_acc_range_from_config(&config->acc_sensi);
    mpu6050_gyro_range_t gyro_mpu_range = mpu6050_get_gyro_range_from_config(&config->gyro_sensi);
    gyro_acc->acc_convertion = mpu5050_acc_data_to_g[acc_mpu_range];
    gyro_acc->gyro_convertion = mpu5050_gyro_data_to_rad_s[gyro_mpu_range];

    /* Configure the MPU6050 */
    /* Check if the mpu answers */
    ret = bus->driver->is_ready(&gyro_acc->device);
    if (ret == 0)
    {
        /* Wake up the mpu */
        mem_info_t mem_info =
        {
            .addr = MPU6050_PWR_MGMT_1,
            .data = &data,
            .size = 1
        };
        data = 0x80;
        ret = bus->driver->mem_write(&gyro_acc->device, &mem_info);
    }

    if (ret == 0)
    {
        /* Set the clock to use */
        mem_info_t mem_info =
        {
            .addr = MPU6050_PWR_MGMT_1,
            .data = &data,
            .size = 1
        };
        data = 0x03;
        ret = bus->driver->mem_write(&gyro_acc->device, &mem_info);
    }

    if (ret == 0)
    {
        /* Configure the gyro range */
        mem_info_t mem_info =
        {
            .addr = MPU6050_GYRO_CONFIG,
            .data = &data,
            .size = 1
        };
        ret = bus->driver->mem_read(&gyro_acc->device, &mem_info);
        if (ret == 0)
        {
            data &= 0xE7; // Clear self-test bits [7:5]
            data |= (gyro_mpu_range << 3);
            ret = bus->driver->mem_write(&gyro_acc->device, &mem_info);
        }
    }

    if (ret == 0)
    {
        /* Configure the acc range */
        mem_info_t mem_info =
        {
            .addr = MPU6050_ACCEL_CONFIG,
            .data = &data,
            .size = 1
        };
        ret = bus->driver->mem_read(&gyro_acc->device, &mem_info);
        if (ret == 0)
        {
            data &= 0xE7; // Clear self-test bits [7:5]
            data |= (acc_mpu_range << 3);
            ret = bus->driver->mem_write(&gyro_acc->device, &mem_info);
        }
    }

    return ret;
}
/* ************************************* Public functions *************************************** */

/* ************************************* Public callback functions ****************************** */