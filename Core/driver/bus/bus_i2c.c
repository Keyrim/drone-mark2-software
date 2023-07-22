/**
 * @file bus_i2c.c
 * @brief  Bus interface implementation for i2c
 * @author Théo Magne
 * @date 15/07/2023
 * @see bus_i2c.h
 */

/* ************************************* Includes *********************************************** */
#include "bus_i2c.h"
#include "i2c.h"

/* ************************************* Private macros ***************************************** */
#define BUS_I2C_OPERATION_TIMEOUT_MS    10

/* ************************************* Private type definition ******************************** */
typedef enum
{
    I2C_1 = 0,
    I2C_COUNT
} bus_id_t;

/* ************************************* Private functions prototypes *************************** */
static int i2c_init(void);
static int i2c_mem_write(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size);
static int i2c_mem_read(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size);
static int i2c_mem_write_dma(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size);
static int i2c_mem_read_dma(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size);
static int i2c_write(uint8_t bus_id, uint8_t dev, uint8_t* data, uint8_t size);
static int i2c_read(uint8_t bus_id, uint8_t dev, uint8_t* data, uint8_t size);
static int i2c_deinit(void);

/* ************************************* Private variables ************************************** */
static I2C_HandleTypeDef* i2c[I2C_COUNT] =
{
    [I2C_1] = &hi2c1,
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

static int i2c_mem_write(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(i2c[bus_id], dev, mem, I2C_MEMADD_SIZE_8BIT,
                                                 data, size, BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_mem_read(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(i2c[bus_id], dev, mem, I2C_MEMADD_SIZE_8BIT,
                                                data, size, BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_mem_write_dma(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write_DMA(i2c[bus_id], dev, mem, I2C_MEMADD_SIZE_8BIT,
                                                     data, size);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_mem_read_dma(uint8_t bus_id, uint8_t dev, uint8_t mem, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read_DMA(i2c[bus_id], dev, mem, I2C_MEMADD_SIZE_8BIT,
                                                    data, size);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_write(uint8_t bus_id, uint8_t dev, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c[bus_id], dev, data, size, 
                                                       BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_read(uint8_t bus_id, uint8_t dev, uint8_t* data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Master_Receive(i2c[bus_id], dev, data, size,
                                                      BUS_I2C_OPERATION_TIMEOUT_MS);
    return (result == HAL_OK) ? 0 : -1;
}

static int i2c_deinit(void)
{
    return 0;
}

/* ************************************* Public functions *************************************** */

/* ************************************* Public callback functions ****************************** */
