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
typedef enum
{
    BUS_1 = 0,
    BUS_COUNT
} bus_id_t;


typedef void (*bus_transfer_complete_cb_t)(uint8_t bus_id);
typedef void (*bus_transfer_half_complete_cb_t)(uint8_t bus_id);

typedef struct
{
    bus_transfer_complete_cb_t transfer_cplt;               /**< Transfer complete callback */
    bus_transfer_half_complete_cb_t half_transfer_cplt;     /**< Half transfer complete callback */
} bus_cb_t;

typedef struct bus_t bus_t;

typedef struct
{
    bus_t* bus;             /**< Bus pointer */
    uint8_t addr;           /**< Device adress (after offset) */
    bus_cb_t* cb;           /**< Bus callback */
}dev_info_t;

typedef struct
{
    uint16_t addr;          /**< Memory address */
    uint8_t* data;          /**< Data buffer (in or out) */
    uint8_t size;           /**< Data size */
}mem_info_t;

typedef int (*bus_init_t)(bus_t *bus);
typedef int (*bus_is_device_ready_t)(dev_info_t* dev_info);
typedef int (*bus_mem_write_t)(dev_info_t* dev_info, mem_info_t* mem_info);
typedef int (*bus_mem_read_t)(dev_info_t* dev_info, mem_info_t* mem_info);
typedef int (*bus_mem_write_dma_t)(dev_info_t* dev_info, mem_info_t* mem_info);
typedef int (*bus_mem_read_dma_t)(dev_info_t* dev_info, mem_info_t* mem_info);
typedef int (*bus_write_t)(dev_info_t* dev_info, uint8_t* data, uint8_t size);
typedef int (*bus_read_t)(dev_info_t* dev_info, uint8_t* data, uint8_t size);
typedef int (*bus_deinit_t)(void);


/**
 * @brief Bus driver structure
 */
typedef struct
{
    bus_init_t init;                    /**< Bus initialization function */
    bus_is_device_ready_t is_ready;     /**< Bus device ready function */
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
typedef struct bus_t
{
    uint8_t id;                 /**< Bus ID (spi or I2C 1, 2, 3, ...)*/
    bus_driver_t* driver;       /**< Private use */
} bus_t;

/* ************************************* Public variables *************************************** */
extern bus_t bus_i2c_1;

/* ************************************* Public functions *************************************** */

/* ************************************* Public callback functions ****************************** */

#endif /* _BUS_H_ */
