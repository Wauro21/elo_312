/**
  * @file mpu9250.h
  * @author Mauricio Solis
  * @date 28/11/2018
  * @brief Define the mpu9250 API
  *
  * This File was created by Mauricio Solis
*/
  
  
#ifndef _MPU9250_H_
#define _MPU9250_H_

/*--->> Includes <<-----------------------------------------------------------*/
#include "i2c.h"

/*--->> Definition <<---------------------------------------------------------*/




/*-------------------------- Magnetometer Registers --------------------------*/
#define AK8963_ADDRESS              0x0C << 1

#define WHO_AM_I_AK8963_REG         0x00 // should return 0x48
#define INFO                        0x01

#define AK8963_ST1_REG              0x02  // data ready status bit 0
#define AK8963_ST1_DRDY_BIT         0x00  /*<< Dta Ready */
#define AK8963_ST1_DOR_BIT          0x01  /*<< Data Over Run */

#define AK8963_ST1_DRDY_MASK        1 << AK8963_ST1_DRDY_BIT
#define AK8963_ST1_DOR_MASK         1 << AK8963_ST1_DOR_BIT


#define AK8963_XOUT_L_REG           0x03  // data
#define AK8963_XOUT_H               0x04
#define AK8963_YOUT_L               0x05
#define AK8963_YOUT_H               0x06
#define AK8963_ZOUT_L               0x07
#define AK8963_ZOUT_H               0x08
#define AK8963_ST2_REG              0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_ST2_HOFL_BIT         0x03
#define AK8963_ST2_HOFL_MASK        1 << AK8963_ST2_HOFL_BIT

#define AK8963_CNTL_REG                 0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0

#define AK8963_CNTL_MODE_POWER_DOWN     0x00
#define AK8963_CNTL_MODE_SIGLE          0x01
#define AK8963_CNTL_MODE_CONTINUOUS_1   0x02  /*<< This set the ODR in 8Hz*/
#define AK8963_CNTL_MODE_CONTINUOUS_2   0x06  /*<< This set the ODR in 100Hz*/
#define AK8963_CNTL_MODE_EXTERNAL_TRIG  0x04  /*<< External Triggers*/
#define AK8963_CNTL_MODE_SELF_TEST      0x08
#define AK8963_CNTL_MODE_FUSE_ROM       0x0F

#define AK8963_CNTL_RESOLUTION_BIT      0x04
#define AK8963_CNTL_RESOLUTION_16_MASK  1 << AK8963_CNTL_RESOLUTION_BIT


#define AK8963_CNTL2_REG                0x0B
#define AK8963_CNTL2_SRST_BIT           0x00  /*<< Soft Reset*/
#define AK8963_CNTL2_SRST_MASK          1 << AK8963_CNTL2_SRST_BIT


#define AK8963_ASTC_REG                 0x0C  // Self test control
#define AK9863_ASTC_SELF_BIT            0x06;
#define AK9863_ASTC_SELF_MASK           1 << AK9863_ASTC_SELF_BIT;

#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX_REG  0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value
/*----------------------------------------------------------------------------*/


#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A       0x10

#define XG_OFFSET_H       0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L       0x14
#define YG_OFFSET_H       0x15
#define YG_OFFSET_L       0x16
#define ZG_OFFSET_H       0x17
#define ZG_OFFSET_L       0x18

#define SMPLRT_DIV_REG        0x19 /* << Divides the internal sample rate*/
#define SMPLRT_DIV_200HZ      0x04 /* << When the internal sample rate is 1K, this value presacale the SR to 200HZ*/

/*--------------------------- CONGIF REGISTER --------------------------------*/
#define CONFIG_REG            0x1A
#define CONFIG_REGISTER_FIFO_MODE_BIT       0x06 /*<< 1 = fifo full, 0 = fifo available to be written*/
#define CONFIG_REGISTER_EXT_SYNCH_SET_BIT2  0x05
#define CONFIG_REGISTER_EXT_SYNCH_SET_BIT1  0x04
#define CONFIG_REGISTER_EXT_SYNCH_SET_BIT0  0x03


                                                /* 0 = function disabled
                                                 * 1 = TEMP_OUT_L[0]
                                                 * 2 = GYRO_XOUT_L[0]
                                                 * 3 = GYRO_YOUT_L[0]
                                                 * 4 = GYRO_ZOUT_L[0]
                                                 * 5 = ACCEL_XOUT_L[0]
                                                 * 6 = ACCEL_YOUT_L[0]
                                                 * 7 = ACCEL_ZOUT_L[0]*/
