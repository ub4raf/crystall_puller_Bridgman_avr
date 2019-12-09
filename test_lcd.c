/*Chip type               : ATmega168PA
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/
#include <mega168a.h>
#include <alcd.h>
#include <delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "inits.c"

#define STEP_MM             (200/5)
#define MICROSTEP           (16*2)

#define GROWTH_LENGTH_MM    200
//#define MIX_LENGTH          
#define GROWTH_LENGTH       (GROWTH_LENGTH_MM*STEP_MM*MICROSTEP)

enum STEPS {STOP,START,DOWN_MIX,UP_MIX,DOWN_MIX2,UP_MIX2,GROWTH,WAIT};
//eeprom
enum STEPS step_program;//=STOP;

struct prg_step
    {
        char name[8];//={"step_n"};
        unsigned char delay;
        unsigned int period;
        signed int steps;//
    } ;   

//eeprom struct prg_step program[10];
//volatile char program_step=0;
volatile unsigned int counter_=10;
volatile unsigned char speed_counter;
volatile signed long int position_=0;

// Declare your global variables here
//signed char     encoder =0;
//unsigned char   button  =0;

//eeprom 
volatile unsigned int i=(STEP_MM*5*32/MICROSTEP)/2;
//2mm/h == (double)(STEP_MM*5*32/MICROSTEP)/(double)i;
//  i*2=
            // delay in growth mode
            
//eeprom volatile char stop_flag;
//eeprom 
//eeprom 
volatile signed long int position_program=0;


// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
EIMSK=0x00;
if(PINC.2)
if(!PIND.2)
            {
                //PORTB^=1<<5; 
                if(PIND.3)
                    {
                        if(!position_)
                        {
                        if(PINB.3) 
                            { if (i>4)i=i+i/4;
                                else i++;
                            }
                        else
                            { if (i>4)i=i-i/4;
                                else if(i>1)i--;
                            } 
                        }
                    }
                else                 
                    {
                        speed_counter=16;
                        if(!PINB.3) position_+=STEP_MM*MICROSTEP;
                        else position_-=STEP_MM*MICROSTEP;
                    }  
                counter_=1;
            }
            
EIMSK=0x03;
EIFR=3;
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
EIMSK=0x00;
if(!PIND.3)
    {
       //PORTB^=1<<5;
       //i=-i;  
       // Timer/Counter 1 Interrupt(s) initialization
       //TIMSK1^=0x01;
       
       position_program=0;
       switch(step_program)
            {
                case STOP:
                step_program=START;
                break;
                
                case START:
                case DOWN_MIX:
                case UP_MIX:  
                
                counter_=0xFFFF;
                step_program=WAIT;
                break;
                
                case WAIT:
                step_program=GROWTH;
                break;
                
                //case GROWTH:
                //step_program=STOP;
                //break;
                
                default:
                step_program=STOP;
                break;                        
            }
       counter_=1;
       //TCCR1B^=0x03;
    }
EIMSK=0x03;
EIFR=3;
}

// Pin change 0-7 interrupt service routine
interrupt [PC_INT0] void pin_change_isr0(void)
{
// Place your code here
//PORTB^=1<<5;
}


// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
// Reinitialize Timer2 value
  char lcd_string[32];
TCNT2=0x00;
// Place your code here
//PORTB^=1<<5;
            if(position_)
                {
                    lcd_clear();
                    lcd_gotoxy(0,1); 
                    sprintf(lcd_string,"%d",position_);//(double)(STEP_MM*MICROSTEP));
                    lcd_puts(lcd_string);
                    lcd_gotoxy(0,0); 
                    sprintf(lcd_string,"%c%.2fmm",(position_<0)?'^':'v',(position_/(double)(STEP_MM*MICROSTEP)));
                    lcd_puts(lcd_string);
                }
            else  
                if(PINC.3)
                    {
                        lcd_clear();
                        lcd_gotoxy(0,0);             
                        lcd_puts("LIMITER"); 
                        lcd_gotoxy(0,1);             
                        lcd_puts("Pull up!");
                    }
                else
                {
                    lcd_clear();
                    lcd_gotoxy(0,0);             
                    //PORTB.5=PIND.2;
                    //if(TIMSK1&(0x01))
                    //if (TCCR1B&0x03)    //sprintf(lcd_string,"%.2fHz",((double)5*16/(double)i)); 
                    //if(step_program==STOP)  sprintf(lcd_string,"STOP, v=");
                        //else                sprintf(lcd_string,"%.2fmm",(position_program/(double)(STEP_MM*MICROSTEP))); 
                    switch(step_program)
                        {
                            case DOWN_MIX:
                            case DOWN_MIX2:   
                                sprintf(lcd_string,"v%.2fmm",(position_program/(double)(STEP_MM*MICROSTEP)));
                            break;
                            case UP_MIX: 
                            case UP_MIX2:
                                sprintf(lcd_string,"^%.2fmm",(position_program/(double)(STEP_MM*MICROSTEP)));
                            break;  
                            case WAIT: 
                                sprintf(lcd_string,"%d",counter_);
                            break;
                            case GROWTH:     
                                sprintf(lcd_string,"%.2fmm",(position_program/(double)(STEP_MM*MICROSTEP)));
                            break;
                            default: 
                                sprintf(lcd_string,"STOP, v=");                               
                            break;    
                        };
                    lcd_puts(lcd_string);
                    lcd_gotoxy(0,1);             
                    
                    sprintf(lcd_string,"%.2fmm/h",((double)(STEP_MM*5*32/MICROSTEP)/(double)i));
                    lcd_puts(lcd_string); 
                }
}

// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=0xF9E6 >> 8;  // c2f6
TCNT1L=0xF9E6 & 0xff;
//counter_--;
if(!position_)
switch(step_program)
    {
        case START:
            step_program=DOWN_MIX;            
        break;
        case DOWN_MIX:   
            //if(!(counter_--))
                {
                    //counter_=i;//60;
                    dir_set(1);
                    step_change();
                    position_program++;    
                    PORTB^=1<<5;
                }
            if(position_program>GROWTH_LENGTH)                 
                step_program=UP_MIX;        
        break;
        case UP_MIX: 
            //if(!(counter_--))
                {   
                    dir_set(0);
                    //counter_=i;//60;
                    step_change();
                    position_program--;    
                    PORTB^=1<<5;
                }
            if(position_program<=0)
                {   
                    step_program=DOWN_MIX2;
                    //step_program=WAIT;
                    //counter_=0x0FFF;
                }
                
        break;   
        case DOWN_MIX2:   
            //if(!(counter_--))
                {
                    //counter_=i;//60;
                    dir_set(1);
                    step_change();
                    position_program++;    
                    PORTB^=1<<5;
                }
            if(position_program>GROWTH_LENGTH)                 
                step_program=UP_MIX2;        
        break;
        case UP_MIX2: 
            //if(!(counter_--))
                {   
                    dir_set(0);
                    //counter_=i;//60;
                    step_change();
                    position_program--;    
                    PORTB^=1<<5;
                }
            if(position_program<=0)
                {
                    step_program=WAIT;
                    counter_=0x0FFF;
                }
                
        break; 
        case WAIT: 
            if((counter_--))
                {   
                    //dir_set(0);
                    //counter_=i;//60;
                    //step_change();
                    //position_program--;    
                    PORTB^=1<<5;
                }
            else
                {   
                    counter_=1;
                    step_program=GROWTH;
                }
        break;
        case GROWTH:     
            if(!(counter_--))
                {
                    dir_set(1);
                    counter_=i;//60;
                    step_change();
                    position_program++;    
                    PORTB^=1<<5;
                }
            //if(PINC.2)
            if(position_program>GROWTH_LENGTH)
                step_program=STOP;
        break;
        default:
            step_program=STOP;
        break;    
    };
// Place your code here
}

//// Watchdog timeout interrupt service routine
//interrupt [WDT] void wdt_timeout_isr(void)
//{
//// Place your code here
//            
//            //WDTCSR |= (1<<WDIE);
//            #asm("wdr")
//            /* Write logical one to WDCE and WDE */
//            WDTCSR |= (1<<WDCE) | (1<<WDE);
//            //WDTCSR=0x39;
//            //WDTCSR=0x69;
//}

