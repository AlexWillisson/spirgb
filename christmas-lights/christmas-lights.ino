#include <SPI.h>

#define LED_COUNT 304

int running;

void clear_strand (void);
void start_frame (void);
void setup_frame (void);
void setup_cylon (void);
void step_cylon (void);
void push_led (unsigned long pixel);
void set_led (uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);

void
setup (void)
{
	setup_cylon ();
	SPI.begin();
	/* SPI.beginTransaction (SPISettings (1000000, MSBFIRST, SPI_MODE0)); */
	Serial.begin (9600);
	Serial.println ("init2");

  running = 1;
}

void
clear_strand (void)
{
	int idx;

	Serial.println ("clear");

	start_frame ();

	for (idx = 0; idx < LED_COUNT+2; idx++) {
		push_led (0xe0000000);
	}
}

void
start_frame (void)
{
	SPI.transfer (0x00);
	SPI.transfer (0x00);
	SPI.transfer (0x00);
	SPI.transfer (0x00);
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

unsigned long frames[LED_COUNT+1];

void
setup_frame (void)
{
	int idx;

	for (idx = 0; idx < LED_COUNT; idx++) {
		switch (idx % 3) {
		case 0:
			frames[idx] = 0x400000f0;
			break;
		case 1:
			frames[idx] = 0x004000f0;
			break;
		case 2:
			frames[idx] = 0x000040f0;
			break;
		}
	}

	frames[idx] = 0;
}

int cylon_eye;
int cylon_dir;

void
setup_cylon (void)
{
	int idx;

	cylon_dir = 1;
	cylon_eye = 0;

	for (idx = 0; idx < LED_COUNT; idx++) {
		if (idx == cylon_eye) {
			frames[idx] = 0x400000f0;
		} else {
			frames[idx] = 0x000000e0;
		}
	}

	frames[idx] = 0;
}

void
step_cylon (void)
{
	char buf[100];

	if (cylon_dir == 1) {
		frames[cylon_eye] = 0x000000e0;
		if (cylon_eye >= LED_COUNT-1) {
			cylon_dir = -1;
			cylon_eye = LED_COUNT - 2;
		} else {
			cylon_eye += 1;
		}
	} else {
		frames[cylon_eye] = 0x000000e0;
		if (cylon_eye <= 0) {
			cylon_dir = 1;
			cylon_eye = 1;
		} else {
			cylon_eye -= 1;
		}
	}

	frames[cylon_eye] = 0x400000f0;
}

void
push_led (unsigned long pixel)
{
	SPI.transfer (pixel & 0xff);
	SPI.transfer ((pixel >> 8) & 0xff);
	SPI.transfer ((pixel >> 16) & 0xff);
	SPI.transfer ((pixel >> 24) & 0xff);
}

void
write_frame (void)
{
	char buf[100];
	int idx;

	start_frame ();

	for (idx = 0; frames[idx]; idx++) {
		push_led (frames[idx]);
	}

}

int delay_count;

void
loop (void)
{
	char c;
	char buf[100];

  if (running) {
    delay_count++;
    if (delay_count >= 1000) {
        delay_count = 0;
        step_cylon();
        write_frame();
    }
  }
  
	if (Serial.available ()) {
		c = Serial.read ();

		switch (c) {
		case 'x':
      running = 0;
      cylon_dir = 1;
      cylon_eye = 1;
			clear_strand ();
			break;
		case 'n':
      write_frame ();
			break;
		case 'c':
      step_cylon ();
			write_frame ();
			break;
		case 'z':
			setup_frame ();
			break;
		case 'v':
			setup_cylon ();
			break;
    case 'r':
      clear_strand ();
      cylon_dir =1;
      cylon_eye = 1;
      running = 1;
      break;
    }

		/* if (isxdigit (c)) { */
		/* 	acc = ((acc << 4) | hexval (c)) & 0xff; */
		/* 	Serial.println (buf); */
		/* } else { */
		/* 	switch (c) { */
		/* 	/\* case '.': *\/ */
		/* 	/\* 	sprintf (buf, "buffered: 0x%x", acc); *\/ */
		/* 	/\* 	Serial.println (buf); *\/ */
		/* 	/\* 	break; *\/ */
		/* 	/\* case ' ': *\/ */
		/* 	/\* 	sprintf (buf, "sending: 0x%x", acc); *\/ */
		/* 	/\* 	Serial.println (buf); *\/ */
		/* 	/\* 	SPI.transfer (acc); *\/ */
		/* 	/\* 	acc = 0; *\/ */
		/* 	/\* 	break; *\/ */
		/* 	case 'x': */
		/* 		clear_strand (); */
		/* 		break; */
		/* 	case '-': */
		/* 		start_frame (); */
		/* 		set_led (0x00, 0x00, 0x40, 0xf0); */
		/* 		set_led (0x00, 0x40, 0x00, 0xf0); */
		/* 		set_led (0x40, 0x00, 0x00, 0xf0); */
		/* 		set_led (0x40, 0x00, 0x00, 0xf0); */

		/* 		end_frame (); */
		/* 		break; */
		/* 	case 'z': */
		/* 		SPI.transfer (0xf0); */
		/* 		SPI.transfer (0x40); */
		/* 		SPI.transfer (0x00); */
		/* 		SPI.transfer (0x00); */
		/* 		break; */
		/* 	case 'c': */
		/* 		SPI.transfer (0x00); */
		/* 		SPI.transfer (0x00); */
		/* 		SPI.transfer (0x00); */
		/* 		SPI.transfer (0x00); */
		/* 		break; */
		/* 	} */
		/* } */
	}
}
