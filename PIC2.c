#include <16F877a.h>
#fuses HS, NOLVP, NOPROTECT, NOWDT
#use delay(clock=16000000) 
#use i2c(Master,Fast=100000,sda=pin_c4,scl=pin_c3,force_sw) // Initialize I2C
#define MFRC522_CS         PIN_D3                 
#define MFRC522_SCK        PIN_D2
#define MFRC522_SI         PIN_C2                           
#define MFRC522_SO         PIN_D0              
#define MFRC522_RST        PIN_C1
#include <lcd_i2c.c> //lcd_i2c
#include<rfid.h>
char Home1[4] ={0xF3,0xEA,0x55,0x4F};
char Home2[4] ={0x13,0x7C,0xCE,0xA6};
int THE_1=0,i;
char card_1, card_2;
int1 fire=1;
char enter_password[16]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
char password_default[16]={'0','5','1','1','#','0','0','0','0','0','0','0','0','0','0','0'};

int1 scan(char DATA[],char UID[])
{
   for (int i = 0; i < 4; i++)
   {
      if(UID[i]== DATA[i] )
      {
         THE_1=1;
      }
      else
      {
         THE_1=0;
         break;
      }
   }
   return THE_1;
}
char keypad()
{  
   //col1
   output_low(pin_C0);
   output_high(pin_C5);
   output_high(pin_C6);
   output_high(pin_C7);
   if(input(pin_B4)==0) return '7';
   else if(input(pin_B5)==0) return '4';
   else if(input(pin_B6)==0) return '1';
   else if(input(pin_B7)==0) return '*';
   
   //Col2
   output_high(pin_C0);
   output_low(pin_C5);
   output_high(pin_C6);
   output_high(pin_C7);
   if(input(pin_B4)==0) return '8';
   else if(input(pin_B5)==0) return '5';
   else if(input(pin_B6)==0) return '2';
   else if(input(pin_B7)==0) return '0';
   
   //Col3
   output_high(pin_C0);
   output_high(pin_C5);
   output_low(pin_C6);
   output_high(pin_C7);
   if(input(pin_B4)==0) return '9';
   else if(input(pin_B5)==0) return '6';
   else if(input(pin_B6)==0) return '3';
   else if(input(pin_B7)==0) return '#';
   
   //Col4
   output_high(pin_C0);
   output_high(pin_C5);
   output_high(pin_C6);
   output_low(pin_C7);
   if(input(pin_B4)==0) return 'A';
   else if(input(pin_B5)==0) return 'B';
   else if(input(pin_B6)==0) return 'C';
   else if(input(pin_B7)==0) return 'D';
   
   return 0xff;
}

char key_debouncing() //chong doi nut nhan
{
   char key;
   do
   {
      key = keypad(); //key = 'A'
   }
   while(key==0xff); // doi nguoi dung nhap va giu phim
   while(keypad()!=0xff); //doi cho toi khi nha phim keypad() = oxff
   return key;
}

void enter_pw()
{
   for(i=0;i<16;i++)
   {
      enter_password[i] = key_debouncing();
      if(enter_password[i]=='#') break;
      lcd_gotoxy(i+1,2);
      printf(lcd_putc,"%c",enter_password[i]);
   }
}

void empty_pw()
{
   for(i=0;i<16;i++)
   {
      enter_password[i] = '0';
   }
}

int compare()
{
   for(i=0;i<16;i++)
   {
      if(password_default[i] != enter_password[i]) return 0; // sai mat khau
   }
   return 1; //dung mat khau
}

void open()
{
   delay_ms(5000);
}
void close()
{
   delay_ms(1000);
}
void main()
{
   CHAR UID[4];
   UNSIGNED int TagType;
   lcd_init();
   set_tris_b(0);
   port_b_pullups(true);
   MFRC522_Init();
   delay_ms(100);
   while(TRUE)
   {      
      fire=input(pin_E0);
      output_high(pin_B3);
      lcd_clear();      
      lcd_gotoxy(1,1);
      printf(lcd_putc,"* to enterPW");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"# scanTAG");
      char check = key_debouncing(); 
      if(check=='#')
      {     
      lcd_clear();
      lcd_gotoxy(3,1);
      printf(LCD_PUTC,"Please scan");
      delay_ms(3000);
      //Scanning RFID Tag
      if (MFRC522_isCard (&TagType)) //Check any card
      {                                           
         //Read ID 
         if (MFRC522_ReadCardSerial (&UID))             
         {
            card_1 = scan(Home1,UID);
            card_2 = scan(Home2,UID);
          if(card_1 == 1)
            {                
               lcd_clear();
               lcd_gotoxy(2,1);
               printf(lcd_putc,"DOOR UNLOCKED");
               open();
               delay_ms(2000);
               close();
            }
          if(card_2 == 1)
            {               
               lcd_clear();
               lcd_gotoxy(2,1);
               printf(lcd_putc,"DOOR UNLOCKED");
               open();
               delay_ms(2000);
               close();
            }
          else
          {
               lcd_clear();
               lcd_gotoxy(5,1);
               printf (LCD_PUTC,"TRY AGAIN");
               delay_ms(2000);
          }
         }
      }
      }
      if(check=='*')
      {
         lcd_putc('\f');
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Enter password:");
         enter_pw();
         if(compare()==1)
         {
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Correct password");
            lcd_clear();
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Door Unlocked");
            empty_pw();
            delay_ms(2000);
            lcd_putc('\f');
            open();
         }
         else if(compare()==0)
         {
            lcd_clear();
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Try Again");
            empty_pw();
            delay_ms(2000);
         }
         
      }
   }
      delay_ms(1000);
   }
      

