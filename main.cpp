#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <time.h>

#define DEBUG 0;

// virtual keys
const short VKEY_ESCAPE = 0x1B;
const short VKEY_S= 0x53;
const short VKEY_D = 0x44;
const short VKEY_R = 0x52;
const short VKEY_Z = 0x5A;
const short VKEY_LEFT = 0x4E;
const short VKEY_RIGHT = 0x4D;

const char DELIM = ':';

// function prototypes
int isPressed(short);
void recordKeys(void);
void waitForInput(void);
void printRecord(void);
char vkeyToChar(short);

// global variables
long timeRecord[5000] = { 0 };
short keyRecord[5000] = { 0 };
short pressRecord[5000] = { 0 };
short recordIndex = 0;

int main() {

	waitForInput();
	return 0;
}

int isPressed(short vkey) {
	return ((1 << 16) & GetAsyncKeyState(vkey)) ? 1 : 0;
}

void waitForInput() {
	while (1) {
		if (isPressed(VKEY_S)) {
			while (isPressed(VKEY_S));

			recordIndex = 0;
			recordKeys();
		}
		else if (isPressed(VKEY_R)) {
			while (isPressed(VKEY_R));
			std::cout << "PLAYBACK" << DELIM << std::flush;
		}
	}
}

void printRecord() {
	std::cout << "START" << DELIM << recordIndex << DELIM;
	for (int i = 0; i < recordIndex; i++) {
		std::cout << vkeyToChar(keyRecord[i]) << DELIM << pressRecord[i] << DELIM << timeRecord[i] << DELIM;
	}
	std::cout << "END" << DELIM << std::flush;
	
}

char vkeyToChar(short vkey) {
	switch (vkey) {
	case 0x4E:
		return 'l';
	case 0x4D:
		return 'r';
	case 0x5A:
		return 'j';
	}
}

void recordKeys() {
	long startTime = clock();
	int prevKeyStates[3] = { 0,0,0 };
	short keys[3] = { VKEY_Z , VKEY_LEFT, VKEY_RIGHT };

	#if DEBUG
	std::cout << "begin recording" << std::endl;
	#endif

	while (1) {

		// check press for D to stop record
		if (isPressed(VKEY_D))
			break;

		// check z left and right
		for (int i = 0; i < 3; i++) {
			int keyPressed = isPressed(keys[i]);
			

			if (keyPressed && !prevKeyStates[i]
			|| !keyPressed && prevKeyStates[i]) {
				timeRecord[recordIndex] = clock() - startTime;
				pressRecord[recordIndex] = keyPressed;
				keyRecord[recordIndex] = keys[i];
				
				#if DEBUG
				std::cout << (keyPressed ? "Pressed : " : "Released : ") << keys[i] << " : " << clock() << std::endl;
				#endif

				recordIndex++;
			} 
			prevKeyStates[i] = keyPressed;
		}
	}

	#if DEBUG
	std::cout << "finished recording" << std::endl;
	#endif

	if (recordIndex > 0)
		printRecord();
}