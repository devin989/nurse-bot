#ifndef F_CPU
#define F_CPU 16000000UL //16000000UL
#endif
#include <avr/pgmspace.h>
#include <stdlib.h>


#include "lib/usart/usart_avr.c"

#include <avr/io.h>
#include <util/delay.h>
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7// initialize ports for lcd
#define B PORTB
//#define pwmRight OCR1A
//#define pwmLeft OCR1B

#define digitalWrite(port, bit, state) \
  if(state == 1){ \
    (port) |= (1 << (bit)); \
  } \
  else if(state == 0){ \
    (port) &= ~(1 << (bit)); \
  }

int s1, s2, s3, s4, s5 ;
void initPWM();
void LMN();
void LMS();
void LMF();
void LMB();
void RMS();
void RMN();
void RMF();
void RMB();// initialize functions for turning the motors

void sensors_input();





/*void initPWM() {

  TCCR1A = (1 << WGM10) | (1 << COM1A1) // Set up the two Control registers of Timer1.
           | (1 << COM1B1);          // Wave Form Generation is Fast PWM 8 Bit,
  TCCR1B = (1 << WGM12)   // OC1A and OC1B are cleared on compare match
           | (1 << CS11) | (1 << CS10);        // and set at BOTTOM. Clock Prescaler is 64.

  // OCR1A = 63;                       // Dutycycle of OC1A = 25%
  // OCR1B = 127;                      // Dutycycle of OC1B = 50%
  }*/

void LMN()// left motor nutral
{
  digitalWrite(B, 2, 0);
  digitalWrite(B, 3, 0);
}
void LMS()
{
  digitalWrite(B, 2, 1);
  digitalWrite(B, 3, 1);
}
void LMF()//left motor forward
{
  digitalWrite(B, 2, 1);
  digitalWrite(B, 3, 0);
}
void LMB()//left motor back wards
{
  digitalWrite(B, 2, 0);
  digitalWrite(B, 3, 1);
}

void RMN()
{
  digitalWrite(B, 0, 0);
  digitalWrite(B, 1, 0);
}
void RMS()
{
  digitalWrite(B, 0, 1);
  digitalWrite(B, 1, 1);
}
void RMF()
{
  digitalWrite(B, 0, 1);
  digitalWrite(B, 1, 0);
}
void RMB()
{
  digitalWrite(B, 0, 0);
  digitalWrite(B, 1, 1);
}

void sensors_input()//reding the IR sensors
{

  if ((PINA & (1 << PINA0))) // sensor 1 left side
  { s1 = 1;
  }
  else {
    s1 = 0;

  }

  if ((PINA & (1 << PINA1))) // sensor 2  middle left
  { s2 = 1;
  }
  else {
    s2 = 0;
  }

  if ((PINA & (1 << PINA4))) // sensor 3  middle
  { s3 = 1;
  }
  else {
    s3 = 0;
  }

  if ((PINA & (1 << PINA2))) // sensor 4  middle right
  { s4 = 1;
  }
  else {
    s4 = 0;
  }

  if ((PINA & (1 << PINA3))) // sensor 5  right side
  { s5 = 1;
  }
  else {
    s5 = 0;
  }




}







int main()
{

  DDRA = 0x00;              // make PA as input
  DDRB = 0xff;    // make PortB as output
  PORTA = 0b11100000;
  int state;

  while (1)
  {
    state = getstate();
    if (state == 0)
    {
      while (1)
      {
        refil();
        //        state = getstate();
        //        if (state == 0) break;
      }
    }
    else if (state == 1)
    { while (1)
      {

        sensors_input();
        move();
        //_delay_us(500);
        //        state = getstate();
        //                if (state == 1) break;
      }
    }
  }

}


