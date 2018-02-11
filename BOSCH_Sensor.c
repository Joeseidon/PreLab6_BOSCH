/*
 * BOSCH_Sensor.c
 *
 *  Created on: Feb 11, 2018
 *      Author: joe
 */

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "bme280.h"
#include "BOSCH_Sensor.h"

#include "I2C_interface.h"

struct bme280_dev dev;
int8_t rslt,com_rslt;
uint8_t v_data_u8;
struct bme280_data *device_data;
struct bme280_dev *p_bme280 = &dev;

int8_t sensorInit(void){
    I2C_Init();
    int8_t rslt = BME280_OK;
    dev.dev_id = BME280_I2C_ADDR_PRIM;
    dev.intf = BME280_I2C_INTF;
    dev.read = (bme280_com_fptr_t)I2C_WRITE_READ_STRING;
    dev.write = (bme280_com_fptr_t)I2C_WRITE_STRING;
    dev.delay_ms = user_delay_ms;

    //set default values
    device_data->humidity=0.0;
    device_data->pressure=0.0;
    device_data->temperature=0.0;
    rslt = bme280_init(&dev);

    return rslt;
}

int8_t verifyDeviceAddress(void){
    //struct bme280_dev *p_bme280 = &dev;
    com_rslt = p_bme280->read(p_bme280->dev_id,BME280_CHIP_ID_ADDR, &v_data_u8,8);
    if(v_data_u8 == 0x60){
        return 1; //correct address
    }else
        return 0;
}

int8_t retrieveCalibratedData(void){
    int8_t rtn;
    struct bme280_data data = {0};
    //data input stores the calibrated values for temp, pressure, and humidity
    rtn = bme280_get_sensor_data(BME280_ALL, &data, p_bme280);
    device_data=&data;
   /*if(rtn !=0){
        return -1; //something went wrong
    }else{
        return 0;
    }*/
    return rtn;
}

double getTemp(void){
    return device_data->temperature;
}

double getHumidity(void){
    return device_data->humidity;
}

double getPressure(void){
    return device_data->pressure;
}
