/**
 * 
 * This is a modified source code from the official Raspberry Pi Pico SDK repo (pico-examples), dealing with a BMP280 temperature and pressure sensor 
 *   
 *
 * March 22, 2026
 *  
 **/

#include <stdio.h>
#include <stdlib.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

// OLED SSD1306 w/ SSD1306 controller
#include "oled.h"
#include "lookupTable.h"

 /*  
    GPIO PICO_DEFAULT_I2C_SDA_PIN  -> GP4
    GPIO PICO_DEFAULT_I2C_SCK_PIN  -> GP5 
 */

 // device has default bus address of 0x76
#define ADDR _u(0x77) // if in any doubt, connect to the Raspberry Pi and type "i2cdetect -y 1" on the shell

// hardware registers
#define REG_CONFIG _u(0xF5)
#define REG_CTRL_MEAS _u(0xF4)
#define REG_RESET _u(0xE0)

#define REG_TEMP_XLSB _u(0xFC)
#define REG_TEMP_LSB _u(0xFB)
#define REG_TEMP_MSB _u(0xFA)

#define REG_PRESSURE_XLSB _u(0xF9)
#define REG_PRESSURE_LSB _u(0xF8)
#define REG_PRESSURE_MSB _u(0xF7)

// calibration registers
// _u is defining and forcing a 32bit unsigned integer literal (literal is just a value like "2", "4001" or whatever)
#define REG_DIG_T1_LSB _u(0x88)
#define REG_DIG_T1_MSB _u(0x89)
#define REG_DIG_T2_LSB _u(0x8A)
#define REG_DIG_T2_MSB _u(0x8B)
#define REG_DIG_T3_LSB _u(0x8C)
#define REG_DIG_T3_MSB _u(0x8D)
#define REG_DIG_P1_LSB _u(0x8E)
#define REG_DIG_P1_MSB _u(0x8F)
#define REG_DIG_P2_LSB _u(0x90)
#define REG_DIG_P2_MSB _u(0x91)
#define REG_DIG_P3_LSB _u(0x92)
#define REG_DIG_P3_MSB _u(0x93)
#define REG_DIG_P4_LSB _u(0x94)
#define REG_DIG_P4_MSB _u(0x95)
#define REG_DIG_P5_LSB _u(0x96)
#define REG_DIG_P5_MSB _u(0x97)
#define REG_DIG_P6_LSB _u(0x98)
#define REG_DIG_P6_MSB _u(0x99)
#define REG_DIG_P7_LSB _u(0x9A)
#define REG_DIG_P7_MSB _u(0x9B)
#define REG_DIG_P8_LSB _u(0x9C)
#define REG_DIG_P8_MSB _u(0x9D)
#define REG_DIG_P9_LSB _u(0x9E)
#define REG_DIG_P9_MSB _u(0x9F)

void OLED_showDigits(uint32_t digit, uint32_t a, uint32_t b, uint32_t* c , uint32_t* d)
{
    struct render_area frame_area = {a, b, *c, *d};
    calc_render_area_buflen(&frame_area);
    
    if (digit == 0)
        render(null, &frame_area); 
    else if (digit == 1)
        render(one, &frame_area);
    else if (digit == 2)
        render(two, &frame_area);
    else if (digit == 3)
        render(three, &frame_area);
    else if (digit == 4)
        render(four, &frame_area);
    else if (digit == 5)
        render(five, &frame_area);
    else if (digit == 6)
        render(six, &frame_area);
    else if (digit == 7)
        render(seven, &frame_area);
    else if (digit == 8)
        render(eight, &frame_area);
    else if (digit == 9)
        render(nine, &frame_area);        
    
}

