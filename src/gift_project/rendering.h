#include <stdio.h>
#include <stdlib.h> 
#include <vector>

enum ElementType { text68, text816,picture };

class Element { 

    public: Element(int firstpage,int lastpage,int startcol,bool (*generatorfunction)(int,int,int,int))  {
      this->firstpage = firstpage;
      this->lastpage = lastpage;
      this->generatorfunction = generatorfunction; 
      this->startcol = startcol;
      this->source = source; 
    }
      

    public: Element(int firstpage,int lastpage,int startcol,char * source)  {
      this->firstpage = firstpage;
      this->lastpage = lastpage;
      this->generatorfunction = NULL; 
      this->startcol = startcol;
      this->source = source; 
    };
    private: int firstpage;
    private: int lastpage;
    private: int startcol;
    private: char * source; 
    private: bool (*generatorfunction)(int,int,int,int);
    public: bool IsInPage(int page);
    public: void renderOntoBuffer(char * buffer,int pagenumber);


};

class OneWholeDisplayScreen {

  public: OneWholeDisplayScreen(std::vector<Element> elements) { this->elements = elements; } 
  public: OneWholeDisplayScreen()  {} 

  public: std::vector<Element> elements;

  public: bool renderPage(int page,char *pagebuffer);

};

class DisplaySequenceElement { 

  public: DisplaySequenceElement( OneWholeDisplayScreen screen, bool blink, int displayTimeMS) {
     this->screen = screen;
     this->blink  = blink;  
     this->displayTimeMS = displayTimeMS;  
  };   


  OneWholeDisplayScreen screen; 
  bool blink; 
  int displayTimeMS; 

}; 