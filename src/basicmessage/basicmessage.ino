
/*
 *  Roger D. Pease 3 Jan 2025. 
 *  Free to use and modify. Tested on a SSD1315 with a Seeedstudio Xiao. 
 *  Font leveraged from https://github.com/tejashwikalptaru/ssd1306xled/blob/master/font8x16.h
 */ 


#include "font6x8.h" 
#include <Wire.h>

const uint8_t ssd1315_init_sequence [] PROGMEM = {      // Initialization Sequence^M
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

                                

#define COMMAND  (0x0)
#define DATA     (0x40)
#define DEVICEID (0x3C)

void setup() {


  Serial.begin(9600);
  Wire.begin(); 
  Wire.setClock(1000000);

  Wire.beginTransmission(DEVICEID); 
  Wire.write(COMMAND);
  for (int i = 0; i< sizeof(ssd1315_init_sequence);i++)
      Wire.write(ssd1315_init_sequence[i]);          
  Wire.endTransmission();    
  delay(500);


}


void ssd1315_setpos(uint8_t column, uint8_t page)
{

  Wire.beginTransmission(DEVICEID); 
  Wire.write(COMMAND);
  Wire.write(0xb0 | (page & 0x07));
  Wire.write(0x10 | ((column & 0xf0) >> 4));
  Wire.write(column & 0x0f); 
  Wire.endTransmission();

}


int count = 0;

void string_to_68_buffer(char * string, int stringlen, char * page)
{
   int pageposition = 0;
   const int fontwid = 6;
   for (int i = 0; i < stringlen;i++)
   {
     char character = string[i];
     int charindex = (int) character - 32; 
       
     for (int fontcol = 0; fontcol < fontwid;fontcol++) 
     {
       *(page+pageposition++) = ssd1306xled_font6x8[charindex*fontwid + fontcol]; 
     } 
   } 
   // clean up to rest of line. 
   while (pageposition < 128) { *(page+pageposition++) = 0;}
}

char strings[][24] = 
       { 
        "This is a basic",
        "test message to see", 
        "how this font looks",
        "on the display.", 
        "Note that we have", 
        "to clean the end of",
        "the line or the ",
        "screen shows junk!"
        };

char buffer[128];
int page; 

void loop() {

  for (page = 0;page < 8;page ++)
  {  

    string_to_68_buffer(strings[page], strlen(strings[page]), buffer);

    ssd1315_setpos(0,page);
    Wire.beginTransmission(DEVICEID); // transmit to device #4
    Wire.write(DATA); // Control byte saying data is about to come. 
    Wire.write(buffer,128);
    Wire.endTransmission(); 

  }

  delay(1000);

}

