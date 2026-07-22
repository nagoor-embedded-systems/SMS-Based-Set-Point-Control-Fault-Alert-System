#include <lpc21xx.h>
#include "uart.h"
#include "gsm.h"
#include "i2c_eeprom.h"
#include "adc.h"
#include "lcd.h"
#include "delay.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>

#define Gas_sensor 5
#define BUZZER_PIN 6
#define slaveaddr 0x50
#define SPADDR 0x0000
#define M_NOADDR 0x0004
#define PASSKEY 1234

// External variables
extern char Mobile_no[11];
extern char msg_rec[60];
extern int position;
extern char buff[100];
extern char r_flag;
extern volatile int buff_pointer;

// Local variables
u8 phone_no_i2c[11];
u8 SP_i2c[3];
u8 old_p_no[11];
u8 sp[3]="35", mn[11]="6305513576";
char passkey[5] ="1234";
int i2c_temp;
int flag;

void init()
{
InitLCD();
IODIR0 |=  (1 << BUZZER_PIN);
    InitUART0();
    gsm_Init();
    init_i2c();
    Init_ADC();
    
}
void changes() {

    if (strstr(msg_rec, "1234")) {
        if (strstr(msg_rec, "T") ) {
            strncpy((char *)sp, msg_rec + 5, sizeof(sp) - 1);
            sp[sizeof(sp) - 1] = '\0'; // Ensure null-termination
            i2c_eeprom_page_write(slaveaddr,SPADDR, sp, 2);
            i2c_eeprom_seq_read(slaveaddr, SPADDR, SP_i2c, 2);
           	send_sms1((char *)mn, "set point changed : ",(char *)sp);
			flag=1;
		/*if(flag == 1)
			{
				  send_sms1((char *)mn, "set point changed");
				  flag=0;
			}*/
		  
        } else if (strstr(msg_rec, "M") ) {
	//	old_p_no=mn;
			strcpy((char *)old_p_no,(char *)mn);
            strncpy((char *)mn, msg_rec + 5, sizeof(mn) - 1);
            mn[sizeof(mn) - 1] = '\0'; // Ensure null-termination
            // Save and load mobile number
		
		
            i2c_eeprom_page_write(slaveaddr, M_NOADDR, mn, 10);
            i2c_eeprom_seq_read(slaveaddr, M_NOADDR, phone_no_i2c, 10);
           
		   	   send_sms1((char *)old_p_no, " changed",(char *)mn);
			flag=2;
			
		   /*if(flag == 1)
		   {
		   send_sms1((char *)mn, "mobile number changed");
		   flag=0;
		   }*/
        }  else {
            CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("Invalid Option");
	delay_ms(1000);
        }
    } else {
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("Invalid Passkey");
	delay_ms(1000);	  
    }
}


int main() {
    
    float voltage, temperature;
    int temp, gas_detect;
    memset(msg_rec, '\0', sizeof(msg_rec));

    init();
	 //send_sms1((char *)"9999999999", "set point changed");
    // Save and load setpoint
   i2c_eeprom_page_write(slaveaddr, SPADDR, sp, 2);
    i2c_eeprom_seq_read(slaveaddr, SPADDR, SP_i2c, 2);

    // Save and load mobile number
   i2c_eeprom_page_write(slaveaddr, M_NOADDR, mn, 10);
    i2c_eeprom_seq_read(slaveaddr, M_NOADDR, phone_no_i2c, 10);

    while (1) {
        if (r_flag == 1) {
            //is_msg_arrived = GSM_Wait_for_Msg();
            //if (is_msg_arrived) {
                delay_ms(1000);
                read_sms();  // Ensure read_sms() is implemented correctly
                delay_ms(2000);
				CmdLCD(0x01);
				CmdLCD(0x80);
				StrLCD(Mobile_no);
				delay_ms(1000);
				delay_ms(2000);
                if (strcmp(Mobile_no, (const char *)mn) == 0) {
                    CmdLCD(0x01);
					CmdLCD(0x80);
					StrLCD("changing..");
					delay_ms(1000);
                    changes();
					if(flag==1)
					{
						send_sms1((char *)mn, "set point changed : ",(char *)sp);
				  		flag=0;		
					}
					else if(flag==2)
					{
						send_sms1((char *)old_p_no, "mobile no changed : ",(char *)mn);
				  		flag=0;		
					}
					 
				
				
                } else {
                    
					CmdLCD(0x01);
					CmdLCD(0x80);
					StrLCD("wrong MN");
					delay_ms(1000);
                }

                delete_msg(); 
						
                delay_ms(1000);
            r_flag = 0;
        }

        voltage = Read_ADC(1);
        temperature = voltage * 100;
        temp = (int)temperature;
        gas_detect = ((IOPIN0 >> Gas_sensor) & 1);
        i2c_temp = atoi((const char *)SP_i2c);
        CmdLCD(0x01);
        StrLCD("T:");
        U32LCD(temp);
        StrLCD(" ");
        StrLCD((char *)phone_no_i2c);
        CmdLCD(0xC0);

        if (gas_detect == 0) {
            StrLCD("Gas:D");
        } else {
            StrLCD("Gas:ND");
        }

        StrLCD(" sp:");
        U32LCD(i2c_temp);
        delay_ms(3000);

        if (temp > i2c_temp) {
            IOSET0 = (1 << BUZZER_PIN);
            send_sms((char *)mn, "Temperature Alert!");
        } else {
            IOCLR0 = (1 << BUZZER_PIN);
        }

       //  memset(Mobile_no, '\0', sizeof(Mobile_no));
      //  memset(msg_rec, '\0', sizeof(msg_rec));
    }
//	while(1);
}