#define CONFIG_REGISTER_DLPF_CFG_BIT2       0x02
#define CONFIG_REGISTER_DLPF_CFG_BIT1       0x01
#define CONFIG_REGISTER_DLPF_CFG_BIT0       0x00
#define CONFIG_REGISTER_DLPF_CFG_92HZ_3P9ms_1KHZ    0x02
#define CONFIG_REGISTER_DLPF_CFG_41HZ_5P9ms_1KHZ    0x03
/*----------------------------------------------------------------------------*/

/*------------------------ GYROSCOPE CONFIGURATION ---------------------------*/
#define GYRO_CONFIG_REG                     0x1B
#define GYRO_CONFIG_XGYRO_Cten_BIT          0x07  /*<< X Gyro self-test.*/
#define GYRO_CONFIG_YGYRO_Cten_BIT          0x06  /*<< Y Gyro self-test.*/
#define GYRO_CONFIG_ZGYRO_Cten_BIT          0x05  /*<< Z Gyro self-test.*/

#define GYRO_CONFIG_XGYRO_Cten_MASK         1 << GYRO_CONFIG_XGYRO_Cten_BIT
#define GYRO_CONFIG_YGYRO_Cten_MASK         1 << GYRO_CONFIG_YGYRO_Cten_BIT
#define GYRO_CONFIG_ZGYRO_Cten_MASK         1 << GYRO_CONFIG_ZGYRO_Cten_BIT

#define GYRO_CONFIG_ZGYRO_FS_SEL_BIT0       0x03  /*<< Gyro full scale selector.*/
#define GYRO_CONFIG_ZGYRO_FS_SEL_BIT1       0x04  /*<< Gyro full scale selector.*/
                                                  /* 0x00 = +-250 dps
                                                     0x01 = +-500 dps
                                                     0x02 = +-1000 dps
                                                     0x03 = +-2000 dps */
#define GYRO_CONFIG_ZGYRO_FS_SEL_250_DPS    0x00
#define GYRO_CONFIG_ZGYRO_FS_SEL_500_DPS    0x01
#define GYRO_CONFIG_ZGYRO_FS_SEL_1000_DPS   0x02
#define GYRO_CONFIG_ZGYRO_FS_SEL_2000_DPS   0x03

#define GYRO_CONFIG_ZGYRO_FS_SEL_250_DPS_MASK   GYRO_CONFIG_ZGYRO_FS_SEL_250_DPS << GYRO_CONFIG_ZGYRO_FS_SEL_BIT0
#define GYRO_CONFIG_ZGYRO_FS_SEL_500_DPS_MASK   GYRO_CONFIG_ZGYRO_FS_SEL_500_DPS << GYRO_CONFIG_ZGYRO_FS_SEL_BIT0
#define GYRO_CONFIG_ZGYRO_FS_SEL_1000_DPS_MASK  GYRO_CONFIG_ZGYRO_FS_SEL_1000_DPS << GYRO_CONFIG_ZGYRO_FS_SEL_BIT0
#define GYRO_CONFIG_ZGYRO_FS_SEL_2000_DPS_MASK  GYRO_CONFIG_ZGYRO_FS_SEL_2000_DPS << GYRO_CONFIG_ZGYRO_FS_SEL_BIT0

#define GYRO_CONFIG_ZGYRO_FCHIOCE_B_BIT0    0x00  /*<< For details see page 13-14 of Register Map and Descriptions*/
#define GYRO_CONFIG_ZGYRO_FCHOICE_B_BIT1    0X01

#define GYRO_CONFIG_ZGYRO_FCHIOCE_B_00    0x00
#define GYRO_CONFIG_ZGYRO_FCHIOCE_B_01    0x01
#define GYRO_CONFIG_ZGYRO_FCHIOCE_B_10    0x10
#define GYRO_CONFIG_ZGYRO_FCHIOCE_B_11    0x11

