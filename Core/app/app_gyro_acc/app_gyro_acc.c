/**
 * @file app_gyro_acc.c
 * @brief This applications test the gyroscope and accelerometer
 * @author Théo Magne
 * @date 08/09/2023
 * @see app.h
 */

/* ************************************* Includes *********************************************** */
#include "stm32f4xx_hal.h"
#include "app.h"
#include "gyro_acc.h"
#include "bus.h"

/* ************************************* Private macros ***************************************** */

/* ************************************* Private type definition ******************************** */

/* ************************************* Private functions prototypes *************************** */

/* ************************************* Private variables ************************************** */
static gyro_acc_t gyro_acc =
{
    .config =
    {
        .acc = ACC_RANGE_MEDIUM,
        .gyro = GYRO_RANGE_MEDIUM
    }
};

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
	static int result = 0;
	if (result == 0)
	{
		result = gyro_acc.driver->read_all_async(&gyro_acc);
	}
	else
	{
		for(;;);
	}
    HAL_Delay(10);
}
/* ************************************* Public callback functions ****************************** */