void move()
{

  DDRA = 0x00; // make portA as inputs
  DDRB = 0xFF; // make Port  as output to connect motor pins
  int i;
  DDRD |= (1 << PD5); /* Make OC1A pin as output */
  TCNT1 = 0;    /* Set timer1 count zero */
  ICR1 = 4999;    /* Set TOP count for timer1 in ICR1 register */

  /* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
  TCCR1A = (1 << WGM11) | (1 << COM1A1);
  TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11);





  if ((s1 == 1) & (s2 == 1) & (s3 == 1) & (s4 == 1) & (s5 == 1)) //all sensors white

  {

    LMN();
    RMN(); //stop both motors
    //refil();
  }
  else if ( s2 == 1 & s3 == 0 & s4 == 1) // check if middle sensor is on black and sensor 2 and 4 is on white
  {
    LMF();
    RMF(); //move forward




  }



  else if (((s4 == 1) & (s5 == 1) & (s2 == 0) & (s3 == 0) & (s1 == 1))) //robot is in the middle but turned to left side
  {

    RMF();
    LMN();

  }

  else if (((s1 == 1) & (s2 == 1) & (s4 == 0) & (s3 == 0) & (s5 == 1)) ) //robot is in the middle but turned to right side
  {
    RMN();
    LMF();

  }



  //90 degrees turn
  else if (( s1 == 1 &  s4 == 0 & s5 == 0  & s2 == 1 )) // 90 degrees to right
  {


    RMN();
    LMF();
    _delay_ms(200);
    RMB();
  }

  else if (( s1 == 1 &  s4 == 0 & s5 == 0 &  s2 == 0 )) // 90 degrees to right
  {


    RMN();
    LMF();
    _delay_ms(200);
    RMB();
  }

  //  {
  //
  //
  //    RMB();
  //    LMF();
  //    while(1)
  //    { RMB();
  //      LMF();
  //     // _delay_ms(1);
  //      sensors_input();
  //     // _delay_ms(1);
  //      if ( s2 ==0 & s3 == 0 & s4== 0)
  //      {RMB();
  //      LMF();}
  //      if ( s2 ==0 & s3 == 0 & s4== 0 & s5 ==0)
  //      {RMB();
  //      LMF();}
  //      if ( s2 ==0 & s3 == 0 & s4== 0 & s5 ==1 & s1 ==0)
  //      {RMB();
  //      LMF();}
  //      if ( s2 ==0 & s3 == 0 & s4== 0 & s5 ==0 & s1==0)
  //      {RMB();
  //      LMF();}
  //      if (s5 ==1 & s4 == 1) break;
  //
  //    }
  //    //    _delay_ms(10);
  //    //      sensors_input();
  //    //      if(s5==1 & s4==1)
  //    //      break;
  //
  //
  //
  //  }

  else if (( s1 == 0 & s2 == 0  & s5 == 1  & s4 == 1 )) // 90 degrees turn to left
  {


    LMN();
    RMF();
    _delay_ms(200);
    LMB();
  }

  else if (( s1 == 0 & s2 == 0  & s5 == 1  & s4 == 0)) // 90 degrees turn to left
  {


    LMN();
    RMF();
    _delay_ms(200);
    LMB();
  }

  //  {
  //
  //    RMF();
  //    LMB();
  //    while(1)
  //    { RMF();
  //      LMB();
  //     // _delay_ms(1);
  //      sensors_input();
  //     // _delay_ms(1);
  //      if ( s2 ==0 & s3 == 0 & s4== 0)
  //      {RMF();
  //      LMB();}
  //      if ( s2 ==0 & s3 == 0 & s4== 0 & s5 ==0)
  //      {RMF();
  //      LMB();}
  //      if ( s2 ==0 & s3 == 0 & s4== 0 & s5 ==1 & s1 ==0)
  //      {RMF();
  //      LMB();}
  //      if ( s2 ==0 & s3 == 0 & s4== 0 & s5 ==0 & s1==0)
  //      {RMF();
  //      LMB();}
  //      if(s1==1 & s2 ==1) break;
  //
  //    //    _delay_ms(10);
  //    //      sensors_input();
  //    //      if(s1==1 & s2==1)
  //    //      break;
  //    //
  //
  //
  //
  //
  //  }
  //  }

  else if (s1 == 0 & s2 == 0 & s3 == 0 & s4 == 0 & s5 == 0) //all sensors black
  {

    LMN();
    RMN();


//      
////     _delay_ms(1000);
//     OCR1A = 375;
//     _delay_ms(1000);
//     OCR1A = 125;

    
    dispense();
    while(1)
    {
    if (med_taken())
    {
      while(1)
      {
        if (!(med_taken())) break;
      }
      break;
    }
    }
_delay_ms(3000);
OCR1A = 125;
//_delay_ms(1000);


sensors_input();
    
    // _delay_ms(6000);

    while (s1 == 0 & s2 == 0 & s3 == 0 & s4 == 0 & s5 == 0)
    {


      RMF();
      LMF();
      sensors_input();
    }


  }
}



int med_taken()
{
  if ((PINA & (1 << PINA6))) // sensor 2  middle left
  { return  1;
  }
  else {
    return 0;
  }
}

int getstate ()
{

  if (!(PINA & (1 << PINA7)))
    return 1;
  else return 0;

}

void refil()
{
  DDRA = 0x00;
  PORTA = 0xff;


  DDRB = 0xff;

  DDRD |= (1 << PD5); /* Make OC1A pin as output */
  TCNT1 = 0;    /* Set timer1 count zero */
  ICR1 = 4999;    /* Set TOP count for timer1 in ICR1 register */

  /* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
  TCCR1A = (1 << WGM11) | (1 << COM1A1);
  TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11);

  PORTB = 0b00000000;
  //_delay_ms(3);
  //    while (1)
  //    {

  if (!(PINA & (1 << PINA5)))

  {


    OCR1A = 290;
    _delay_ms(9000);


    OCR1A = 456;
    _delay_ms(10000);

    OCR1A = 625;
    _delay_ms(10000);




  }
  //  else continue;
  //}


}


void dispense()
{


  
    DDRD |= (1 << PD5); /* Make OC1A pin as output */
    TCNT1 = 0;    /* Set timer1 count zero */
    ICR1 = 4999;    /* Set TOP count for timer1 in ICR1 register */
  
    /* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
    TCCR1A = (1 << WGM11) | (1 << COM1A1); 
    TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11);
  //Init LCD Module with NO Cursor
  int reqid;
  
 
  USARTInit(103);
  //Init USART @ 9600bps

  

  

  reqid = getreq();
  
  if (reqid == 16265)
  {
   
      OCR1A = 250;
      //_delay_ms(3000);
      //OCR1A = 125;



  }

  if (reqid == 21288)
  {
    
      OCR1A = 375;
      //_delay_ms(3000);
      //OCR1A = 125;



  }

  if (reqid == 14448)
  {

      OCR1A = 500;
      //_delay_ms(3000);
      //OCR1A =125;



  }

}







int getreq()
{

  long int card_number = 0;
  while (1)
  {

    _delay_ms(10);

    //Check if USART has Got data from RFID
    //RFID packet is always 14 bytes long
    if (UDataAvailable() >= 14)
    {


      //Check the First byte it must be 2
      if (UReadData() == 2)
      {

        //We got correct packet!
        //Display it on LCD

        //Go to start of 2nd line on LCD


        //First Two bytes of data are NOT used, so remove them from queue.
        UReadData();
        UReadData();

        //Card data is 4 byte long in hex format so we need 8 chars to store it.
        //and one more for C's end of string marker (NULL Char)
        char card_number_in_hex[10];

        uint8_t i;
        for (i = 0; i < 8; i++)
        {
          card_number_in_hex[i] = UReadData();
        }

        //Put Null char at the end
        card_number_in_hex[8] = '\0';

        //Discard \ Checksum (2 bytes)

        UReadData();
        UReadData();

        //Read and discard the last byte (Stop Byte)
        UReadData();

        //Convert HEX to decimal
        card_number = strtol(card_number_in_hex, NULL, 16);
        //     lcd_clear();
        //          lcd_clear();
        //          lcd_gotoxy(0, 1);
        //          lcd_printint(card_number);

        //Wait so that user can read the LCD
        //          _delay_ms(2000);
        //          lcd_clear();

        //          if (card_number == 16265)
        //          {
        //            lcd_clear();
        //            lcd_gotoxy(1,0);
        //            lcd_print("paitient 1");
        //            _delay_ms(2000);
        //            }

        break;





      }
    }
  }

  return card_number;
}
