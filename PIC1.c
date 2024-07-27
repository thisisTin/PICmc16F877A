#include <16F877a.h>
#device ADC=10
#fuses HS, NOLVP, NOPROTECT, NOWDT
#use delay(clock=16000000) 
#use i2c(Master,Fast=100000,sda=pin_c4,scl=pin_c3,force_sw) // Initialize I2C
#include <lcd_i2c.c> //lcd_i2c
unsigned int8 tem;
int1 fire=0,gas=0;
int1 menu;
int1 lastmenu=1;
int counter;
//Counting PUSHBUTTON
void count()
{
      if(menu==0)
         {
          if(menu != lastmenu)
            {
             counter++;  
            }
          else  
         {
             //do nothing
         }
       lastmenu=menu;  
         }
}



void main()
{  
   set_tris_b(0x00);
   set_tris_d(0x00);
   //setup LCD
   lcd_init();
   lcd_clear();     
  //setup ANALOG
   int8 tem;
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0_VREF_VREF);
   set_adc_channel(0);
   delay_us(10);
  
   while(TRUE)
   {
      int rawinput;
      menu=input(pin_E2);
      fire=input(pin_E0);
      rawinput = read_adc();
      tem=(rawinput*0.488);
      gas=input(pin_E1);
      if(fire==0 && gas==1)
      {
      lcd_clear();
      lcd_gotoxy(7,1);
      printf(lcd_putc,"FIRE");
      lcd_gotoxy(4,2);
      printf(lcd_putc,"LEAVE NOW");
      delay_ms(1000);
      }
      if(fire==1 && gas==0)
      {
      lcd_clear();
      lcd_gotoxy(3,1);
      printf(lcd_putc,"Gas Leaking");
      lcd_gotoxy(4,2);
      printf(lcd_putc,"LEAVE NOW");
      delay_ms(1000);
      }
      if(fire==0 && gas==0)
      {
      lcd_clear();
      lcd_gotoxy(3,1);
      printf(lcd_putc,"FIRE+GAS LEAK");
      lcd_gotoxy(4,2);
      printf(lcd_putc,"LEAVE NOW");
      delay_ms(1000);
      }
      if(fire==1 && gas==1)
      {      
      if(menu==0)
      {      
      lcd_clear();
      lcd_gotoxy(1,1);
      printf(lcd_putc,"Temperature");
      lcd_gotoxy(4,2);
      printf(lcd_putc,"%d%d Cel",tem/10,tem%10);
      delay_ms(1000);
      }
      if(menu==1)
      {
      lcd_clear();
      lcd_gotoxy(4,1);
      printf(lcd_putc,"Xin Chao");
      lcd_gotoxy(2,2);
      printf(lcd_putc,"42101352-Tin");
      delay_ms(1000);
      }
      }
      //TODO: User Code
      }
   }
   