/*----------------------------------------------------------------------------*/
/*----------------------- ACCELEROMETER CONFIGURATION ------------------------*/
#define ACCEL_CONFIG_REG      0x1C
#define ACCEL_CONFIG_AX_SELFTEST_ENABLED_BIT  0x07
#define ACCEL_CONFIG_AY_SELFTEST_ENABLED_BIT  0x06
#define ACCEL_CONFIG_AZ_SELFTEST_ENABLED_BIT  0x05

#define ACCEL_CONFIG_ACCEL_FS_SEL_BIT0        0x03
#define ACCEL_CONFIG_ACCEL_FS_SEL_BIT1        0x04
                                                  /* 0x00 = +- 2g
                                                     0x01 = +- 4g
                                                     0x01 = +- 8g
                                                     0x01 = +- 16g*/
#define ACCEL_CONFIG_ACCEL_FS_2G              0x00
#define ACCEL_CONFIG_ACCEL_FS_4G              0x01
#define ACCEL_CONFIG_ACCEL_FS_8G              0x02
#define ACCEL_CONFIG_ACCEL_FS_16G             0x03

#define ACCEL_CONFIG_ACCEL_FS_2G_MASK         ACCEL_CONFIG_ACCEL_FS_2G << ACCEL_CONFIG_ACCEL_FS_SEL_BIT0
#define ACCEL_CONFIG_ACCEL_FS_4G_MASK         ACCEL_CONFIG_ACCEL_FS_4G << ACCEL_CONFIG_ACCEL_FS_SEL_BIT0
#define ACCEL_CONFIG_ACCEL_FS_8G_MASK         ACCEL_CONFIG_ACCEL_FS_8G << ACCEL_CONFIG_ACCEL_FS_SEL_BIT0
#define ACCEL_CONFIG_ACCEL_FS_16G_MASK        ACCEL_CONFIG_ACCEL_FS_16G << ACCEL_CONFIG_ACCEL_FS_SEL_BIT0


#define ACCEL_CONFIG2_REG     0x1D

#define ACCEL_CONFIG2_ACCEL_FCHIOCE_B_BIT     0x03
#define ACCEL_CONFIG2_A_DLPFCFG_BIT0          0x00 /*<< For Details see page 15 of Register Map and Descriptions*/
#define ACCEL_CONFIG2_A_DLPFCFG_BIT1          0x00
#define ACCEL_CONFIG2_A_DLPFCFG_BIT2          0x00 

#define ACCEL_CONFIG2_ZGYRO_FCHIOCE_B_00    0x00
#define ACCEL_CONFIG2_ZGYRO_FCHIOCE_B_01    0x01
#define ACCEL_CONFIG2_ZGYRO_FCHIOCE_B_10    0x10
#define ACCEL_CONFIG2_ZGYRO_FCHIOCE_B_11    0x11


#define LP_ACCEL_ODR_REG                    0x1E
#define LP_ACCEL_ODR_LPOSC_CLKSEL_BIT0      0x00
#define LP_ACCEL_ODR_LPOSC_CLKSEL_BIT1      0x01
#define LP_ACCEL_ODR_LPOSC_CLKSEL_BIT2      0x02
#define LP_ACCEL_ODR_LPOSC_CLKSEL_BIT3      0x03

#define LP_ACCEL_ODR_LPOSC_CLKSEL_0p24      0x00
#define LP_ACCEL_ODR_LPOSC_CLKSEL_0p49      0x01
#define LP_ACCEL_ODR_LPOSC_CLKSEL_0p98      0x02
#define LP_ACCEL_ODR_LPOSC_CLKSEL_1p95      0x03
#define LP_ACCEL_ODR_LPOSC_CLKSEL_3p91      0x04



/*----------------------------------------------------------------------------*/


#define WOM_THR           0x1F

// Duration counter threshold for motion interrupt generation, 1 kHz rate,
// LSB = 1 ms
#define MOT_DUR           0x20
// Zero-motion detection threshold bits [7:0]
#define ZMOT_THR          0x21
// Duration counter threshold for zero motion interrupt generation, 16 Hz rate,
// LSB = 64 ms
#define ZRMOT_DUR         0x22

