/**
 * @file gyro_acc.c
 * @brief Provide an init function to link a gyro_acc_type_t to a driver
 * @author Théo Magne
 * @date 08/09/2023
 * @see gyro_acc.h
 */

/* ************************************* Includes *********************************************** */
#include "gyro_acc.h"
#include "gyro_acc_mpu6050.h"

/* ************************************* Private macros ***************************************** */

/* ************************************* Private type definition ******************************** */

/* ************************************* Private functions prototypes *************************** */

/* ************************************* Private variables ************************************** */

/* ************************************* Public variables *************************************** */

/* ************************************* Private functions ************************************** */

/* ************************************* Public functions *************************************** */
int gyro_acc_init(gyro_acc_t *gyro_acc, gyro_acc_type_t type, bus_t *bus)
{
    int ret = -1;
    switch(type)
    {
        case GYRO_ACC_MPU6050:
            gyro_acc->driver = &gyro_acc_mpu6050_driver;
            ret = gyro_acc->driver->init(gyro_acc, bus);
            break;
        default:
            break;
    }
    return ret;
}

/* ************************************* Public callback functions ****************************** */