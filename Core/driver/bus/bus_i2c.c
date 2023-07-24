/**
 * @file bus_i2c.c
 * @brief  Bus interface implementation for i2c
 * @author Théo Magne
 * @date 15/07/2023
 * @see bus_i2c.h
 */

/* ************************************* Includes *********************************************** */
#include <string.h>
#include "bus_i2c.h"
#include "i2c.h"

/* ************************************* Private macros ***************************************** */
#define BUS_I2C_OPERATION_TIMEOUT_MS    10

/* ************************************* Private type definition ******************************** */


/* ************************************* Private functions prototypes *************************** */
static int i2c_init(void);
static int i2c_mem_write(dev_info_t* dev_info, mem_info_t* mem_info);
static int i2c_mem_read(dev_info_t* dev_info, mem_info_t* mem_info);
static int i2c_mem_write_dma(dev_info_t* dev_info, mem_info_t* mem_info);
static int i2c_mem_read_dma(dev_info_t* dev_info, mem_info_t* mem_info);
static int i2c_write(dev_info_t* dev_info, uint8_t* data, uint8_t size);
static int i2c_read(dev_info_t* dev_info, uint8_t* data, uint8_t size);
static int i2c_deinit(void);
static inline bus_id_t i2c_get_bus_id(I2C_HandleTypeDef* hi2c);

/* ************************************* Private variables ************************************** */
static I2C_HandleTypeDef* i2c[BUS_COUNT] =
{
    [BUS_1] = &hi2c1,
};

static bus_cb_t bus_cb[BUS_COUNT] =
{
    [BUS_1] = {NULL, NULL},
};

/* ************************************* Public variables *************************************** */
bus_driver_t bus_driver_i2c =
{
    .init = i2c_init,
    .mem_write = i2c_mem_write,
    .mem_read = i2c_mem_read,
    .mem_write_dma = i2c_mem_write_dma,
    .mem_read_dma = i2c_mem_read_dma,
    .write = i2c_write,
    .read = i2c_read,
    .deinit = i2c_deinit,
};

/* ************************************* Private functions ************************************** */
static int i2c_init(void)
{
    return 0;
}

static int i2c_mem_write(dev_info_t* dev_info, mem_info_t* mem_info)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(i2c[dev_info->bus_id], dev_info->addr,
                                                 mem_info->addr, I2C_MEMADD_SIZE_8BIT,
                                                 mem_info->data, mem_info->size,
                                                 BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_mem_read(dev_info_t* dev_info, mem_info_t* mem_info)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(i2c[dev_info->bus_id], dev_info->addr,
                                                mem_info->addr, I2C_MEMADD_SIZE_8BIT,
                                                mem_info->data, mem_info->size,
                                                BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_mem_write_dma(dev_info_t* dev_info, mem_info_t* mem_info)
{
    memcpy(&bus_cb[dev_info->bus_id], dev_info->cb, sizeof(bus_cb_t));
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write_DMA(i2c[dev_info->bus_id], dev_info->addr,
                                                     mem_info->addr, I2C_MEMADD_SIZE_8BIT,
                                                     mem_info->data, mem_info->size);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_mem_read_dma(dev_info_t* dev_info, mem_info_t* mem_info)
{
    memcpy(&bus_cb[dev_info->bus_id], dev_info->cb, sizeof(bus_cb_t));
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read_DMA(i2c[dev_info->bus_id], dev_info->addr,
                                                    mem_info->addr, I2C_MEMADD_SIZE_8BIT,
                                                    mem_info->data, mem_info->size);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_write(dev_info_t* dev_info, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c[dev_info->bus_id], dev_info->addr, 
                                                       data, size, 
                                                       BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_read(dev_info_t* dev_info, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Master_Receive(i2c[dev_info->bus_id], dev_info->addr,
                                                      data, size,
                                                      BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_deinit(void)
{
    return 0;
}

static inline bus_id_t i2c_get_bus_id(I2C_HandleTypeDef* hi2c)
{
    bus_id_t bus_id = BUS_COUNT;
    while(bus_id--)
    {
        if(i2c[bus_id] == hi2c)
        {
            break;
        }
    }
    return bus_id;
}

/* ************************************* Public functions *************************************** */

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    bus_id_t bus_id = i2c_get_bus_id(hi2c);
    if(bus_cb[bus_id].transfer_cplt != NULL)
    {
        bus_cb[bus_id].transfer_cplt();
    }
    memset(&bus_cb[bus_id], 0, sizeof(bus_cb_t));
}


void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    bus_id_t bus_id = i2c_get_bus_id(hi2c);
    if(bus_cb[bus_id].transfer_cplt != NULL)
    {
        bus_cb[bus_id].transfer_cplt();
    }
    memset(&bus_cb[bus_id], 0, sizeof(bus_cb_t));
}
/* ************************************* Public callback functions ****************************** */