#define FIFO_EN            0x23
#define I2C_MST_CTRL_REG                0x24
#define I2C_MST_CTRL_MULT_MST_EN_BIT    0x07
#define I2C_MST_CTRL_WAIT_FOR_ES_BIT    0x06
#define I2C_MST_CTRL_SLV_3_FIFO_EN_BIT  0x05
#define I2C_MST_CTRL_I2C_MST_P_NSR_BIT  0x04
#define I2C_MST_CTRL_I2C_MST_CLK_3_BIT  0x03
#define I2C_MST_CTRL_I2C_MST_CLK_2_BIT  0x02
#define I2C_MST_CTRL_I2C_MST_CLK_1_BIT  0x01
#define I2C_MST_CTRL_I2C_MST_CLK_0_BIT  0x00

#define I2C_MST_CTRL_I2C_MST_CLK_400KHZ 0x0D << I2C_MST_CTRL_I2C_MST_CLK_0_BIT



#define I2C_SLV0_ADDR_REG       0x25
#define I2C_SLV0_REG            0x26
#define I2C_SLV0_CTRL           0x27
#define I2C_SLV0_EN_BIT         0x07
#define I2C_SLV0_EN_MASK        1 << I2C_SLV0_EN_BIT


#define I2C_SLV1_ADDR      0x28
#define I2C_SLV1_REG       0x29
#define I2C_SLV1_CTRL      0x2A
#define I2C_SLV2_ADDR      0x2B
#define I2C_SLV2_REG       0x2C
#define I2C_SLV2_CTRL      0x2D
#define I2C_SLV3_ADDR      0x2E
#define I2C_SLV3_REG       0x2F
#define I2C_SLV3_CTRL      0x30
#define I2C_SLV4_ADDR      0x31
#define I2C_SLV4_REG       0x32
#define I2C_SLV4_DO        0x33
#define I2C_SLV4_CTRL      0x34
#define I2C_SLV4_DI        0x35
#define I2C_MST_STATUS     0x36

/*-------------------------- INT PIN  CONFIGURATION --------------------------*/
#define INT_PIN_CFG_REG                         0x37
#define INT_PIN_CFG_ACTL_BIT                    0x07
#define INT_PIN_CFG_OPEN_BIT                    0x06
#define INT_PIN_CFG_LATCH_INT_EN_BIT            0x05
#define INT_PIN_CFG_INT_ANYRD_2CLEAR_BIT        0x04
#define INT_PIN_CFG_ACTL_FSYNC_BIT              0x03
#define INT_PIN_CFG_FSYNCK_INT_MODE_EN_BIT      0x02
#define INT_PIN_CFG_BYPASS_EN_BIT               0x01

#define INT_PIN_CFG_ACTL_MASK                   1 << INT_PIN_CFG_ACTL_BIT
#define INT_PIN_CFG_OPEN_MASK                   1 << INT_PIN_CFG_OPEN_BIT
#define INT_PIN_CFG_LATCH_INT_EN_MASK           1 << INT_PIN_CFG_LATCH_INT_EN_BIT
#define INT_PIN_CFG_INT_ANYRD_2CLEAR_MASK       1 << INT_PIN_CFG_INT_ANYRD_2CLEAR_BIT
#define INT_PIN_CFG_ACTL_FSYNC_MASK             1 << INT_PIN_CFG_ACTL_FSYNC_BIT
#define INT_PIN_CFG_FSYNCK_INT_MODE_EN_MASK     1 << INT_PIN_CFG_FSYNCK_INT_MODE_EN_BIT
#define INT_PIN_CFG_BYPASS_EN_MASK              1 << INT_PIN_CFG_BYPASS_EN_BIT
#define INT_PIN_CFC_CLEAR_VALUE                 0x00


/*----------------------------------------------------------------------------*/
/*--------------------- INTERRUPTION ENABLE CONFIGURATION --------------------*/
#define INT_ENABLE_REG                  0x38
#define INT_ENABLE_WOM_EN_BIT           0x06
#define INT_ENABLE_FIFO_OF_EN_BIT       0x04
#define INT_ENABLE_FSYNC_INT_EN_BIT     0x03
#define INT_ENABLE_RAW_RDY_EN_BIT       0x00

