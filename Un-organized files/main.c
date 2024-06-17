/*
 * main.c
 *
 *  Created on: 28 juin 2022
 *      Author: jonat
 */

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Init.h"
#include "Time.h"
#include "regul.h"
#include "EoT.h"
#include "ADC.h"
#include "uart.h"



// UART Port Configuration parameters and registers
UARTConfig cnf;
USCIUARTRegs uartUsciRegs;
USARTUARTRegs uartUsartRegs;

// Buffers to be used by UART Driver
unsigned char uartTxBuf[200];
unsigned char uartRxBuf[200];


//Variables globales-------------------
sTimeDate Time;

int HallSens1;
int HallSens2;

int horloge1ms;
int horloge10ms;
int horloge100ms, horloge100ms2, horloge1s, horloge1min, horloge1h = 6;
int horlogeMvtMot;
int ms;

int drapeau = 0;

double CurrentDistEL = 0, CurrentDistAZ = 0;

int isSleep;

volatile int time = 0;


//------------------------------------

volatile unsigned int i;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT

  InitOscillator();
  initTimerA1();
  initTestPorts();
  RtcInit();
  initADC();
  //initPWM();

  /********************************
  *
  * UART Specific Configuration
  *
  ********************************/
 initUartDriver();

 // Configure UART Module on USCIA1
 cnf.moduleName = USCI_A1;

 //
 P8SEL |= BIT3 + BIT2;
 // Use UART Pins P8.3 and P8.2
 cnf.portNum = PORT_8;
 cnf.RxPinNum = PIN3;
 cnf.TxPinNum = PIN2;

 // 115200 Baud from 20MHz SMCLK
 cnf.clkRate = F_SMCLK;
 cnf.baudRate = BAUD_RATE;
 cnf.clkSrc = UART_CLK_SRC_SMCLK;

 // 8N1
 cnf.databits = 8;
 cnf.parity = UART_PARITY_NONE;
 cnf.stopbits = 1;

 int res = configUSCIUart(&cnf,&uartUsciRegs);
 if(res != UART_SUCCESS)
 {
     // Failed to initialize UART for some reason
     __no_operation();
 }

 // Configure the buffers that will be used by the UART Driver.
 // These buffers are exclusively for the UART driver's use and should not be touched
 // by the application itself. Note that they may affect performance if they're too
 // small.
 setUartTxBuffer(&cnf, uartTxBuf, 200);
 setUartRxBuffer(&cnf, uartRxBuf, 200);

 enableUartRx(&cnf);

 char tempBuf[100];
 memset(tempBuf,0,100); //cleaning du buffer

 /*********************************/

  P3DIR |= BIT4; //Port 3.4 as output
  P3SEL |= BIT4; //Make SMCLK readable on port 3.4

  P1DIR |= BIT6 + BIT7; //Motor pins has outputs
  P2DIR |= BIT0 + BIT1; //Motor puns has outputs


  //------------------------------------------
  int motor_flag;
  int EL_flag = 0;
  int AZ_flag = 0;
  int old_min;
  int min_counter = 0;
  volatile double EoT;
  double declination_RAD;
  volatile double HRA_RAD;
  volatile double EL_RAD;
  volatile double AZ_RAD;
  volatile double deltaEL, distEL, oldDistEL;
  volatile double deltaAZ, distAZ, oldDistAZ;
  int Day;
  volatile double Long = 6.64; //° A Yverdon les bains
  volatile double Lat = 46.78; //° A Yverdon les bains
  double Lat_rad = Lat*TO_RAD;
  volatile double LT;
  int DeltaTutc = 1;  //En été en Suisse 2h à l'heure d'hiver A faire si le temps le permet, un algo pour calculer automatiquement la différence
  int setup, manual_AZ, manual_EL, tracking;
  char timeString[10];
  char str[10];

  volatile double Mux = 0;
  int sleepflag = 0;

  volatile double distELtriche, distAZtriche;

  volatile int old_hour = horloge1h;

  MotorAZstop();
  MotorELstop();
  ADCOn();

  volatile uint16_t ton = 0;

  volatile MODE mode = SETUP;
  volatile DATA_SETUP setup_switch = LAT;

  Time.Hours = 0x5;
  Time.Minutes = 0x00;
  Time.Date = 0x21;
  Time.Month = 0x6;
  Time.Seconds = 0x23;
  Time.Year = 0x2022;

  RtcSetTime(&Time);

  __enable_interrupt();    //enable maskable interrupts
  while (1)
  {
      if(time){
          sprintf(str,"%d:%d:%d\n",horloge1h,horloge1min,horloge1s);
          uartSendDataInt(&cnf,(unsigned char *)str, strlen(str));
          time = 0;
      }
      //Stop time fetching when in setup mode
      if(mode != SETUP){
          RtcFetchTime();
          RtcGetTime(&Time);
          RtcGetTimeToDecimal(&Time);
      }
     switch(mode){

     case SETUP :
         if(setup){
             setup = 0;
             uartSendDataInt(&cnf,(unsigned char *)"Mode Setup \n lettre + double pour set une valeur \nA->Lat\nB->long\nC->An\nD->Mois\nE->Date\nF->Heure\nG->Minute\nH->Sec\nI->setTime\n", strlen("Mode Setup \n lettre + double pour set une valeur \nA->Lat\nB->long\nC->An\nD->Mois\nE->Date\nF->Heure\nG->Minute\nH->Sec\nI->setTime\n"));
             MotorAZstop();
             MotorELstop();
             drapeau = 0;
             horloge1h = 6;
         }
         if(tempBuf[0] == 'A'){
             tempBuf[0] = '0';
             Lat = atof(tempBuf);
             Lat_rad = Lat*TO_RAD;
         }
         else if(tempBuf[0] == 'B'){
             tempBuf[0] = '0';
             Long = atof(tempBuf);

         }
         else if(tempBuf[0] == 'C'){
             tempBuf[0] = '0';
             Mux = atof(tempBuf);
             Time.Year = uint2bcd(atoi(tempBuf));
             //uint2bcd ne fonctionne pas avec une valeur comme 2022!!
         }
         else if(tempBuf[0] == 'D'){
             tempBuf[0] = '0';
             Mux = atof(tempBuf);
             Time.Month = uint2bcd(atoi(tempBuf));

         }
         else if(tempBuf[0] == 'E'){
             tempBuf[0] = '0';
             Mux = atof(tempBuf);
             Time.Date = uint2bcd(atoi(tempBuf));

         }
         else if(tempBuf[0] == 'F'){
             tempBuf[0] = '0';
             Mux = atof(tempBuf);
             Time.Hours = uint2bcd(atoi(tempBuf));

         }
         else if(tempBuf[0] == 'G'){
             tempBuf[0] = '0';
             Mux = atof(tempBuf);
             Time.Minutes = uint2bcd(atoi(tempBuf));

         }
         else if(tempBuf[0] == 'H'){
             tempBuf[0] = '0';
             Mux = atof(tempBuf);
             Time.Seconds = uint2bcd(atoi(tempBuf));

         }
         else if(tempBuf[0] == 'I'){
             RtcSetTime(&Time);
             Mux = atof(tempBuf);
             RtcFetchTime();
             RtcGetTimeString(timeString);
             Mux = atof(tempBuf);
             uartSendDataInt(&cnf,(unsigned char *)timeString, strlen(timeString));
             Mux = atof(tempBuf);
             tempBuf[0] = '0';
             Mux = atof(tempBuf);
         }

         break;

     case MANUAL_AZ :

         if(manual_AZ){
             manual_AZ = 0;
             AZ_flag = 0;
             uartSendDataInt(&cnf,(unsigned char *)"Réglage manuel AZ : +-10 mm pour bouger\n c pour motor stop\n d pour set 0\n", strlen("Réglage manuel AZ : +-10 mm pour bouger\n c pour motor stop\n d pour set 0\n"));
             MotorAZstop();
             MotorELstop();

         }
         if(tempBuf[0] == '+' || tempBuf[0] == '-'){
             deltaAZ = atof(tempBuf);
             tempBuf[0] = '0';
             AZ_flag = 1;

         }
         else if(tempBuf[0] == 'c'){
             tempBuf[0] = '0';
             AZ_flag = 0;
             MotorAZstop();
         }
         else if(tempBuf[0] == 'd'){
             tempBuf[0] = '0';
             CurrentDistAZ = distAZ;
             MotorAZstop();
         }
         break;

     case MANUAL_EL :
         if(manual_EL){
             manual_EL = 0;
             EL_flag = 0;
             uartSendDataInt(&cnf,(unsigned char *)"Réglage manuel EL : +-10 mm pour bouger\n", strlen("Réglage manuel AZ : +-10 mm pour bouger\n"));
             MotorAZstop();
             MotorELstop();
         }
         if(tempBuf[0] == '+' || tempBuf[0] == '-'){
             deltaEL = atof(tempBuf);
             tempBuf[0] = '0';
             EL_flag = 1;
         }
         else if(tempBuf[0] == 'c'){
             tempBuf[0] = '0';
             MotorELstop();
             EL_flag = 0;
         }
         else if(tempBuf[0] == 'd'){
             tempBuf[0] = '0';
             CurrentDistEL = distEL;
             MotorELstop();
         }
         break;

     case TRACKING :
         //Menu
        if(tracking){
            uartSendDataInt(&cnf,(unsigned char *)"Mode Tracking \n", strlen("Mode Tracking \n"));
            tracking = 0;
            motor_flag = 0;
            EL_flag = 0;
            AZ_flag = 0;
            min_counter = 0;
        }
        //Compte chaque fois qu'une minute passe
        //Go back to the start position
        //optimal would be to calculate sunrise and sunset to adjust this time during the year
        if(horloge1h >= 19 || horloge1h <= 7){
            if(sleepflag){
                //retour plein sud si besoin
                deltaAZ = -CurrentDistAZ;
                deltaEL = -CurrentDistEL;
                EL_flag = 1;
                AZ_flag = 1;
                sleepflag = 0;
            }

       }
       else if(old_hour != horloge1h){
            motor_flag = 1;
            min_counter = 0;
            EL_flag = 1;
            AZ_flag = 1;
            sleepflag = 1;
            old_hour = horloge1h;

            //if sun is too low, the motors don't move
            if(EL_RAD < 0.2){
                deltaEL = 0;
                deltaAZ = 0;
            }
            else{
                deltaEL = distEL - CurrentDistEL;
                deltaAZ = distAZ - CurrentDistAZ;
                Mux++;
            }
        }
        else if(motor_flag){
            if(EL_flag == 0 && AZ_flag == 0){
                //__bis_SR_register(LPM1_bits + GIE); // Enter LPM1
            }
        }
        else{
            //__bis_SR_register(LPM1_bits + GIE); // Enter LPM1
        }
         break;
     }

     //if(horlogeMvtMot){
        horlogeMvtMot = 0;
        AZ_flag = mesDistMotorAZ(deltaAZ, AZ_flag);
        EL_flag = mesDistMotorEL(deltaEL, EL_flag);
        if(AZ_flag){
          mvtMotorAZ(deltaAZ);
        }
        else{
          MotorAZstop();
          deltaAZ = 0;
        }

        if(EL_flag){
            mvtMotorEL(deltaEL);
        }
        else{
            MotorELstop();
            deltaEL = 0;
        }
     //}

     //Calcul des azimuth et elevation
      if(old_min != horloge1min){
         min_counter++;
         old_min = horloge1min;

         Day = (int)(30.44*(double)(Time.Month-1.00)+(double)Time.Date);
         EoT = calcEoT(Day);
         declination_RAD = calcDeclination(Day);


         LT = (float)(horloge1min*0.0166667)+horloge1h; //Local time in hours *0.0167 = /60
         //DeltaTutc = (int)(4.00 * Long + EoT) *0.0166667; //Equation fausse
         HRA_RAD = calcHRA(EoT, Long, LT, DeltaTutc);
         EL_RAD = calcEL(Lat_rad, declination_RAD, HRA_RAD);
         AZ_RAD = calcAZ(EL_RAD, HRA_RAD, declination_RAD, Lat_rad);
         distELtriche = distEL;
         distAZtriche = distAZ;
         distAZ = calcDistAZ(AZ_RAD);
         distEL = calcDistEL(EL_RAD, AZ_RAD);
         Mux++;

      }

     if(horloge100ms >= 1){
        horloge100ms = 0;
        int bytesAvailable = numUartBytesReceived(&cnf);
        if(bytesAvailable > 0)
        {
          memset(tempBuf,0,100);
          volatile int bytesRead = readRxBytes(&cnf, (unsigned char *)tempBuf, bytesAvailable, 0);
          if(bytesRead == bytesAvailable)
          {
              // All requested bytes read. Do something with it.
              // data is storred until next read.
              // If we receive the letter a, we toggle the LED
              Mux = atof(tempBuf);
              uartSendDataInt(&cnf,(unsigned char *)tempBuf, strlen(tempBuf));
              mode = ChooseMenu(tempBuf[0], &setup, &manual_AZ, &manual_EL, &tracking);
          }
          else
          {
              // Couldn't read all the bytes we requested
              __no_operation();
          }
        }
     }
  }
}


/*==============================================================================
 * // Description : Interruption du timer1 qui incrémente toutes les 1ms
 * Parametre(s): -
 * Retour      : -
 * ------------------------------------------------------------------------------ */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_1ms(void)
{
    horloge1ms++;
    ms++;

    if(horloge1ms >= 10){
        horloge1ms -= 10;
        horloge10ms++;
        horlogeMvtMot++;
    }
    if(horloge10ms >= 10){
        horloge10ms -= 10;
        horloge100ms++;
        horloge100ms2++;

    }

    //Fake time for the calculations
    if(horloge100ms2 >= 1){
        horloge100ms2 -= 1;
        horloge1s ++;

    }
    if(horloge1s >= 3){
        horloge1min ++;
        horloge1s -= 3;
        P8OUT ^= BIT0;

    }
    if(horloge1min >= 59){
        horloge1h ++;
        horloge1min-= 59;
        time = 1;
    }
    if(horloge1h >= 24){
        horloge1h -=24;
    }

}



