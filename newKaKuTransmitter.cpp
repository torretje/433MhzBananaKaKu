/*
 * NewRemoteSwitch library v1.0.0 (20121229) made by Randy Simons http://randysimons.nl/
 * See NewRemoteTransmitter.h for details.
 *
 * License: GPLv3. See license.txt
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <iostream>
#include "RemoteSwitch.cpp"
#include "newKaKuTransmitter.h"

int main(int argc, char **argv) 
{
    int pin_out=0;
    int unit = 0;
    long  addres =  11876070;
    bool state = false;
    int period= 250;    
    
    if( argc != 4 ) { // not enough arguments
        std::cout << "usage: " << argv[0] << " adress unit state" << std::endl;
        std::cout << "example: " << argv[0] << " 11876070 0 on" << std::endl;
            return 0;
    } else {
            addres = atol(argv[1]);
            unit = atol(argv[2]);
           std:: string statestr = argv[3];
            if( statestr.compare("on") == 0 ) {
                    state = true;
            } else {
                    state = false;
            }
    }
    // Check if the pi library is installed
    if(wiringPiSetup() == -1)
    {
        printf("WiringPi setup failed. Maybe you haven't installed it yet?");
        exit(1);
    }

    // setup pin and make it low
    pinMode(pin_out, OUTPUT);
    //Enable pulldown resistors 
    pullUpDnControl(pin_out,PUD_DOWN);

    NewRemoteTransmitter transmitter(addres, pin_out, period, 3);
    transmitter.sendUnit(unit, state);
}

NewRemoteTransmitter::NewRemoteTransmitter(unsigned long address, unsigned short pin, unsigned int periodusec, unsigned short repeats) {
	_address = address;
	_pin = pin;
	_periodusec = periodusec;
	_repeats = (1 << repeats) - 1; // I.e. _repeats = 2^repeats - 1

	pinMode(_pin, OUTPUT);
}

void NewRemoteTransmitter::sendGroup(bool switchOn) {
	for (int i = _repeats; i >= 0; i--) {
		_sendStartPulse();

		_sendAddress();

		// Do send group bit
		_sendBit(true);

		// Switch on | off
		_sendBit(switchOn);

		// No unit. Is this actually ignored?..
		_sendUnit(0);

		_sendStopPulse();
	}
}

void NewRemoteTransmitter::sendUnit(unsigned short unit, bool switchOn) {
	for (int i = _repeats; i >= 0; i--) {
		_sendStartPulse();

		_sendAddress();

		// No group bit
		_sendBit(false);

		// Switch on | off
		_sendBit(switchOn);

		_sendUnit(unit);

		_sendStopPulse();
	}
}

void NewRemoteTransmitter::sendDim(unsigned short unit, unsigned short dimLevel) {
	for (int i = _repeats; i >= 0; i--) {
		_sendStartPulse();

		_sendAddress();

		// No group bit
		_sendBit(false);

		// Switch type 'dim'
		digitalWrite(_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, LOW);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, LOW);
		delayMicroseconds(_periodusec);

		_sendUnit(unit);

		for (short j=3; j>=0; j--) {
		   _sendBit(dimLevel & 1<<j);
		}

		_sendStopPulse();
	}
}

void NewRemoteTransmitter::_sendStartPulse(){
	digitalWrite(_pin, HIGH);
	delayMicroseconds(_periodusec);
	digitalWrite(_pin, LOW);
	delayMicroseconds(_periodusec * 10 + (_periodusec >> 1)); // Actually 10.5T insteat of 10.44T. Close enough.
}

void NewRemoteTransmitter::_sendAddress() {
	for (short i=25; i>=0; i--) {
	   _sendBit((_address >> i) & 1);
	}
}

void NewRemoteTransmitter::_sendUnit(unsigned short unit) {
	for (short i=3; i>=0; i--) {
	   _sendBit(unit & 1<<i);
	}
}

void NewRemoteTransmitter::_sendStopPulse() {
	digitalWrite(_pin, HIGH);
	delayMicroseconds(_periodusec);
	digitalWrite(_pin, LOW);
	delayMicroseconds(_periodusec * 40);
}

void NewRemoteTransmitter::_sendBit(bool isBitOne) {
	if (isBitOne) {
		// Send '1'
		digitalWrite(_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, LOW);
		delayMicroseconds(_periodusec * 5);
		digitalWrite(_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, LOW);
		delayMicroseconds(_periodusec);
	} else {
		// Send '0'
		digitalWrite(_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, LOW);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_pin, LOW);
		delayMicroseconds(_periodusec * 5);
	}
}
