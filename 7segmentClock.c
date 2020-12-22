/*
*******************************************************
* Program Name:
*  lab2_01_D17PL4Y.c
*
* Purpose:
*       to display a clock on a 7seg display
*
*
* Test environment:
*   - Bare metal C via Atmel Studio 7
*   - ATtiny817 Xplained Pro development board
*
* Authors: Joshua Teixeira, Wilson Tran
*
* Course: ECE231, UMass Amherst
*
* Usage:
*     1. Run the program in Atmel Studio 7.
*     2. Observe the glorious 7seg display
*
* Revision history:
*      1.0 2019-04-05, Joshua Teixeira & Wilson Tran
*           Initial Version.
*      2.0 2019-04-09, Joshua Teixeira & Wilson Tran
			Added everything.
* Note: 2019-04-09 - tested and ready for demo
*
* References:
*       UIAs and datasheets
*
*******************************************************
*/

#include <math.h>
#include <avr/io.h>
#include <stdio.h>
#define UNIT_TIME .0001	//Delay amount between digits
#define BASE_UNIT 60	//Make this '1' for a clock that counts up seconds, 60 for normal

//delay on a second clock to handle space between digits
void quikDelay(double time){
	double numOfTicks = time/(1/(pow(2,14)-1));		//same from Morse delay method
	TCB0.CNT = 0x0;									//set reg content to 0
	while(TCB0.CNT < numOfTicks){					//wait until the reg has counted up
		//
	}
	return;
}

void disp(int num, int pos){
	SPI0.INTFLAGS = 0b01000000;  //1 bit tells us if its ready
	switch(pos){
		case 0 :		//Choose which digit to display the num on
			SPI0.DATA = 0b00001000; //digit 1
			break;
		case 1 :
			SPI0.DATA = 0b00010000; //digit 2
			break;
		case 2 :
			SPI0.DATA = 0b00100000; //digit 3
			break;
		case 3 :
			SPI0.DATA = 0b01000000;	//digit 4
			break;
		default:
			break;
		}

		while((SPI0.INTFLAGS & 0b01000000) == 0b01000000){
		}
		
		switch(num){
			case 0 :		//choose what num to display
				SPI0.DATA = 0b01111110; // 0
				break;
			case 1 :
				SPI0.DATA = 0b00001100; // 1
				break;
			case 2 :
				SPI0.DATA = 0b10110110; // 2
				break;
			case 3 :
				SPI0.DATA = 0b10011110; // 3
				break;
			case 4 :
				SPI0.DATA = 0b11001100; // 4
				break;
			case 5 :
				SPI0.DATA = 0b11011010; // 5
				break;
			case 6 :
				SPI0.DATA = 0b11111010; // 6
				break;
			case 7 :
				SPI0.DATA = 0b00001110; // 7
				break;
			case 8 :
				SPI0.DATA = 0b11111110; // 8
				break;
			case 9 :
				SPI0.DATA = 0b11001110; // 9
				break;
			default :
				break;
			}
}

int main(void){
	SPI0.INTCTRL = 0b01100000;	// Setting up interrupts
	PORTA.DIR = 0xFF;
	SPI0.CTRLA = 0b00100001;
	SPI0.CTRLB = 0b11000100;	// enable buffer, enable ssd, enable master
	TCB0.CTRLA = 0b00000001;	// independent timer

	//set pre-scale to divide by 32768 to get a 1Hz freq, enable bit 1, new period = 1 second
	RTC.CTRLA = 0b01111001; //0 XXXX (prescale) 00 X (enable)
	
	//SET CURRENT TIME HERE
	int hrs = 0;
	int hr	= 4;
	int mins= 1;
	int min = 9;
	
	RTC.CNT = 0x0; //set RTC to zero, 1Hz freq so 60 ticks is 60 sec
	while(1){
		disp(hrs, 0);
		quikDelay(UNIT_TIME);
		disp(hr, 1);
		quikDelay(UNIT_TIME);
		disp(mins, 2);
		quikDelay(UNIT_TIME);
		disp(min, 3);
		quikDelay(UNIT_TIME);
		if(RTC.CNT >= BASE_UNIT){//60 sec or 1sec
			RTC.CNT = 0x0;
			min += 1;
		}
		if(min >= 10){
			min = 0; 
			mins += 1;
		}
		if(mins >= 6){
			mins = 0;
			hr += 1;
		}
		if(hrs >= 2 && hr >= 4){
			hrs = 0; hr = 0; mins = 0; min = 0;
		}
		if(hr >= 10){
			hr = 0;
			hrs += 1;
		}
	}

	return 0;
}