#define INT_ENABLE_WOM_EN_MASK          1 << INT_ENABLE_WOM_EN_BIT
#define INT_ENABLE_FIFO_OF_EN_MASK      1 << INT_ENABLE_FIFO_OF_EN_BIT
#define INT_ENABLE_FSYNC_INT_EN_MASK    1 << INT_ENABLE_FSYNC_INT_EN_BIT
#define INT_ENABLE_RAW_RDY_EN_MASK      1 << INT_ENABLE_RAW_RDY_EN_BIT
#define INT_ENABLE_DISABLE_INTERRUPT    0x00


/*----------------------------------------------------------------------------*/

#define DMP_INT_STATUS                      0x39  // Check DMP interrupt

#define INT_STATUS_REG                      0x3A
#define INT_STATUS_WOM_INT_BIT              0x06
#define INT_STATUS_FIFO_OVERFLOW_INT_BIT    0x04
#define INT_STATUS_FSYNCK_INT_BIT           0x03
#define INT_STATUS_RAW_DATA_RDY_INT_BIT     0x00

#define INT_STATUS_WOM_INT_MASK             1 << INT_STATUS_WOM_INT_BIT
#define INT_STATUS_FIFO_OVERFLOW_INT_MASK   1 << INT_STATUS_FIFO_OVERFLOW_INT_BIT
#define INT_STATUS_FSYNCK_INT_MASK          1 << INT_STATUS_FSYNCK_INT_BIT
#define INT_STATUS_RAW_DATA_RDY_INT_MASK    1 << INT_STATUS_RAW_DATA_RDY_INT_BIT


#define ACCEL_XOUT_H_REG        0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define TEMP_OUT_H              0x41
#define TEMP_OUT_L              0x42
#define GYRO_XOUT_H_REG         0x43
#define GYRO_XOUT_L             0x44
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48


#define USER_CTRL_REG          0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define USER_CTRL_I2C_MST_EN_BIT  0x05
#define USER_CTRL_I2C_MST_EN_MASK 1 << USER_CTRL_I2C_MST_EN_BIT

/*----------------------- POWER MANAGER CONFIGURATION ------------------------*/
#define PWR_MGMT_1_REG          0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_1_H_RESET_BIT  0x07
#define PWR_MGMT_1_SLEEP_BIT    0x06
#define PWR_MGMT_1_CYCLE_BIT    0x05
#define PWR_MGMT_1_GYRO_SB_BIT  0x04
#define PWR_MGMT_1_PD_PTAT_BIT  0x03
#define PWR_MGMT_1_CLKSEL_BIT2  0x02
#define PWR_MGMT_1_CLKSEL_BIT1  0x01
#define PWR_MGMT_1_CLKSEL_BIT0  0x00

#define PWR_MGMT_1_H_RESET_MASK             1 << PWR_MGMT_1_H_RESET_BIT
#define PWR_MGMT_1_SLEEP_MASK               1 << PWR_MGMT_1_SLEEP_BIT
#define PWR_MGMT_1_CYCLE_MASK               1 << PWR_MGMT_1_CYCLE_BIT
#define PWR_MGMT_1_GYRO_SB_MASK             1 << PWR_MGMT_1_GYRO_SB_BIT
#define PWR_MGMT_1_PD_PTAT_MASK             1 << PWR_MGMT_1_PD_PTAT_BIT
#define PWR_MGMT_1_CLKSEL_INTERAL_OSC       0x00
#define PWR_MGMT_1_CLKSEL_INTERAL_AUTO_OSC  0x01

#define PWR_MGMT_1_RESET_VALUE              0x00

#define PWR_MGMT_2_REG                      0x6C
#define PWR_MGMT_2_DISABLE_XA_BIT           0x05
#define PWR_MGMT_2_DISABLE_YA_BIT           0x04
#define PWR_MGMT_2_DISABLE_ZA_BIT           0x03
#define PWR_MGMT_2_DISABLE_XG_BIT           0x02
#define PWR_MGMT_2_DISABLE_YG_BIT           0x01
#define PWR_MGMT_2_DISABLE_ZG_BIT           0x00

