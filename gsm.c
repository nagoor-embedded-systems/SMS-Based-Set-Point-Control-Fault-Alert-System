#include "gsm.h"
#include "lcd.h"
#include <lpc21xx.h>
#include "types.h"
#include "uart.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern char buff[100];
extern unsigned char i;
int position = 0;
unsigned char Mobile_no[13] = {0};
unsigned char msg_rec[32] = {0};
volatile int buff_pointer= 0;
extern char r_flag;


void gsm_Init()
{
    //delay_ms();
	StrLCD("SMS Project");
	CmdLCD(0xC0);
    delay_ms(1000);
	CmdLCD(0x01);
    StrLCD("Gsm initialize");
    CmdLCD(0xC0);
    delay_ms(1000);
    CmdLCD(0x01);
    StrLCD("AT");
    delay_ms(1000);
    UART0_Str("AT\r\n");
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD((s8 *)buff);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0xC0);
        StrLCD("OK");
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    CmdLCD(0x01);
    StrLCD("ATE0");
    delay_ms(1000);
    UART0_Str("ATE0\r\n");
    i = 0;
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD(buff);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0xC0);
        StrLCD("OK");
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    CmdLCD(0x01);
    StrLCD("AT+CMGF=1");
    delay_ms(1000);
    UART0_Str("AT+CMGF=1\r\n");
    i = 0;
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD(buff);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0xC0);
        StrLCD("Initialized GSM");
        delay_ms(1000);
        CmdLCD(0x01);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }
	

	display("AT+CNMI=2,1,0,0,0");
    delay_ms(1000);
    UART0_Str("AT+CNMI=2,1,0,0,0\r\n");
    i = 0;
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD(buff);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0xC0);
        StrLCD("OK");
        delay_ms(1000);
        CmdLCD(0x01);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }
	delete_msg();

}

void send_sms(char *number, char *msg)
{
    delay_ms(100);
    CmdLCD(0x01);
    StrLCD("Sending AlertMsg");
   UART0_Str("AT+CMGS=");
    //delay_ms(50);
    UART0_Tx('"');
    UART0_Str(number);
    UART0_Tx('"');	
    UART0_Str("\r\n");
//	UART0_Str("AT+CMGS=\"6303372710"\"");

   // delay_ms(500);
    UART0_Str(msg);
    delay_ms(100);
    UART0_Tx(0x1A);
    i = 0;
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD(buff);
    delay_ms(500);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0x01);
        StrLCD("Message sent..");
        delay_ms(500);
    }
    else
    {
        CmdLCD(0x01);
        StrLCD("Message sent fail..");
        delay_ms(500);
    }
}

void send_sms1(char *num, char *msg, char *sp)
{
    delay_ms(100);
    CmdLCD(0x01);
    StrLCD("Sending Ack..");
	UART0_Str("\r\n");
   UART0_Str("AT+CMGS=");
    //delay_ms(50);
    UART0_Tx('"');
    UART0_Str(num);
    UART0_Tx('"');	
    UART0_Str("\r\n");
//	UART0_Str("AT+CMGS=\"6303372710"\"");

   // delay_ms(500);
    UART0_Str(msg);
	UART0_Str(sp);
    delay_ms(100);
    UART0_Tx(0x1A);
    i = 0;
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD(buff);
    delay_ms(500);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0x01);
        StrLCD("Message sent..");
        delay_ms(500);
    }
    else
    {
        CmdLCD(0x01);
        StrLCD("Message sent fail..");
        delay_ms(500);
    }
	
}

void read_sms()
{

    
    delay_ms(100);
    CmdLCD(0x01);
    StrLCD("Reading Msg");
    UART0_Str("AT+CMGR=1\r\n");
    i = 0;
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD(buff);
    delay_ms(500);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0x01);
        StrLCD("Message reading..");
        delay_ms(500);
    }
    else
    {
        CmdLCD(0x01);
        StrLCD("Message read failed..");
        delay_ms(500);
    }
    msg_ext_dis_GSM();
}



