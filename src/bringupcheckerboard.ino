
/*
 *  Roger D. Pease 3 Jan 2025. 
 * 
 *  This is a "working bringup" sequence to display a checkerboard on a SSD1315 display from MakerFocus on Amazon with an Seeedstudio Xiao. 
 *  Based on https://github.com/tejashwikalptaru/ssd1306xled/blob/master/ssd1306xled.cpp
 * 
 */ 


#include <Wire.h>
const uint8_t ssd1315_init_sequence [] PROGMEM = {      // Initialization Sequence^M
        0x00,             // Tells the system a command (rather than data) is coming next. 
                          // Display powered up unreliably without this. 
        0xAE,             // Set Display ON/OFF - AE=OFF, AF=ON
        0xD5, 0x40,       // Set display clock divide ratio/oscillator frequency, set divide ratio
                          // I was getting flickering with 0xD5 0x90
        0xA8, 0x3F,       // Set multiplex ratio (1 to 64) ... (height - 1)
        0xD3, 0x00,       // Set display offset. 00 = no offset
        0x40 | 0x00,      // Set start line address, at 0
       
        0x20, 0x02,       // Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, 10=Page, 11=Invalid
        0xA0 | 0x01,      // Set Segment Re-map
        0xC8,             // Set COM Output Scan Direction
        0xDA, 0x12,       // Set COM Pins Hardware Configuration - 128x32:0x02, 128x64:0x12
        0x81, 0x30,       // Set contrast control register
        0xD9, 0xF1,       // Set pre-charge period (0x22 or 0xF1)
        0xDB, 0x20,       // Set Vcomh Deselect Level - 0x00: 0.65 x VCC, 0x20: 0.77 x VCC (RESET), 0x30: 0.83 x VCC
        0xA4,             // Entire Display ON (resume) - output RAM to display
        0xA6,             // Set Normal/Inverse Display mode. A6=Normal; A7=Inverse
        0x8D, 0x14,       // Charge Pump Setting, 14h = Enable Charge Pump

        0x2E,             // Deactivate Scroll command
        0xAF,             // Set Display ON/OFF - AE=OFF, AF=ON
};

                                


#define DEVICEID (0x3C)

void setup() {


  Serial.begin(9600);
  Wire.begin(); 
  Wire.setClock(1000000);

  Wire.beginTransmission(DEVICEID); 
  int i;
  for (i = 0; i< sizeof(ssd1315_init_sequence);i++)
      Wire.write(ssd1315_init_sequence[i]);          
  Wire.endTransmission();    
  delay(500);


}


void ssd1315_setpos(uint8_t x, uint8_t y)
{

  Wire.setClock(1000000);
  Wire.beginTransmission(DEVICEID); 
  Wire.write(0x0);//        ssd1306_send_command_start();^M
  Wire.write(0xb0 | (y & 0x07));
  Wire.write(0x10 | ((x & 0xf0) >> 4));
  Wire.write(x & 0x0f); // | 0x01^M
  Wire.endTransmission();

}


  int count = 0;


void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Writing Checkerboard\n");
 
  char buffer[128];
 
  int page; 

  for (page = 0;page < 8;page ++)
  {  
    // Checkerboard pattern.
    for (int col = 0; col < 128;col++)
    {
      uint8_t oddcol = (col>>4) & 1; // Never use mod in embedded systems it's very expensive.  
      uint8_t oddpage = page & 1; 
      buffer[col] = 0xFF;
      if ((oddcol + oddpage) == 1) buffer[col] = 0x00;
    }
    ssd1315_setpos(0,page);
    Wire.beginTransmission(DEVICEID); // transmit to device #4
    Wire.write(0x40); // Control byte saying data is about to come. 
    Wire.write(buffer,128);
    Wire.endTransmission(); 

  }

  Serial.print("Wrote Checkerboard\n");
  Serial.println(count++);
  delay(1000);

}

