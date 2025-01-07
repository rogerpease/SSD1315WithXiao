//
//  Roger D. Pease Jan 4, 2025 
//
// In a "normal" production environment I'd build a test collar. For something this minor it's not worth it. 
//

#include <cstdint>
#include "font8x16.h"
#include "font6x8.h"
#include "rendering.h"
#include <string.h>

#define ROWSPERPAGE (8) 

//
//  Convert "Hello World" to  
//               **   **
//               **   ** 
//               **   ** *****
//               ******* *   *
//               **   ** *****
//               **   ** *      (you get the idea)
//               **   ** *****  
//
//  page_offset is 0 (top page pixels) or 1 (bottom page pixels)
//

void string_to_816_buffer(char * string, int stringlen,int page_offset, int startcol, char * pagebuffer)
{
   int pageposition = startcol;
   const int fontwidthpixels = 8;
   const int fontwidthdata = 16;
   int fontdataoffset = 0;
   if (page_offset == 1) fontdataoffset = 8; 

   for (int i = 0; i < stringlen;i++)
   {
     char character = string[i];
     int charindex = (int) character - 32;

     for (int fontpixelcolumn = 0; fontpixelcolumn < fontwidthpixels;fontpixelcolumn++)
     {
       *(pagebuffer+pageposition++) = ssd1306xled_font8x16[charindex*fontwidthdata + fontpixelcolumn+fontdataoffset];
     }
   }

}


bool OneWholeDisplayScreen::renderPage(int page,char *pagebuffer)
{

      std::vector<Element>::iterator it; 
      for(it = elements.begin(); it != elements.end(); it++ )    
        if (it->IsInPage(page))
          it->renderOntoBuffer(pagebuffer,page);
      return true;

}

bool Element::IsInPage(int page) { if ((page <firstpage) || (page > lastpage)) return false; return true;}

void Element::renderOntoBuffer(char * pagebuffer,int pagenumber)
{
  if (this->generatorfunction == NULL) 
    string_to_816_buffer(this->source, strlen(this->source),pagenumber - this->firstpage, this->startcol,pagebuffer);
  else  
  { 
    int numrowsinimage = (this->lastpage-this->firstpage+1)*ROWSPERPAGE; 
    int numcolsinimage = numrowsinimage*2; 
    for (int col_of_image=0;col_of_image < numcolsinimage; col_of_image++)    
     { 
        int bits = 0; 
        for (int row_of_page=0;row_of_page < ROWSPERPAGE; row_of_page++)    
        { 
          bits |= (*this->generatorfunction)(
                    row_of_page + (pagenumber - this->firstpage)*8,
                    numrowsinimage,
                    col_of_image,
                    numcolsinimage) << row_of_page; 
        } 
        pagebuffer[this->startcol+col_of_image] = bits; 
     } 
  } 

}

#ifdef UNITTEST

// Compile with: g++ rendering.cpp heart.cpp -DUNITTEST
bool isInHeartWrapper(int row, int rows, int col, int cols);

bool isInHeartWrapperInv(int row, int rows, int col, int cols) { return 1-isInHeartWrapper(row,rows,col,cols); }

int main()
{

  OneWholeDisplayScreen MyFirstDisplay({
                      Element(0,1,0,(char *) "%%   Happy    %%"),
                      Element(2,3,0,(char *) "%%   VD   %%"),
                      Element(4,5,0,(char *) "%%   Day %%"),
                      Element(6,7,0,(char *) "%%   World %%")}); 

  OneWholeDisplayScreen MySecondDisplay({
                      Element(0,1,0,(char *) "%%   I love   %%"),
                      Element(2,5,0,isInHeartWrapper),
                      Element(2,5,64,isInHeartWrapperInv),
                      Element(6,7,0,(char *) "%%    you!    %%")}); 

  OneWholeDisplayScreen MyThirdDisplay({Element(0,7,0,isInHeartWrapper)}); 

  std::vector<OneWholeDisplayScreen> mydisplays = { 
                                           MyFirstDisplay,
                                           MySecondDisplay, 
                                           MyThirdDisplay}; 

  std::vector<OneWholeDisplayScreen>::iterator iter; 
  char buffer[128]; 


  for (iter = mydisplays.begin(); iter != mydisplays.end(); iter++) 
  {  
    for (int page = 0;page < 8;page ++)
    {  
      memset(buffer,0,128);
      iter->renderPage(page,buffer);
      for (int row = 0;row<8;row++) 
      {
        for (int i = 0;i<128;i++) 
          if (buffer[i] & (1<<row)) printf("*"); else printf(" ");
        printf("\n");
      }
    } 
  } 

  return 0;


}
#endif
