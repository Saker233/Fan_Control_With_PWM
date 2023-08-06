/* 
 * File:   application.c
 * Author: Mohamed Saker
 * https://www.linkedin.com/in/mohamed-saker-795123228/
 */
#include "application.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer2/hal_timr2.h"
#include "MCAL_Layer/CCP/hal_ccp.h"
void application_intialize(void);

void welcome_massege();


chr_lcd_4bit_t lcd ={
  .lcd_rs.port = PORTD_INDEX,
  .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
  .lcd_rs.pin = GPIO_PIN0,
  .lcd_rs.logic = GPIO_LOW,
  .lcd_en.port = PORTD_INDEX,
  .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
  .lcd_en.pin = GPIO_PIN1,
  .lcd_en.logic = GPIO_LOW,
  .lcd_data[0].port = PORTD_INDEX,
  .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
  .lcd_data[0].pin = GPIO_PIN2,
  .lcd_data[0].logic = GPIO_LOW,
  .lcd_data[1].port = PORTD_INDEX,
  .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
  .lcd_data[1].pin = GPIO_PIN3,
  .lcd_data[1].logic = GPIO_LOW,
  .lcd_data[2].port = PORTD_INDEX,
  .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
  .lcd_data[2].pin = GPIO_PIN4,
  .lcd_data[2].logic = GPIO_LOW,
  .lcd_data[3].port = PORTD_INDEX,
  .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
  .lcd_data[3].pin = GPIO_PIN5,
  .lcd_data[3].logic = GPIO_LOW,
};

adc_conf_t adc = {
  .ADC_InterruptHandler = NULL,
  .acquisition_time = ADC_12_TAD,
  .adc_channel = ADC_CHANNEL_AN0,
  .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
  .result_format = ADC_RESULT_RIGHT,
  .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED  
};

timer2_t timer2 ={
  .TMR2_InterruptHandler = NULL,
  .timer2_prescaler_value =  TIMER2_PRESCALER_DIV_BY_1,
  .timer2_postscaler_value = TIMER2_POSTSCALER_DIV_BY_1,
  .timer2_preload_value = ZERO_INIT,
};

ccp_t ccp1 ={
    .ccp_inst = CCP1_INST,
    .CCP2_InterruptHandler = NULL,
    .ccp_mode = CCP_PWM_MODE_SELECTED,
    .PWM_Frequency = 20000,
    .ccp_pin.port = PORTC_INDEX,
    .ccp_pin.direction = GPIO_DIRECTION_OUTPUT,
    .ccp_pin.pin = GPIO_PIN2,
    .timer2_prescaler_value = CCP_TIMER2_PRESCALER_DIV_BY_1,
    .timer2_postscaler_value = CCP_TIMER2_POSTSCALER_DIV_BY_1
};

ccp_t ccp2 ={
    .ccp_inst = CCP2_INST,
    .CCP2_InterruptHandler = NULL,
    .ccp_mode = CCP_PWM_MODE_SELECTED,
    .PWM_Frequency = 20000,
    .ccp_pin.port = PORTC_INDEX,
    .ccp_pin.direction = GPIO_DIRECTION_OUTPUT,
    .ccp_pin.pin = GPIO_PIN1,
    .timer2_prescaler_value = CCP_TIMER2_PRESCALER_DIV_BY_1,
    .timer2_postscaler_value = CCP_TIMER2_POSTSCALER_DIV_BY_1
};