#define PWR_MGMT_2_DISABLE_XA_MASK          1 << PWR_MGMT_2_DISABLE_XA_BIT
#define PWR_MGMT_2_DISABLE_YA_MASK          1 << PWR_MGMT_2_DISABLE_YA_BIT
#define PWR_MGMT_2_DISABLE_ZA_MASK          1 << PWR_MGMT_2_DISABLE_ZA_BIT
#define PWR_MGMT_2_DISABLE_XG_MASK          1 << PWR_MGMT_2_DISABLE_XG_BIT
#define PWR_MGMT_2_DISABLE_YG_MASK          1 << PWR_MGMT_2_DISABLE_YG_BIT
#define PWR_MGMT_2_DISABLE_ZG_MASK          1 << PWR_MGMT_2_DISABLE_ZG_BIT

#define PWR_MGMT_2_DISABLE_ACC_MASK     PWR_MGMT_2_DISABLE_XA_MASK | PWR_MGMT_2_DISABLE_YA_MASK | PWR_MGMT_2_DISABLE_ZA_MASK
#define PWR_MGMT_2_DISABLE_GYR_MASK     PWR_MGMT_2_DISABLE_XG_MASK | PWR_MGMT_2_DISABLE_YG_MASK | PWR_MGMT_2_DISABLE_ZG_MASK



/*----------------------------------------------------------------------------*/



#define DMP_BANK           0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT         0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG            0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1          0x70
#define DMP_REG_2          0x71
#define FIFO_COUNTH        0x72
#define FIFO_COUNTL        0x73
#define FIFO_R_W           0x74
#define WHO_AM_I_MPU9250_REG   0x75 // Should return 0x71
#define XA_OFFSET_H        0x77
#define XA_OFFSET_L        0x78
#define YA_OFFSET_H        0x7A
#define YA_OFFSET_L        0x7B
#define ZA_OFFSET_H        0x7D
#define ZA_OFFSET_L        0x7E

// Using the MPU-9250 breakout board, ADO is set to 0
// Seven-bit device address is 110100 for ADO = 0 and 110101 for ADO = 1
#define ADO 0
#if ADO
#define MPU9250_ADDRESS 0x69   // Device address when ADO = 1
#else
#define MPU9250_ADDRESS 0x68   // Device address when ADO = 0
//#define AK8963_ADDRESS  0x0C  // Address of magnetometer
#endif // AD0

#define MPU_TX_BUFFER_MAX_SIZE        100
#define MPU_RX_BUFFER_MAX_SIZE        100

#define AK8963_CHANGE_MODE_DELAY      100 /*<< in ms*/

#define IMU_NUMBER_OF_AXIS            3

/*--->> Data types <<---------------------------------------------------------*/

/**
  * @brief The most common Sample Rate Divisor to get  the most common 
  * Output Data Rate
  * ODR = 1KHz/(1 + Sample Divisor)
  */
typedef enum
{
  ODR_1KHZ  = 0,
  ODR_500HZ = 1,
  ODR_333HZ = 2,
  ODR_250HZ = 3,
  ODR_200HZ = 4,
  ODR_166HZ = 5,
  ODR_143HZ = 6,
  ODR_125HZ = 7,
  ODR_111HZ = 8,
  ODR_100HZ = 9,
  ODR_50HZ  = 20,
  ODR_ERROR = 1000
}mpu9250_output_data_rate_t;

typedef enum
{
  MAG_ODR_8HZ = 0,
  MAG_ODR_100HZ = 1,
  MAG_ODR_ERROR = 2
}mag_odr_t;

/**
  * @brief Possible Gyroscope Scale 
  */
typedef enum
{
  GYR_SCALE_250_DPS = GYRO_CONFIG_ZGYRO_FS_SEL_250_DPS_MASK,
  GYR_SCALE_500_DPS = GYRO_CONFIG_ZGYRO_FS_SEL_500_DPS_MASK,
  GYR_SCALE_1000_DPS = GYRO_CONFIG_ZGYRO_FS_SEL_1000_DPS_MASK,
  GYR_SCALE_2000_DPS = GYRO_CONFIG_ZGYRO_FS_SEL_2000_DPS_MASK,
  GYR_SCALE_ERROR
}mpu9250_gyr_scale_t;

