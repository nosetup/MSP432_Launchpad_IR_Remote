/*
 * main.h
 *
 *  Created on: Aug 2023
 *      Author: Nelson
 *
 *      Using Ti MSP432 LaunchPad with IR Emitter,
 *      RH Button Press will transmit IR Remote.
 *      Supports both RAW and Pronto Hex Files.
 *      RAW Files = Data Captured using Logic Analyzer. Retransmitted at fixed frequency with Raw Data.
 *      Pronto Hex Files = Array Containing  with Specified ON/OFF Times.
 *
 */


#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include "IRSubAssyRemote.h"
#include "Clock.h"

// Remote Codes Macro
#include "IRCodesBoseSoundtouch300.h"
#include "IRCodesSamsungTV.h"
// #define DEBUG  // Uncomment to enable debug mode

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    #ifdef DEBUG
    printf ("\r start of test \r");
    #endif

    Clock_Init48MHz();
    IR_SubAssy_Init();

    #ifdef DEBUG
    printf ("\r end of test \r");
    #endif

    uint8_t i = 0;

    #ifdef DEBUG
    BoseIR_On(); // Use for debug.. set output high.
    #endif


    while(1){

        if (BoseButton_Input()){

//      Template to Send Captured Hex
//            for (i = 0; i < 8; i++)
//            {
//                RawIR_Play(BoseCtrl_ON[i]);
//            }

            //      Template to Send Pronto Raw Hex
            for (i = 0; i < 72; i++)
            {
                ProntoIR_Play(SamsungTVPronto_PwrToggle[i], i);
            }

            Clock_Delay1ms(500);//add delay
        }
        else{
            IROutput_Off();// IR OFF
        }

    }
}

