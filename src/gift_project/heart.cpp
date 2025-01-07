#include <math.h> 
#include <cstdio>


//
// Do a Google search for "Heart Math equation" and you will
// probably find several. Among them, (x^2+ (1.35y-(x^2)^(1/3))) = 1
// Anything < 1 is in the heart. Anything > 1 is outside the heart. 
//

int isInHeart(float y, float x)
{
   if (pow(x,2.0)+pow(1.35*y-pow(pow(x,2.0),0.3333),2) > 1) return 0; 
   return 1; 
}


float row_to_x(int row, int rows) { return (float) 1.5*(rows/2.0-row)/(rows/2.0); }  
float col_to_y(int col, int cols) { return (float) 1.5*(col-cols/2.0)/(cols/2.0); }  

bool isInHeartWrapper(int row, int rows, int col, int cols)
{
   return (isInHeart(row_to_x(row,rows),col_to_y(col,cols)));
} 

#ifdef UNITTEST_HEART
int main() 
{
  const int rows=32; 
  const int cols=64; 

  for (int row = 0; row < rows; row ++) 
  {
     for (int col = 0; col < cols; col ++) 
     { 
        if (isInHeartWrapper(row,rows,col,cols)) { printf("*"); } else { printf(" "); }  
     } 
     printf("\n"); 
  } 

}
#endif 