int GSM_wait(void)
{
    char msg_location[20] = {0};
    int j;
    delay_ms(500);
    buff_pointer = 0;

    /* Skip leading \r or \n characters */
    while (buff[buff_pointer] == '\r' || buff[buff_pointer] == '\n')
    {
        buff_pointer++;
    }

    /* Check for "CMTI:" to confirm a new message */
    if (strstr(buff, "CMTI:"))
    {
        /* Skip to the position marker after ',' */
        while (buff[buff_pointer] != ',')
        {
            buff_pointer++;
        }
        buff_pointer++;

        /* Extract message location */
        for (j = 0; buff[buff_pointer] != '\r' && j < sizeof(msg_location) - 1; j++)
        {
            msg_location[j] = buff[buff_pointer];
            buff_pointer++;
        }
        msg_location[j] = '\0';

        /* Convert extracted location to an integer */
        position = atoi(msg_location);

        /* Clear the buffer */
        memset(buff, 0, sizeof(buff));
        buff_pointer = 0;

        return 1;
    }

    return 0;
}
void delete_msg()
{
	CmdLCD(0x01);
    display("delete Msg");
	delay_ms(1000);
	UART0_Str("AT+CMGD=1\r\n");
    //UART0_Str("AT+CMGD=1")
	//("\r\n");
    i = 0;
    memset(buff, '\0', 100);
    while (i < 4);
    delay_ms(500);
    buff[i] = '\0';
    CmdLCD(0x01);
    StrLCD(buff);
    delay_ms(500);
    if (strstr(buff, "OK"))
    {
        CmdLCD(0x01);
        StrLCD("Message delete..");
        delay_ms(500);
    }
    else
    {
        CmdLCD(0x01);
        StrLCD("Message failed..");
        delay_ms(500);
    }
}
 void msg_ext_dis_GSM()		  
{
    int j = 0;
    int msg_index = 0; // Variable for message storage
    delay_ms(500);  // Delay added for processing

    if (!(strstr(buff, "+CMGR"))) /* Check for +CMGR response */
    {
        StrLCD("No message");
    }
    else
    {
        buff_pointer = 0;

        // Skip initial '\r' and '\n' characters
        while (buff[buff_pointer] == '\r' || buff[buff_pointer] == '\n')
        {
            buff_pointer++;
        }

        /* Search for 1st ',' to get mobile number */
        while (buff[buff_pointer] != ',')
        {
            buff_pointer++;
        }
        buff_pointer = buff_pointer + 4; // Skip ',' and "+91"

        /* Extract mobile number (10 digits only) */
        for (j = 0; j < 10; j++)
        {
            Mobile_no[j] = buff[buff_pointer]; // Store digits
            buff_pointer++;
        }

        Mobile_no[j] = '\0'; // Null-terminate the mobile number

        /* Move to the start of the message */
        while (buff[buff_pointer] != '\n')
        {
            buff_pointer++;
        }
        buff_pointer++; // Skip '\n'

        CmdLCD(0xC0); // Move to second line of LCD
        j = 0;

        /* Display and save the full original message */
        while (buff[buff_pointer] != '\r' && msg_index < 31) // Stop at '\r' or after 31 characters
        {
            if (buff[buff_pointer] >= 32 && buff[buff_pointer] <= 126) // Only display printable ASCII characters
            {
                UART0_Tx(buff[buff_pointer]);       // Transmit the character
                CharLCD(buff[buff_pointer]);       // Display character on LCD
                msg_rec[msg_index] = buff[buff_pointer]; // Save the character
                msg_index++;
                j++;
            }
            buff_pointer++;

            if (j == 16)
                CmdLCD(0x80); /* Move to the first line after 16 characters */
        }

        msg_rec[msg_index] = '\0'; // Null-terminate the stored message

        /* Clear the buffer after processing */
        buff_pointer = 0;
        memset(buff, 0, sizeof(buff)); // Clear the buffer for next use
    }
    r_flag = 0;
}
                 
            



