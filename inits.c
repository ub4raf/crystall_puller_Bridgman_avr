


#define     LED0                  0b00100000//BIT0 // BIT6 // Green -> button
//#define     LED1                  //BIT0 // Red
#define     LED_DIR               DDRB
#define     LED_OUT               PORTB

//#define     LED_LCD               0 // BIT5
//#define     BUZZER                0 // BIT4

//#define     BUTTON                (BIT3)//|BIT4|BIT5|BIT6|BIT7)
//#define     BUTTON_OUT            P1OUT
//#define     BUTTON_DIR            P1DIR
//#define     BUTTON_IN             P1IN
//#define     BUTTON_IE             P1IE
//#define     BUTTON_IES            P1IES
//#define     BUTTON_IFG            P1IFG
//#define     BUTTON_REN            P1REN


#define     DIR_DIR               DDRC          // P1DIR   pb4
#define     DIR_OUT               PORTC         // P1OUT
#define     DIR                   0b00100000//0b00010000    //

#define     STEP_DIR               DDRC         //P1DIR     pb3
#define     STEP_OUT               PORTC        //P1OUT
#define     STEP                  0b00010000// 0b00001000   //BIT0

#define     EN_DIR                  DDRB         //P1DIR     pb3
#define     EN_OUT                  PORTB        //P1OUT
#define     EN                      0b00000100   //BIT0




inline void dir_init(void)
{
  DIR_DIR |= DIR;                          
  DIR_OUT &= ~(DIR);
}

inline void dir_set(char state)
{
  if(!state)
      DIR_OUT|=DIR;
    else
      DIR_OUT&=~DIR;
}
inline void en_init(void)
{
  EN_DIR |= EN;                          
  EN_OUT |=(EN);
}

inline void en_set(char state)
{
  if(state)
      EN_OUT|=EN;
    else
      EN_OUT&=~EN;
}


inline void dir_change(void)
{
    DIR_OUT^=DIR;
}

inline void step_init(void)
{
  STEP_DIR |= STEP;                          
  STEP_OUT &= ~(STEP);
}

inline char dir_get()
{
return (DIR_OUT&DIR);
}

inline void step_change(void)
{
    STEP_OUT^=STEP;
}

inline void step_set(int state)
{
    if(state)
      STEP_OUT|=STEP;
    else
      STEP_OUT&=~STEP;
}

 inline void stepper_init (void)
 {
 dir_init();
 //dir_set(0);
 step_init();
 //step_set(0);
 en_init(); 
 }

void InitializeLeds(void)
{
  LED_DIR |= LED0;// | LED1 | LED_LCD | BUZZER;                          
  LED_OUT &= ~(LED0);// | LED1 | LED_LCD);  
}
//void LED1_set(int state)
//  {
//    /*if(state)
//      LED_OUT|=LED1;
//    else
//      LED_OUT&=~LED1;
//    */
//  }
void LED0_set(int state)
  {
    
    if(state)
      LED_OUT|=LED0;
    else
      LED_OUT&=~LED0;
    
  }  
 
 inline void LED0_change(void)
{
    LED_OUT^=LED0;
}
  
//void LED_LCD_set(int state)
//  {
//    /*
//    if(state)
//      LED_OUT|=LED_LCD;
//    else
//      LED_OUT&=~LED_LCD;
//    */
//  }

//void buzzer_set(int state)
//  {
//    
//    /*
//    if(state)
//      LED_OUT|=BUZZER;
//    else
//      LED_OUT&=~BUZZER;
//    */
//  }

//
//inline void InitializeButton(void)                 // Configure Push Button 
//{
//  BUTTON_DIR &= ~BUTTON;
//  BUTTON_OUT |= BUTTON;
//  BUTTON_REN |= BUTTON;
//  //BUTTON_IES |= BUTTON;
//  //BUTTON_IFG &= ~BUTTON;
//  //BUTTON_IE |= BUTTON;
//}

//inline char get_button (void)                 // Configure Push Button 
//{
//  return (BUTTON_IN&BUTTON);
//  //BUTTON_DIR &= ~BUTTON;
//  //BUTTON_OUT |=  BUTTON;
//  //BUTTON_REN |= BUTTON;
//  //BUTTON_IES |= BUTTON;
//  //BUTTON_IFG &= ~BUTTON;
//  //BUTTON_IE |= BUTTON;
//}