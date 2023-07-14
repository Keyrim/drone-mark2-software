/**
 * @file bus.c
 * @brief Interface for the communication bus driver
 * @author Théo Magne
 * @date 14/07/2023
 * @see bus.h
 */

/* ************************************* Includes *********************************************** */
#include "bus.h"
#include "bus_i2c.h"

/* ************************************* Private macros ***************************************** */

/* ************************************* Private type definition ******************************** */

/* ************************************* Private functions prototypes *************************** */

/* ************************************* Private variables ************************************** */

/* ************************************* Public variables *************************************** */
bus_t bus_i2c_1 =
{
    .bus_id = 1,
    .driver = &bus_driver_i2c
};

/* ************************************* Private functions ************************************** */

/* ************************************* Public functions *************************************** */

/* ************************************* Public callback functions ****************************** */
