/*
  Digital Pot Control

  This example controls an Analog Devices AD5206 digital potentiometer.
  The AD5206 has 6 potentiometer channels. Each channel's pins are labeled
  A - connect this to voltage
  W - this is the pot's wiper, which changes when you set it
  B - connect this to ground.

  The AD5206 is SPI-compatible,and to command it, you send two bytes,
  one with the channel number (0 - 5) and one with the resistance value for the
  channel (0 - 255).

  The circuit:
  * All A pins  of AD5206 connected to +5V
  * All B pins of AD5206 connected to ground
  * An LED and a 220-ohm resisor in series connected from each W pin to ground
  * CS - to digital pin 10  (SS pin)
  * SDI - to digital pin 11 (MOSI pin)
  * CLK - to digital pin 13 (SCK pin)

  created 10 Aug 2010
  by Tom Igoe

  Thanks to Heather Dewey-Hagborg for the original tutorial, 2005

*/


// inslude the SPI library:
#include <SPI.h>


void clear_strand (void);
void start_frame (void);
void end_frame (void);
void set_led(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

// make a frame buffer

void setup() {
	// set the slaveSelectPin as an output:
	/* pinMode(slaveSelectPin, OUTPUT); */
	// initialize SPI:
	SPI.begin();
	SPI.beginTransaction (SPISettings (100000, MSBFIRST, SPI_MODE0));
	Serial.begin (9600);
	Serial.println ("init");
}

void
clear_strand (void)
{
	int idx;

	Serial.println ("clear");

	start_frame ();

	for (idx = 0; idx < 350; idx++) {
		set_led (0, 0, 0, 0);
	}

	/* end_frame (); */
}

void
start_frame (void)
{
	int idx;

	for (idx = 0; idx <  4; idx++) {
		SPI.transfer (0x00);
	}
}

void
end_frame (void)
{
	int idx;

	for (idx = 0; idx < 4; idx++) {
		SPI.transfer (0xff);
	}
}

void set_led(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 31)
{
	SPI.transfer(0b11100000 | brightness);
	SPI.transfer(blue);
	SPI.transfer(green);
	SPI.transfer(red);
}

int
hexval (int c)
{
	if (c >= '0' && c <= '9') {
		return (c - '0');
	} else if (c >= 'a' && c <= 'f') {
		return (c - 'a' + 10);
	} else if (c >= 'A' && c <= 'F') {
		return (c - 'A' + 10);
	} else {
		return (0);
	}
}

int acc = 0;

void loop() {
	char c;
	char buf[100];

	if (Serial.available ()) {
		c = Serial.read ();

		if (isxdigit (c)) {
			acc = ((acc << 4) | hexval (c)) & 0xff;
			Serial.println (buf);
		} else {
			switch (c) {
			case '.':
				sprintf (buf, "buffered: 0x%x", acc);
				Serial.println (buf);
				break;
			case ' ':
				sprintf (buf, "sending: 0x%x", acc);
				Serial.println (buf);
				SPI.transfer (acc);
				acc = 0;
				break;
			case 'x':
				clear_strand ();
				break;
			case '-':
				start_frame ();
				set_led (0x00, 0x00, 0x40, 0xf0);
				set_led (0x00, 0x40, 0x00, 0xf0);
				set_led (0x40, 0x00, 0x00, 0xf0);
				set_led (0x40, 0x00, 0x00, 0xf0);

				end_frame ();
				break;
			}
		}
				
		/* if (c == 'c') { */
		/* 	clear_strand (); */
		/* } else if (c == 'r') { */
		/* 	red_strand (); */
		/* } else if (c == ' ') { */
		/* 	Serial.println ("frame"); */

		/* 	if (0) { */
		/* 		SPI.transfer (0x51); */
		/* 	} else if (1) { */
		/* 		int idx; */
		/* 		start_frame (); */

		/* 		set_led (0x00, 0x00, 0x40, 0xf0); */
		/* 		set_led (0x00, 0x40, 0x00, 0xf0); */
		/* 		set_led (0x40, 0x00, 0x00, 0xf0); */
		/* 		set_led (0x40, 0x00, 0x00, 0xf0); */

		/* 		/\* end_frame (); *\/ */
		/* 		/\* for (idx = 0; idx < 12; idx++) { *\/ */
		/* 		/\* 	SPI.transfer (0xff); *\/ */
		/* 		/\* } *\/ */

		/* 		SPI.transfer (0xe0); */
		/* 		SPI.transfer (0x00); */
		/* 		SPI.transfer (0x00); */
		/* 		SPI.transfer (0xff); */

		/* 		SPI.transfer (0xff); */
		/* 		SPI.transfer (0xff); */
		/* 		SPI.transfer (0xff); */
		/* 		SPI.transfer (0xff); */
		/* 	} */
		/* } */
	}


	/* for (int idx = 0; idx < 3; idx++) { */
	/* 	  SPI.transfer (0xff); */
	/* } */
	/* SPI.transfer (0b11101111); */
	/* SPI.transfer (0); */
	/* SPI.transfer (0xff); */
	/* SPI.transfer (0); */

	/* for (int idx = 0; idx < 3; idx++) { */
	/* 	  SPI.transfer (0xff); */
	/* } */

	/* SPI.transfer (0b11101111); */
	/* SPI.transfer (0); */
	/* SPI.transfer (0); */
	/* SPI.transfer (0xff); */


	// go through the six channels of the digital pot:
	/* for (int channel = 0; channel < 6; channel++) { */
	/*   // change the resistance on this channel from min to max: */
	/*   for (int level = 0; level < 255; level++) { */
	/*     digitalPotWrite(channel, level); */
	/*     delay(10); */
	/*   } */
	/*   // wait a second at the top: */
	/*   delay(100); */
	/*   // change the resistance on this channel from max to min: */
	/*   for (int level = 0; level < 255; level++) { */
	/*     digitalPotWrite(channel, 255 - level); */
	/*     delay(10); */
	/*   } */
	/* } */

}

void digitalPotWrite(int address, int value) {
	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin, LOW);
	//  send in the address and value via SPI:
	SPI.transfer(address);
	SPI.transfer(value);
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin, HIGH);
}