void renderTemperature(uint32_t temperature, uint32_t c, uint32_t d){      
            
    uint32_t interimValue;
    uint32_t digit[4];
    
    digit[0] = temperature/1000;
    digit[1] = (temperature - (1000*digit[0]))/100;  
    interimValue = temperature/100;
    digit[2] = (temperature - interimValue*100)/10;
    interimValue = temperature/10;
    digit[3] = temperature - interimValue*10;
    
    for (int i=0; i < 2; i++)
    {          
      OLED_showDigits(digit[i], (40+(i*8)), (47+(i*8)), &c, &d);
    }
    
    for (int i=0; i < 2; i++)
    {          
      OLED_showDigits(digit[i+2], (64+(i*8)), (71+(i*8)), &c, &d);
    }
    
    // show minus if necessary
    struct render_area frame_area = {32, 39, 2, 2};
    calc_render_area_buflen(&frame_area); 
    
    int32_t negativeTempTrue = (temperature & 0x80000000) >> 31;
        
    if (negativeTempTrue == 1)           
        render(minus, &frame_area);             
    else if (negativeTempTrue == 0)  
        render(clr, &frame_area); 
            
}

void renderPressure(uint32_t pressure, uint32_t c, uint32_t d){ 
    
    uint32_t interimValue;
    uint32_t digit[6];
    
    digit[0] = pressure/100000;
    digit[1] = (pressure - (100000*digit[0]))/10000;  
    interimValue = pressure/10000;
    digit[2] = (pressure - interimValue*10000)/1000;
    interimValue = pressure/1000;
    digit[3] = (pressure - interimValue*1000)/100;    
    interimValue = pressure/100;
    digit[4] = (pressure - interimValue*100)/10;
    interimValue = pressure/10;
    digit[5] = pressure - interimValue*10;
                 
    OLED_showDigits(digit[0], 24, 31, &c, &d);    
    
    for (int i=0; i < 5; i++)
    {          
      OLED_showDigits(digit[i+1], (40+(i*8)), (47+(i*8)), &c, &d);
    }
            
}

// number of calibration registers to be read
#define NUM_CALIB_PARAMS 24

struct bmp280_calib_param {
    // temperature params
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    // pressure params
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
};

#ifdef i2c_default
void bmp280_init() {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // 500ms sampling time, x16 filter
    const uint8_t reg_config_val = ((0x04 << 5) | (0x05 << 2)) & 0xFC;

    // send register number followed by its corresponding value
    buf[0] = REG_CONFIG;
    buf[1] = reg_config_val;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // osrs_t x1, osrs_p x4, normal mode operation
    const uint8_t reg_ctrl_meas_val = (0x01 << 5) | (0x03 << 2) | (0x03);
    buf[0] = REG_CTRL_MEAS;
    buf[1] = reg_ctrl_meas_val;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void bmp280_read_raw(int32_t* temp, int32_t* pressure) {
    // BMP280 data registers are auto-incrementing and we have 3 temperature and
    // pressure registers each, so we start at 0xF7 and read 6 bytes to 0xFC
    // note: normal mode does not require further ctrl_meas and config register writes

    uint8_t buf[6];
    uint8_t reg = REG_PRESSURE_MSB;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 6, false);  // false - finished with bus

    // store the 20 bit read in a 32 bit signed integer for conversion
    *pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
    *temp = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
}

