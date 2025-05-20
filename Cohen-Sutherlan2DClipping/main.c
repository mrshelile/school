#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Window boundaries
const double xmin = 50,ymin=10.0;
const double xmax = 80.0,ymax=40.0;

// Region codes
#define INSIDE 0   //0000
#define LEFT   1   //0001
#define RIGHT  2   //0010
#define BOTTOM 4   //0011
#define TOP    8   //0110

// Prototypes

//Compute the outcode for point(x,y)
// Prototype function
int ComputeOutCod(double x, double y);

//Cohen-Sutherland clipping algo prototype function
bool CCohenSutherLandLineClipping(double x0,double y0,
                                  double x1,double y1);

int main()
{   bool value = CCohenSutherLandLineClipping(70.0,20.0,100.0,10.0);

    return 0;
}

//Compute the outcode for point(x,y)
int ComputeOutCod(double x, double y){
int code = INSIDE;

if (x<xmin) code |=LEFT;
else if(x>xmax) code |=RIGHT;

if(y < ymin) code |=BOTTOM;
else if (y>ymax) code |=TOP;

return code;
}

//Cohen-Sutherland clipping algorithm
bool CCohenSutherLandLineClipping(double x0,double y0,
                                  double x1,double y1)
{
 int outcode0 = ComputeOutCod(x0,y0);
 int outcode1 = ComputeOutCod(x1,y1);
 printf("---initial coordinates--- P1(%f,%f) and P2(%f,%f)\n",x0,y0,x1,y1);
 bool accept = false;

 while(true){
        // if both endpoints lie within rectangle

    if((outcode0 | outcode1)==INSIDE){
        accept = true;
        break;

    }

    // if both endpoints lie outside rectangle
     // in same region
    else if((outcode0 & outcode1)){
        break;
    }
 // some segment of line within  rectangle
    else{
        int code_out;
        double x,y;

        //atleast one point is outside the
        //rectangle ,pick it.

        if(outcode0 !=0)
            code_out =outcode0;

        else
            code_out = outcode1;

        //find the intersection point
        //using formular y = y1 + slope * (x-x1)
       // x = x1 + (1/slope) * ( y - y1 )

        if(code_out & TOP){
            // point is above the clip rectangle
            x = x0 + (x1-x0) * (ymax - y0) / (y1 -y0);
            y=ymax;
        }
        else if(code_out & BOTTOM){
            // point is below the rectangle
            x= x0 + (x1 -x0) * (ymin - y0) / (y1 -y0);
            y = ymin;
        }
        else if(code_out & RIGHT){
            //point is to the right of rectangle
 printf("Yes\n");
            y = y0 +(y1-y0) * (xmax - x0) /(x1 - x0);
            x=xmax;
        }
        else if(code_out =LEFT){

            // point is to left of rectangle
            y = y0 +(y1-y0) * (xmin -x0) / (x1-x0);
            x =xmin;
        }

        // now the intersection point x,y is found
        // we replace point outside rectangle
        // by intersection point

        if(code_out == outcode0){

            x0=x;
            y0=y;
            outcode0 = ComputeOutCod(x0,y0);
        }
        else{
             x1 =x;
             y1=y;

             outcode1 = ComputeOutCod(x1,y1);
        }
    }

 }

 if(accept){

    printf("Line accepted from (%f,%f) to (%f,%f)",x0,y0,x1,y1);

 }
 else{
    printf("Line rejected");
 }
   return accept;
}