void main(void)
{
// Declare your local variables here
   //signed char i; 
   //char lcd_string[2][9];
// Crystal Oscillator division factor: 1


#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;


// Alphanumeric LCD initialization
// Connections specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 0
// RD - PORTB Bit 1
// EN - PORTB Bit 2
// D4 - PORTD Bit 4
// D5 - PORTD Bit 5
// D6 - PORTD Bit 6
// D7 - PORTD Bit 7
// Characters/line: 16
lcd_init(16);
DDRB  |= 1<<5;

DDRB  &=0b11110111; //3  dir
DDRD  &=0b11110011; //3  button_int, 2 int
PORTB |=0b00001000;
PORTD |=0b00001100;

step_init();
dir_init();


// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Falling Edge
// INT1: On
// INT1 Mode: Falling Edge
// Interrupt on any change on pins PCINT0-7: Off
// Interrupt on any change on pins PCINT8-14: Off
// Interrupt on any change on pins PCINT16-23: Off
EICRA=0x0A;
EIMSK=0x03;
EIFR=0x03;
PCICR=0x00;

//EICRA=0x00;
//EIMSK=0x00;
//PCICR=0x00;


// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 15,625 kHz
// Mode: Normal top=0xFF
// OC2A output: Disconnected
// OC2B output: Disconnected
ASSR=0x00;
TCCR2A=0x00;
TCCR2B=0x07;
TCNT2=0x69;
OCR2A=0x00;
OCR2B=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 15,625 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x03;//250khz   //5;15.625
TCNT1H=0xC2;
TCNT1L=0xF6;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

//// Watchdog Timer initialization
//// Watchdog Timer Prescaler: OSC/512k
//// Watchdog Timer interrupt: On
//#pragma optsize-
//#asm("wdr")
//WDTCSR=0x38;
//WDTCSR=0x68;
//#ifdef _OPTIMIZE_SIZE_
//#pragma optsize+
//#endif

//// Watchdog Timer initialization
//// Watchdog Timer Prescaler: OSC/1024k
//// Watchdog Timer interrupt: On
//#pragma optsize-
//#asm("wdr")
//WDTCSR=0x39;
//WDTCSR=0x69;
//#ifdef _OPTIMIZE_SIZE_
//#pragma optsize+
//#endif

DDRC.1=1;
DDRC.2=0;
PORTC.1=0;
PORTC.2=1;

// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=0x01;
// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=0x01;

dir_set(1);
// Global enable interrupts
#asm("sei")

while (1)
      {     
             
            delay_ms(50);
            if(PINC.3)  step_program=STOP;//TIMSK1&=~(0x01);  // если концевик - вџрубать программу
            //else        //TIMSK1|=0x01; 
            if(PINC.2)                                        // если нажатие  - крутить
                 {
                    EICRA=0x0A;
                    EIMSK=0x03;
                    EIFR=0x03;
                    PCICR=0x00;
                    if(position_)
                        {  
                            step_program=STOP;
                            //TIMSK1&=~(0x01);
                            //if(position_>0)
                                {  
                                    //dir_set(1);
                                    //PORTB^=1<<5;
                                    speed_counter=26;
                                    while(position_)
                                        {  
                                            while(!PIND.3)speed_counter=26;
                                            if(position_>0)
                                                    {
                                                        position_--;
                                                        dir_set(1);
                                                    }
                                                else 
                                                    {
                                                        position_++;
                                                        dir_set(0);
                                                    }
                                            step_change();
                                            delay_ms(speed_counter); 
                                            PORTB^=1<<5;
                                            if(speed_counter>1) speed_counter--;                                                                                   
                                        }                                         
                                }
                            step_program=STOP;
                                               
                        //dir_set(1);
                        }
                     
                     
                } 
            else
                {
                    EICRA=0x00;
                    EIMSK=0x00;
                    EIFR=0x00;
                    PCICR=0x00;
                    //step_program=GROWTH;
                }
      }
}
