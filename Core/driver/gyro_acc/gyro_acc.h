/**
 * @file gyro_acc.h
 * @brief Interface for the gyro_acc driver.
 * @author Théo Magne
 * @date 25/07/2023
 * @see gyro_acc.c
 */

#ifndef _GYRO_ACC_H_
#define _GYRO_ACC_H_

/* ************************************* Includes *********************************************** */
#include <stdint.h>
#include "vector.h"
#include "bus.h"

/* ************************************* Public macros ****************************************** */

/* ************************************* Public type definition ********************************* */
typedef enum
{
    GYRO_ACC_MPU6050 = 0,
    GYRO_ACC_ERROR
}gyro_acc_type_t;

typedef enum
{
    GYRO_RANGE_LOW = 0,
    GYRO_RANGE_MEDIUM,
    GYRO_RANGE_HIGH
}gyro_range_t;

typedef enum
{
    ACC_RANGE_LOW = 0,
    ACC_RANGE_MEDIUM,
    ACC_RANGE_HIGH
}acc_range_t;

typedef struct
{
    gyro_range_t gyro;
    acc_range_t acc;
} gyro_acc_config_t;

typedef void (*gyro_acc_read_cb_t)(void);
typedef struct gyro_acc_t gyro_acc_t;


typedef int (*gyro_acc_init_t)(gyro_acc_t *gyro_acc, bus_t *bus);
typedef int (*gyro_acc_read_all_t)(gyro_acc_t *gyro_acc);
typedef int (*gyro_acc_read_gyro_t)(gyro_acc_t *gyro_acc);
typedef int (*gyro_acc_read_acc_t)(gyro_acc_t *gyro_acc);
typedef int (*gyro_acc_read_all_async_t)(gyro_acc_t *gyro_acc);
typedef int (*gyro_acc_read_gyro_async_t)(gyro_acc_t *gyro_acc);
typedef int (*gyro_acc_read_acc_async_t)(gyro_acc_t *gyro_acc);

/**
 * @brief Structure containing all the function pointers for the gyro_acc driver.
 */
typedef struct gyro_acc_driver_t
{
    gyro_acc_init_t init;
    gyro_acc_read_all_t read_all;
    gyro_acc_read_gyro_t read_gyro;
    gyro_acc_read_acc_t read_acc;
    gyro_acc_read_all_async_t read_all_async;
    gyro_acc_read_gyro_async_t read_gyro_async;
    gyro_acc_read_acc_async_t read_acc_async;
}gyro_acc_driver_t;

/**
 * @brief Structure containing all the data needed for the gyro_acc driver.
 */
typedef struct gyro_acc_t
{
    struct config_t {
        gyro_range_t gyro;
        acc_range_t acc;
    } config;
    struct data_t {
        vector3_t gyro;
        vector3_t acc;
    } data;
    struct private_t {
        dev_info_t device;
        gyro_acc_read_cb_t read_cb;
        float gyro_conv_factor;
        float acc_conv_factor;
    } private;
    gyro_acc_driver_t *driver;
}gyro_acc_t;

/* ************************************* Public variables *************************************** */

/* ************************************* Public functions *************************************** */
int gyro_acc_init(gyro_acc_t *gyro_acc, gyro_acc_type_t type, bus_t *bus);

/* ************************************* Public callback functions ****************************** */

#endif /* _GYRO_ACC_H_ */