void bmp280_reset() {
    // reset the device with the power-on-reset procedure
    uint8_t buf[2] = { REG_RESET, 0xB6 };
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

// intermediate function that calculates the fine resolution temperature
// used for both pressure and temperature conversions
int32_t bmp280_convert(int32_t temp, struct bmp280_calib_param* params) {
    // use the 32-bit fixed point compensation implementation given in the
    // datasheet
    
    int32_t var1, var2;
    var1 = ((((temp >> 3) - ((int32_t)params->dig_t1 << 1))) * ((int32_t)params->dig_t2)) >> 11;
    var2 = (((((temp >> 4) - ((int32_t)params->dig_t1)) * ((temp >> 4) - ((int32_t)params->dig_t1))) >> 12) * ((int32_t)params->dig_t3)) >> 14;
    return var1 + var2;
}

int32_t bmp280_convert_temp(int32_t temp, struct bmp280_calib_param* params) {
    // uses the BMP280 calibration parameters to compensate the temperature value read from its registers
    int32_t t_fine = bmp280_convert(temp, params);
    return (t_fine * 5 + 128) >> 8;
}

int32_t bmp280_convert_pressure(int32_t pressure, int32_t temp, struct bmp280_calib_param* params) {
    // uses the BMP280 calibration parameters to compensate the pressure value read from its registers

    int32_t t_fine = bmp280_convert(temp, params);

    int32_t var1, var2;
    uint32_t converted = 0.0;
    var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)params->dig_p6);
    var2 += ((var1 * ((int32_t)params->dig_p5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)params->dig_p4) << 16);
    var1 = (((params->dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)params->dig_p2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)params->dig_p1)) >> 15);
    if (var1 == 0) {
        return 0;  // avoid exception caused by division by zero
    }
    converted = (((uint32_t)(((int32_t)1048576) - pressure) - (var2 >> 12))) * 3125;
    if (converted < 0x80000000) {
        converted = (converted << 1) / ((uint32_t)var1);
    } else {
        converted = (converted / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)params->dig_p9) * ((int32_t)(((converted >> 3) * (converted >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(converted >> 2)) * ((int32_t)params->dig_p8)) >> 13;
    converted = (uint32_t)((int32_t)converted + ((var1 + var2 + params->dig_p7) >> 4));
    return converted;
}

void bmp280_get_calib_params(struct bmp280_calib_param* params) {
    // raw temp and pressure values need to be calibrated according to
    // parameters generated during the manufacturing of the sensor
    // there are 3 temperature params, and 9 pressure params, each with a LSB
    // and MSB register, so we read from 24 registers

    uint8_t buf[NUM_CALIB_PARAMS] = { 0 };
    uint8_t reg = REG_DIG_T1_LSB;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    // read in one go as register addresses auto-increment
    i2c_read_blocking(i2c_default, ADDR, buf, NUM_CALIB_PARAMS, false);  // false, we're done reading

    // store these in a struct for later use
    params->dig_t1 = (uint16_t)(buf[1] << 8) | buf[0];
    params->dig_t2 = (int16_t)(buf[3] << 8) | buf[2];
    params->dig_t3 = (int16_t)(buf[5] << 8) | buf[4];

    params->dig_p1 = (uint16_t)(buf[7] << 8) | buf[6];
    params->dig_p2 = (int16_t)(buf[9] << 8) | buf[8];
    params->dig_p3 = (int16_t)(buf[11] << 8) | buf[10];
    params->dig_p4 = (int16_t)(buf[13] << 8) | buf[12];
    params->dig_p5 = (int16_t)(buf[15] << 8) | buf[14];
    params->dig_p6 = (int16_t)(buf[17] << 8) | buf[16];
    params->dig_p7 = (int16_t)(buf[19] << 8) | buf[18];
    params->dig_p8 = (int16_t)(buf[21] << 8) | buf[20];
    params->dig_p9 = (int16_t)(buf[23] << 8) | buf[22];
}

#endif

int main() {
	
	// reducing the power consumption by reducing clk_sys, we do not need a high system clock for this use case
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);

    // Turn off PLL sys for good measure
    pll_deinit(pll_sys);

    // CLK peri is clocked from clk_sys so need to change clk_peri's freq
    clock_configure(clk_peri,
                    0,
                    CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,
                    48 * MHZ,
                    48 * MHZ);
	
    stdio_init_all();  
    
    i2c_init(i2c_default, 50 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    //gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    //gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    // graphic scaffold, start screen
    oled_init();
    struct render_area title_area = {0, 127, 0, 4};
    calc_render_area_buflen(&title_area);
    render(startScreen, &title_area);
    
    // configure BMP280
    bmp280_init();

    // retrieve fixed compensation params
    struct bmp280_calib_param params;
    bmp280_get_calib_params(&params);

    int32_t raw_temperature;
    int32_t raw_pressure;

    sleep_ms(250); // sleep so that data polling and register update don't collide
    while (1) {
		
        bmp280_read_raw(&raw_temperature, &raw_pressure); // call by reference
        
        int32_t temperature = bmp280_convert_temp(raw_temperature, &params);
        int32_t pressure = bmp280_convert_pressure(raw_pressure, raw_temperature, &params);        
                
        renderTemperature(temperature, 2, 2);
        renderPressure(pressure, 3, 3);
        
         /*
        printf("Pressure = %.5f bar\n", pressure / 100000.f);
        printf("Temp. = %.2f C\n", temperature / 100.f);
       
        printf("Pressure = %d \n", pressure);
        printf("Temp. = %d \n", temperature);        
        */                
        
        // poll
        sleep_ms(1000);
    }

}
