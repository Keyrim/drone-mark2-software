/**
 * @file bus.h
 * @brief Interface pour les bus de communication i2c, spi
 * @author Théo Magne
 * @date 14/07/2023
 * @see bus.c
 */

#ifndef _BUS_H_
#define _BUS_H_

/* ************************************* Includes *********************************************** */
#include <stdint.h>

/* ************************************* Public macros ****************************************** */

/* ************************************* Public type definition ********************************* */
typedef int (*bus_init_t)(void);
typedef int (*bus_mem_write_t)(uint8_t bus_id, uint8_t dev, uint8_t mem,uint8_t* data,
                               uint8_t size);
typedef int (*bus_mem_read_t)(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data,
                              uint8_t size);
typedef int (*bus_mem_write_dma_t)(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data,
                                   uint8_t size);
typedef int (*bus_mem_read_dma_t)(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data,
                                  uint8_t size);
typedef int (*bus_write_t)(uint8_t bus_id, uint8_t dev, uint8_t* data, uint8_t size);
typedef int (*bus_read_t)(uint8_t bus_id, uint8_t dev, uint8_t* data, uint8_t size);
typedef int (*bus_deinit_t)(void);

/**
 * @brief Bus driver structure
 */
typedef struct
{
    bus_init_t init;                    /**< Bus initialization function */
    bus_mem_write_t mem_write;          /**< Bus memory write function */
    bus_mem_read_t mem_read;            /**< Bus memory read function */
    bus_mem_write_dma_t mem_write_dma;  /**< Bus memory write function with DMA */
    bus_mem_read_dma_t mem_read_dma;    /**< Bus memory read function with DMA */
    bus_write_t write;                  /**< Bus write function */
    bus_read_t read;                    /**< Bus read function */
    bus_deinit_t deinit;                /**< Bus deinitialization function */
} bus_driver_t;

/**
 * @brief Bus structure
 */
typedef struct
{
    uint8_t bus_id;         /**< Bus ID (spi or I2C 1, 2, 3, ...)*/
    bus_driver_t* driver;   /**< Bus driver containings the methods to use */
} bus_t;

/* ************************************* Public variables *************************************** */
extern bus_t bus_i2c_1;

/* ************************************* Public functions *************************************** */

/* ************************************* Public callback functions ****************************** */

#endif /* _BUS_H_ */