#include <iostream>
 
using namespace std;

class Result{

   public:

      virtual int getArea() = 0;

      void setWidth(int w){

         width = w;
      }

      void setHeight(int h){

         height = h;
      }
   protected:

      int width;
      int height;
};

class ResultInt: public Result{

   public:

      int getArea()
      { 
         return (width * height); 
      }
};

class ResultDouble: public Result{

   public:

      int getArea()
      { 
         return (width * height)/2; 
      }
};