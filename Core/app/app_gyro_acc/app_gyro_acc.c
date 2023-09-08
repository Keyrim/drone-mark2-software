/**
 * @file app_gyro_acc.c
 * @brief This applications test the gyroscope and accelerometer
 * @author Théo Magne
 * @date 08/09/2023
 * @see app.h
 */

/* ************************************* Includes *********************************************** */
#include "app.h"
#include "gyro_acc.h"
#include "bus.h"

/* ************************************* Private macros ***************************************** */

/* ************************************* Private type definition ******************************** */

/* ************************************* Private functions prototypes *************************** */

/* ************************************* Private variables ************************************** */
static gyro_acc_t gyro_acc = { 0 };

/* ************************************* Public variables *************************************** */

/* ************************************* Private functions ************************************** */

/* ************************************* Public functions *************************************** */
void app_init()
{
    int result = 0;

    if (result >= 0)
    {
        result = bus_i2c_1.driver->init(&bus_i2c_1);
    }

    if (result >= 0)
    {
        result = gyro_acc_init(&gyro_acc, GYRO_ACC_MPU6050, &bus_i2c_1);
    }
}


void app_run()
{

}
/* ************************************* Public callback functions ****************************** */
