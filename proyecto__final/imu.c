/**
  * @file mpu9250.h
  * @author Mauricio Solis
  * @date 06/12/2018
  * @brief Implements the mpu9250 API
  *
  * This File was created by Mauricio Solis
*/


/*--->> Includes <<-----------------------------------------------------------*/
#include "i2c.h"
#include "mpu9250.h"
#include "string.h"
#include "utils.h"


#define LENGTH_WHO_AM_I             0x01

/*--->> Local vars <<----------------------------------------------------------*/
mpu9250_t mpu;


/*--->> API Definition <<-----------------------------------------------------*/

bool mpu9250_simple_init()
{
    memset(&mpu, 0x00, sizeof(mpu9250_t));
    i2c_init(MPU9250_ADDRESS, I2C_MODE_MASTER);
    return true;
}

bool mpu9250_perform_who_ami()
{
    return i2c_master_read(WHO_AM_I_MPU9250_REG, &(mpu.who_am_i), LENGTH_WHO_AM_I);
}

bool mpu9250_simple_config(mpu9250_config_t * mpu_cfg)
{
    //uint8_t reg;
    uint8_t dat = 0;
    
    mpu.mpu_conf = *mpu_cfg;    //saving the configuration
    
    
    //FILL HERE IN ORDER TO CALCULATE THE mpu.acc_resolution_factor
    switch(mpu.mpu_conf.acc_scale)
    {
        case ACC_SCALE_2G: mpu.acc_resolution_factor = (2.0/32767.0);break;
        default :mpu.acc_resolution_factor = 1.0;break;
    }
    
    
    bool ret;
    mpu9250_config_t* mpu_config = &mpu.mpu_conf;
  
    uint8_t gyro_conf = 0;
    uint8_t accel_conf = 0;
    
    dat = PWR_MGMT_1_H_RESET_MASK;
    ret = i2c_master_write(PWR_MGMT_1_REG, &dat, 1);/*Sending a reset to the mpu*/
    if(ret == false)
    {
        return false;
    }
    //HAL_Delay(AK8963_CHANGE_MODE_DELAY);
    m_delay(AK8963_CHANGE_MODE_DELAY);
    dat = PWR_MGMT_1_RESET_VALUE;
    ret = i2c_master_write(PWR_MGMT_1_REG, &dat, 1);/*cleaning power manager reg*/
    if(ret == false)
    {
        return ret;
    }
    //HAL_Delay(AK8963_CHANGE_MODE_DELAY);
    m_delay(AK8963_CHANGE_MODE_DELAY);
    dat = PWR_MGMT_1_CLKSEL_INTERAL_AUTO_OSC;
    ret = i2c_master_write(PWR_MGMT_1_REG, &dat, 1);/*selecting the oscilator*/
    if(ret == false)
    {
        return ret;
    }
    
    //HAL_Delay(200);
    m_delay(200);
    /*Disabling the accelerometer and gyroscope*/
    dat = PWR_MGMT_2_DISABLE_ACC_MASK | PWR_MGMT_2_DISABLE_GYR_MASK;
    ret = i2c_master_write(PWR_MGMT_2_REG, &dat, 1);
    if(ret == false)
    {
        return ret;
    }
    
    //m_delay(200);
    /*Setting the filter*/
    dat = CONFIG_REGISTER_DLPF_CFG_41HZ_5P9ms_1KHZ;
    ret = i2c_master_write(CONFIG_REG, &dat, 1);
    if(ret == false)
    {
        return false;
    }
    
    //m_delay(200);
    /*Setting the sample rate*/
    dat = mpu_config->mpu_odr;
    ret = i2c_master_write(SMPLRT_DIV_REG, &dat, 1);//mpu9250_set_sample_rate_div(mpu, mpu_config->mpu_odr);
    if(ret == false)
    {
        return false;
    }
    
    //m_delay(200);
    gyro_conf = 0;//disable Self Test, full scale selected = yy (xxx dps), Fchoice_b = 00 (Fchoice = 11)
    gyro_conf |= mpu_config->gyr_scale;
    ret = i2c_master_write(GYRO_CONFIG_REG, &gyro_conf, 1);
    if(ret == false)
    {
        return false;
    }
    
    accel_conf = 0;//disable Self Test, Fchoice_b = 00 (Fchoice = 11)
    accel_conf |= mpu_config->acc_scale;
    ret = i2c_master_write(ACCEL_CONFIG_REG, &accel_conf, 1);
    if(ret == false)
    {
        return false;
    }
    
    accel_conf = 0;//accel_fchoice_b = 0 -> accel_fchoice = 1, A_DLPFCFG = 0->BW = 218.1, Rate = 1KHz....
    ret = i2c_master_write(ACCEL_CONFIG2_REG, &accel_conf, 1);//ret = mpu9250_set_acc_config2_reg(mpu, accel_conf);
    if(ret == false)
    {
        return false;
    }
    
    
    return ret;
}

bool mpu9250_simple_start()
{
    uint8_t dat;
    bool ret;
    
    dat = ~(PWR_MGMT_2_DISABLE_ACC_MASK | PWR_MGMT_2_DISABLE_GYR_MASK);
    ret = i2c_master_write(PWR_MGMT_2_REG, &dat, 1);
    return ret;
}

bool mpu9250_simple_get_int_status()
{
    bool ret;
    ret = i2c_master_read(INT_STATUS_REG, &(mpu.int_status), 1);
    return ret;
}


bool mpu9250_simple_get_data()
{
    bool ret;
    ret = mpu9250_simple_get_int_status();
  
    if(ret == false)
    {
        return false;
    }
    
    if(mpu.int_status & INT_STATUS_RAW_DATA_RDY_INT_MASK )
    {
        mpu9250_simple_get_acc_raw_data();
        mpu9250_simple_get_gyr_raw_data();
        return true;
    }
    else
    {
        return false;
    }
}

bool mpu9250_simple_get_acc_raw_data()
{
    bool ret;
    uint8_t data_acc[6];
    ret = i2c_master_read(ACCEL_XOUT_H_REG, data_acc, 6);
    if(ret == true)
    {
        //FILL Here. put data_acc in mpu.mpu_acc_raw_data for ALL AXIS -> listo
        mpu.mpu_acc_raw_data.ax = ((int16_t)data_acc[0]) << 8 | data_acc[1];
        mpu.mpu_acc_raw_data.ay = ((int16_t)data_acc[2]) << 8 | data_acc[3];
        mpu.mpu_acc_raw_data.az = ((int16_t)data_acc[4]) << 8 | data_acc[5];
        
        //FILL Here. put mpu.mpu_acc_raw_data in mpu.acc_real_value. for ALL AXIS -> listo
        mpu.acc_real_value.ax = ((float)(mpu.mpu_acc_raw_data.ax)) * mpu.acc_resolution_factor;
        mpu.acc_real_value.ay = ((float)(mpu.mpu_acc_raw_data.ay)) * mpu.acc_resolution_factor;
        mpu.acc_real_value.az = ((float)(mpu.mpu_acc_raw_data.az)) * mpu.acc_resolution_factor;
    }
    return ret;
}

bool mpu9250_simple_get_gyr_raw_data()
{
    return false;
}
