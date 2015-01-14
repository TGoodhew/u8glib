// Main.cpp : Defines the entry point for the console application.
//

// Include files
//
// This solution assumes that you have local clone of the u8glib repository.
// You will need to add these directories to your Additional Include Directories property
//
// u8glib\cppsrc; 
// u8glib\csrc; 
//
#include "stdafx.h"
#include "arduino.h"
#include "U8glib.h"

//  U8GLIB_LM6059(sck, mosi, cs, a0 [, reset]) 
U8GLIB_LM6059 u8g(13, 11, 8, 10, 9);
//U8GLIB_LM6059_2X u8g(13, 11, 8, 10, 9);

int _tmain(int argc, _TCHAR* argv[])
{
	return RunArduinoSketch();
}

void draw(void) {
	// graphic commands to redraw the complete screen should be placed here  
	u8g.setFont(u8g_font_unifont);
	//u8g.setFont(u8g_font_osb21);
	u8g.drawStr(0, 22, "Hello World!");
}

void setup(void) {

	// flip screen, if required
	// u8g.setRot180();

	// set SPI backup if required
	//u8g.setHardwareBackup(u8g_backup_avr_spi);

	// assign default color value
	if (u8g.getMode() == U8G_MODE_R3G3B2) {
		u8g.setColorIndex(255);     // white
	}
	else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
		u8g.setColorIndex(3);         // max intensity
	}
	else if (u8g.getMode() == U8G_MODE_BW) {
		u8g.setColorIndex(1);         // pixel on
	}
	else if (u8g.getMode() == U8G_MODE_HICOLOR) {
		u8g.setHiColorByRGB(255, 255, 255);
	}
}

void loop(void) {
	// picture loop
	u8g.firstPage();
	do {
		draw();
	} while (u8g.nextPage());

	// rebuild the picture after some delay
	delay(50);
}