/*
 * chatgptX.h
 *
 *  Created on: Aug 2023
 */

#ifndef IRSUBASSYREMOTE_H_
#define IRSUBASSYREMOTE_H_
#include <stdint.h>

/*
Records bose IR Remote
*/

void IR_SubAssy_Init(void);
uint8_t BoseButton_Input(void);
void RawIR_Play(uint16_t rawdata);
void ProntoIR_Play(uint16_t rawdata, uint8_t count);
void IROutput_On(void);
void IROutput_Off(void);

#endif /* IRSUBASSYREMOTE_H_ */
