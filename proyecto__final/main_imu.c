/**
 * Este ejemplo permite
 * 1. Seleccionar mediante  HW_IN_USE si qué hardware se usará
 * 2. Cuando se selecciona USE_RTC se seteará una hora y en la interrupción del 
 *    timerA se leerán los segundos cada 2 segundos
 * 3. Cuando se selecciona USE_IMU se leerá el WHO_AM_I
 *
 *
 * Para ver los resultados de la comunicación, detenga el programa y revise
 * las estructuras the_rtc o mpu según corresponda.
*/

#include "msp430_version.h"
#include "mpu9250.h"

//#include "i2c.h"
#include "osc.h"
#include "ds1307.h"
#include "string.h"
#include "display.h"

#define USE_RTC     0
#define USE_IMU     1

#define HW_IN_USE   USE_IMU


t_data_time d_t;
mpu9250_config_t imu_config;

int main( void )
{
    bool ret;
  	osc_init_xt2();
	// Stop watchdog timer to prevent time out reset
	WDTCTL = WDTPW + WDTHOLD;
        display_init();
    m_delay(500);//Es necesario esperar un poco antes de leer los dispositivos I2C
	
#if (HW_IN_USE == USE_RTC)
    
    TACCTL0 = CCIE;             	// CCR0 habilita interrupción del timer
    TACCR0 = 65535 - 1;
    TACTL = TASSEL_1 + MC_1;    // reloj ACLK, modo up to CCR0
    ds1307_init();
    
	d_t.sec = 0x00;
	d_t.min = 0x15;
	d_t.hour = 0x16;
	d_t.day = 0x02;
	d_t.date = 0x28;
	d_t.month = 0x11;
	d_t.year = 0x18;
    
    ret = ds1307_set_time_and_date(&d_t, use_24_hour_mode);
    if(ret == false)
    {
        while(1);
    }
    
#elif (HW_IN_USE == USE_IMU)
    
    TACCTL0 = CCIE;             	// CCR0 habilita interrupción del timer
    TACCR0 = 32768 >> 4;            //FILL HERE. Ajustar para muestrear al doble
                                    //de la frecuencia del ODR (output data rate)de la IMU
    TACTL = TASSEL_1 + MC_1;    // reloj ACLK, modo up to CCR0
    
    mpu9250_simple_init();
    mpu9250_perform_who_ami();
    imu_config.gyr_scale = GYR_SCALE_500_DPS;
    imu_config.acc_scale = ACC_SCALE_2G;
    imu_config.mag_resolution = mag_16_bits;
    imu_config.mpu_odr = ODR_50HZ; //Se toma datos
    imu_config.mag_odr = MAG_ODR_8HZ; 
    ret = mpu9250_simple_config(&imu_config);
    if(ret == false)
    {
        while(1);
    }
    ret = mpu9250_simple_start();
    if(ret == false)
    {
        while(1);
    }
    
#endif
	_EINT();
	_BIS_SR(LPM0_bits);	// modo bajo consumo

  return 0;
}

#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
#if (HW_IN_USE == USE_RTC)
	if(!i2c_is_busy())
	{
		ds1307_get_secs();
	}
#else
    mpu9250_simple_get_data();
    display_set_pos(0x00);
    if(mpu.acc_real_value.ax >= 0.9)
    {
      display_send_data(0x78);
    }
    else if(mpu.acc_real_value.ay >= 0.9)
    {
      display_send_data(0x79);
    }
    else if(mpu.acc_real_value.az >= 0.9)
    {
      display_send_data(0x7A);  
    }
    else
    {
      display_send_data(0x64);
    }
    //display_send_data((char)mpu.acc_real_value.ax);
#endif
}