/**
  * @brief Possible Accelerometer Scale 
  */
typedef enum
{
  ACC_SCALE_2G = ACCEL_CONFIG_ACCEL_FS_2G_MASK,
  ACC_SCALE_4G = ACCEL_CONFIG_ACCEL_FS_4G_MASK,
  ACC_SCALE_8G = ACCEL_CONFIG_ACCEL_FS_8G_MASK,
  ACC_SCALE_16G = ACCEL_CONFIG_ACCEL_FS_16G_MASK,
  ACC_SCALE_ERROR
}mpu9250_acc_scale_t;

/**
  * @brief Possible Magnetometer Scale 
  */
typedef enum
{
  MAG_SCALE_4800_uT,
}mpu9250_mag_scale_t;


/**
  * @brief To enumerate all possible output data rate of the accelerometer 
  * in LOW POWER MODE
  */
typedef enum
{
  acc_odr_0p24hz,
  acc_odr_0p49hz,
  acc_odr_0p98hz,
  acc_odr_1p95hz,
  acc_odr_3p91hz,
  acc_odr_7p81hz,
  acc_odr_15p63hz,
  acc_odr_31p25hz,
  acc_odr_62p50hz,
  acc_odr_125hz,
  acc_odr_250hz,
  acc_odr_500hz,
}mpu9250_acc_odr_t;

/**
  * @brief To enumerate the possible mag bit resolutions
  */
typedef enum
{
  mag_14_bits = 0x00,
  mag_16_bits = AK8963_CNTL_RESOLUTION_16_MASK,
}mag_resolution_t;

/**
  * @brief This is the MPU config strucy, used to configure the mpu9250 and AK8963
  */
typedef struct
{
  mpu9250_gyr_scale_t gyr_scale;        /*<< The gyr scale. */
  mpu9250_acc_scale_t acc_scale;        /*<< The acc scale. */
  mag_resolution_t mag_resolution;      /*<< The mag resolution. */
  mpu9250_output_data_rate_t mpu_odr;   /*<< The acc and gyr output data rate.*/
  mag_odr_t                 mag_odr;    /*<< The mag output data rate*/
}mpu9250_config_t;


typedef enum
{
  EVENT_IMU_INT,
  EVENT_I2C,
  EVENT_I2C_ERROR,
  EVENT_MAINTHREAD
}mpu_event_t;



/**
  * @brief Magnetometer Sensitivity adjustment values
  * @param
  */
typedef struct
{
  uint8_t  asax;         /*< ASA X Raw data */
  uint8_t  asay;         /*< ASA Y Raw data */
  uint8_t  asaz;         /*< ASA Z Raw data */
} mag_asa_raw_t;

/**
  * @brief Magnetometer Sensitivity fator values
  * @param
  */
typedef struct
{
  float mag_sens_x;     /*< Mag sensitivity X value */
  float mag_sens_y;     /*< Mag sensitivity Y value */
  float mag_sens_z;     /*< Mag sensitivity Z value */
  
}mag_sensitivity_factor_t;

/**
  * @brief Magnetometer Data
  * @param
  */
typedef struct
{
  int16_t  mx;         /*< Mag X Raw data */
  int16_t  my;         /*< Mag Y Raw data */
  int16_t  mz;         /*< Mag Z Raw data */
} mpu9250_mag_data_t;

/**
  * @brief Accelerometer Data
  * @param
  */
typedef struct
{
  int16_t  ax;         /*< Acc X Raw data */
  int16_t  ay;         /*< Acc Y Raw data */
  int16_t  az;         /*< Acc Z Raw data */
} mpu9250_acc_data_t;

/**
  * @brief Gyroscope Data
  * @param
  */
typedef struct
{
  int16_t  gx;         /*< Gyr X Raw data */
  int16_t  gy;         /*< Gyr Y Raw data */
  int16_t  gz;         /*< Gyr Z Raw data */
} mpu9250_gyr_data_t;

/**
  * @brief Magnetometer Data in uT or mG or uT / 50
  * 
  */
typedef struct 
{
  float mx;   /*<< the real magnetometer x value*/
  float my;   /*<< the real magnetometer y value*/
  float mz;   /*<< the real magnetometer z value*/
}mag_real_value_t;

