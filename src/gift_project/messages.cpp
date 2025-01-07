#include "rendering.h" 
#include <vector>

bool isInHeartWrapper(int row, int rows, int col, int cols);


std::vector<DisplaySequenceElement> GenerateDisplayScreens() 
{

  std::vector<DisplaySequenceElement>  DisplaySequenceElements = { 

    DisplaySequenceElement(
        OneWholeDisplayScreen({
              Element(0,1,0,(char *) "%%   Happy    %"),
              Element(2,3,0,(char *) "% Valentine's %"),
              Element(4,5,0,(char *) "%%%  Day!   %%%"),
              Element(6,7,0,(char *) "%%           %%")
           }),false,5000 ), 

   DisplaySequenceElement( OneWholeDisplayScreen({Element(0,7,0,isInHeartWrapper)}),true,5000)

  };  

  return DisplaySequenceElements;

}

 
