/*
 * IRSubAssyRemote.c
 *  Created on: Aug 2023
 *  Author: Nelson
 *
 *  When used with MSP432 Launch Pad,
 *  Connect IR Emitter to pin 3.0. Use approx. 200ohm.
 */
#include <IRSubAssyRemote.h>
#include <stdio.h>
#include <stdint.h>
#include "msp432.h"

#define IROUT3_0 (*((volatile uint8_t *)(0x42098440)))  // P3.0 IR ODD  Port 3 Output P3OUT 022h
// #define IROUT9_2 (*((volatile uint8_t *)(0x42099048))) // P9.2 IR ODD  Port 9 Output P9OUT 082h, not power enough to turn-on Rx equipment
// #define IROUT5_3 (*((volatile uint8_t *)(0x4209884C))) // P5.3 IR EVEN Port 5 Output P5OUT 042h, not power enough to turn-on Rx equipment
#define IROUTPORT   IROUT3_0
int carrierfreq = 0;


void IR_SubAssy_Init(void){
    // INI IR INPUT
    // Enable Bottom IR Rx Active. This will allow capture using logic analyzer
    // Use 0x04 = P7.2, //0xFF = P7.0 thru 7.7
    P7->SEL0 &= ~0x04;
    P7->SEL1 &= ~0x04;     // 1) Configure P7.x GPIO
    P7->DIR  |=  0x04;     // 2) Set 7.x to output
    P7->REN  |=  0x04;     // 3) Enable pull resistors
    P7->OUT  |=  0x04;     //    Set 7.x output active

    // INI SIDE BUTTON INPUT
    // Press button to transmit IR
    P1->SEL0 &= ~0x12;
    P1->SEL1 &= ~0x12;      // 1) Configure P1.4, P1.1 as GPIO
    P1->DIR  &= ~0x12;      // 2) Set P1.4 and P1.1 input
    P1->REN  |=  0x12;      // 3) Enable pull resistors on P1.4 and P1.1
    P1->OUT  |=  0x12;      //    P1.4 and P1.1 are pull-up

    // INI OUTPUT
    // Connect IR Emitter to pin 3.0. Use approx. 200ohm.
    P3->SEL0 &= ~0x01;
    P3->SEL1 &= ~0x01;      // 1) Configure P3.0 as GPIO
    P3->DIR  |=  0x01;      // 2) Set P3.0 output
    P3->DS   |=  0x01;      // 3) Enable increased drive strength
    P3->OUT  &= ~0x01;      //    all LEDs off

}

//------------LaunchPad_Input------------
// Input from Switches
// Input: none
// Output: 0x00 none
//         0x01 Button1
//         0x02 Button2
//         0x03 both Button1 and Button2
uint8_t BoseButton_Input(void){
  return ((((~(P1->IN))&0x10)>>3)|(((~(P1->IN))&0x02)>>1));   // read P1.4,P1.1 inputs
}

void RawIR_Play(uint16_t rawdata){
    // without 48Mhz clock 53.88khz
    // with 48Mhz clock 628.93khz
int i = 0;
int j = 0;
int k = 0;

    for (k = 0; k < 16; k++){ // Process data
        if(0x01 & (rawdata >> k)){  // Set state based on bit
                for (j = 0; j < 23; j++){ // About 46 pulses for per 1 bit

                    // i < 35/i < 19 = 36.7khz Single Carrier Frequency
                    // i < 33/i < 18 = 38.91khz Single Carrier Frequency
                    for (i = 0; i < 33; i++){
                        if (i < 18){
                            IROUTPORT = 0x01;
                        }else{
                            IROUTPORT = 0x00;
                        }
                    }
                }
        }
        else{
                for (j = 0; j < 23; j++){ // About 46 pulses for per 0 bit
                    // i < 35/i < 19 = 36.7khz Single Carrier Frequency
                    // i < 33/i < 18 = 38.91khz Single Carrier Frequency
                    for (i = 0; i < 33; i++){
                        if (i < 18){
                            IROUTPORT = 0x00;
                        }else{
                            IROUTPORT = 0x00;
                        }
                    }
                }
        }
    }


}





void ProntoIR_Play(uint16_t rawdata, uint8_t count){
int i = 0;
int j = 0;
if(count == 0){

}
if(count == 1){ // Carrier Frequency
      carrierfreq = rawdata * 6/52;
      // carrierfreq = rawdata * 629/4145;
      // todo add later, capture carrier frequency
}
if(count > 3){ //check even "1" or odd "0"

    if(count & 0x01){ // Odd

        for (j = 0; j < rawdata; j++){ //
            for (i = 0; i < (carrierfreq*2); i++){
                if (i < carrierfreq){
                    IROUTPORT = 0x00;
                }else{
                    IROUTPORT = 0x00;
                }
            }
        }

    }else{ // Even
        for (j = 0; j < rawdata; j++){ // About 46 pulses for per 0 bit
            // Create 1 period of carrier frequency
            for (i = 0; i < (carrierfreq*2); i++){
                if (i < carrierfreq){
                    IROUTPORT = 0x01;
                }else{
                    IROUTPORT = 0x00;
                }
            }
        }
    }


}




}

void IROutput_On(void){
    IROUTPORT = 0x01;
}

void IROutput_Off(void){
    IROUTPORT = 0x00;
}