uint16 lm35_res = 0;
uint16 lm35_res_degree = 0;
uint8 lm35_res_t_text[6];
uint8 PWM1_Duty = 0;
uint8 PWM2_Duty = 0;
int main() { 
    Std_ReturnType ret = E_NOT_OK;
    
    application_intialize();
    
    welcome_massege();
    __delay_ms(100);
    ret = lcd_4bit_send_command(&lcd, _LCD_CLEAR);
    
    ret = lcd_4bit_send_string_pos(&lcd, 1, 3, "Fan : ");
//    __delay_ms(2000);
//    ret = lcd_4bit_send_command(&lcd, _LCD_CLEAR);
    
    ret = lcd_4bit_send_string_pos(&lcd, 2, 1, "Temp : ");
    
    while(1){  
        
       
        ret = ADC_GetConversion_Blocking(&adc, ADC_CHANNEL_AN0, &lm35_res);
        
        lm35_res_degree = lm35_res *0.488;
        
        ret = convert_uint16_to_string(lm35_res_degree, lm35_res_t_text);
        
        ret = lcd_4bit_send_string_pos(&lcd, 2, 9, lm35_res_t_text);
        
        if(lm35_res_degree < 25)
        {
            PWM1_Duty = 0;
            PWM2_Duty = 0;
            ret = lcd_4bit_send_string_pos(&lcd, 1, 8, "OFF");
            
            ret = CCP_PWM_Set_Duty(&ccp1, PWM1_Duty);
            ret = CCP_PWM_Start(&ccp1);
            
            ret = CCP_PWM_Set_Duty(&ccp2, PWM2_Duty);
            ret = CCP_PWM_Start(&ccp2);
            
        }
        else if((lm35_res_degree > 25) && (lm35_res_degree < 35))
        {
            PWM1_Duty = 50;
            PWM2_Duty = 0;
            ret = lcd_4bit_send_string_pos(&lcd, 1, 8, "ON!");
            
            ret = CCP_PWM_Set_Duty(&ccp1, PWM1_Duty);
            ret = CCP_PWM_Start(&ccp1);
            
            ret = CCP_PWM_Set_Duty(&ccp2, PWM2_Duty);
            ret = CCP_PWM_Start(&ccp2);
            
        }
        else if((lm35_res_degree > 35) && (lm35_res_degree < 50))
        {
            PWM1_Duty = 100;
            PWM2_Duty = 0;
            ret = lcd_4bit_send_string_pos(&lcd, 1, 8, "ON!");
            
            ret = CCP_PWM_Set_Duty(&ccp1, PWM1_Duty);
            ret = CCP_PWM_Start(&ccp1);
            
            ret = CCP_PWM_Set_Duty(&ccp2, PWM2_Duty);
            ret = CCP_PWM_Start(&ccp2);
        }
        else if((lm35_res_degree > 50) && (lm35_res_degree < 65))
        {
            PWM1_Duty = 100;
            PWM2_Duty = 50;
            ret = lcd_4bit_send_string_pos(&lcd, 1, 8, "ON!");
            ret = CCP_PWM_Set_Duty(&ccp1, PWM1_Duty);
            ret = CCP_PWM_Start(&ccp1);
            
            ret = CCP_PWM_Set_Duty(&ccp2, PWM2_Duty);
            ret = CCP_PWM_Start(&ccp2);
        }
        else if(lm35_res_degree > 65)
        {
            PWM1_Duty = 100;
            PWM2_Duty = 100;
            ret = lcd_4bit_send_string_pos(&lcd, 1, 8, "ON!");
            ret = CCP_PWM_Set_Duty(&ccp1, PWM1_Duty);
            ret = CCP_PWM_Start(&ccp1);
            
            ret = CCP_PWM_Set_Duty(&ccp2, PWM2_Duty);
            ret = CCP_PWM_Start(&ccp2);
        }
      
    }
    return (EXIT_SUCCESS);
}

void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ecu_layer_intialize();
    ret = lcd_4bit_intialize(&lcd);
    ret = ADC_Init(&adc);
    ret = CCP_Init(&ccp1);
    ret = CCP_Init(&ccp2);
    ret = Timer2_Init(&timer2);
}
void welcome_massege(void){
    Std_ReturnType ret = E_NOT_OK;
    for (int i = 1; i <= 6; i++) {
        ret = lcd_4bit_send_string_pos(&lcd, 1, i, "Hello All");
        ret = lcd_4bit_send_string_pos(&lcd, 2, 2, "Fan Control Proj");
        __delay_ms(100);
        ret = lcd_4bit_send_string_pos(&lcd, 1, i, " ");
    }
    ret = lcd_4bit_send_string_pos(&lcd, 1, 1, "                  ");
    ret = lcd_4bit_send_string_pos(&lcd, 1, 6, "Hello All");
}