/**
  * @brief Accelerometer Data in g
  * 
  */
typedef struct 
{
  float ax;   /*<< the real accelerometer x value*/
  float ay;   /*<< the real accelerometer y value*/
  float az;   /*<< the real accelerometer z value*/
}acc_real_value_t;

/**
  * @brief Gyroscope Data in dps
  * 
  */
typedef struct 
{
  float gx;   /*<< the real gyroscope x value*/
  float gy;   /*<< the real gyroscope y value*/
  float gz;   /*<< the real gyroscope z value*/
}gyr_real_value_t;


/**
  * @brief This are all the registers in the magnetometer device.
  * use it just to verify data.
  * 
  */
typedef struct
{
  uint8_t wia;
  uint8_t info;
  uint8_t st1;
  uint8_t hxl;
  uint8_t hxh;
  uint8_t hyl;
  uint8_t hyh;
  uint8_t hzl;
  uint8_t hzh;
  uint8_t st2;
  uint8_t cntl;
  uint8_t rsv;
  uint8_t astc;
  uint8_t ts1;
  uint8_t ts2;
  uint8_t i2cdis;
  uint8_t asax;
  uint8_t asay;
  uint8_t asaz;
} all_mag_data_t;


typedef void (*new_complete_imu_data_available_f)(uint32_t period);/*<< The prototype of callback function, period in [ms]*/

/**
  * @brief Represents an instance of the MPU9250 driver
  */
typedef struct 
{
  //t_i2c_usm*          i2c;              /*<< ptr. to I2C HAL instance. */
  bool                mpu_enabled;      /*<< If the configuration or Self Test fail. mpu must be disabled*/
  mpu9250_config_t    mpu_conf;         /*<< Configuration set*/
  
  bool                stop_imu;         /*<< This flag is used to stop the imu when the reception is doing by interruptions*/
  
  uint8_t             who_am_i;
  uint8_t             int_status;       /*<< Interruption Status*/
  uint8_t             st1;              /*<< The ST1 of AK8963*/
  
  uint8_t             mpu_tx_buffer[MPU_TX_BUFFER_MAX_SIZE];  /*<< buffer used when the data is sent by I2C interrupts*/
  uint8_t             mpu_rx_buffer[MPU_RX_BUFFER_MAX_SIZE];  /*<< buffer used when the data is received by I2C interrupts*/
  
  mpu9250_mag_data_t  mpu_mag_raw_data; /*<< Magnetometer Raw Data... direct from IMU. */
  mpu9250_acc_data_t  mpu_acc_raw_data; /*<< Acceletometer Raw Data... direct from IMU. */
  mpu9250_gyr_data_t  mpu_gyr_raw_data; /*<< Gyroscope Raw Data... direct from IMU. */
  
  float               mag_resolution_factor;/*<< To scale/transform the raw data into real data. */
  mag_asa_raw_t       mag_asa_raw;          /*<< sensitivitys raw data, direct from Compass ROM. */
  mag_sensitivity_factor_t mag_sens_fact;   /*<< The sensitivity factors to compense the measurements. */
  
  float               acc_resolution_factor;  /*<< To scale/transform/adjust the rw acc data to real data*/
  float               gyr_resolution_factor;  /*<< To scale/transform/adjust the rw gyr data to real data*/
  
  mag_real_value_t mag_real_value;      /*<< magnetometer real value. */
  acc_real_value_t acc_real_value;      /*<< accelerometer real value. */
  gyr_real_value_t gyr_real_value;      /*<< gyroscope real value. */

  
} mpu9250_t;




/*--->> API Definition <<-----------------------------------------------------*/
/*------------------- USM SECTION --------------------------------------------*/
bool mpu9250_simple_init();

bool mpu9250_perform_who_ami();

bool mpu9250_simple_config(mpu9250_config_t * mpu_cfg);

bool mpu9250_simple_start();

bool mpu9250_simple_get_data();

bool mpu9250_simple_get_acc_data();

bool mpu9250_simple_get_int_status();

bool mpu9250_simple_get_acc_raw_data();

bool mpu9250_simple_get_gyr_raw_data();

extern mpu9250_t mpu;

#endif /*_MPU9250_H_*/
