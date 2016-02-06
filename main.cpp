#include <iostream>
#include "/usr/include/ImageMagick/Magick++.h"
#include <iostream>

using namespace std;
using namespace Magick;
unsigned long createRGBA(int r, int g, int b, int a) {
    //http://stackoverflow.com/a/14375308
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
           + (a & 0xff);
}
int main()
{
    //Image processing from stack overflow post,
    //http://stackoverflow.com/questions/27877163/rgba-values-to-2d-array-using-c
    //http://stackoverflow.com/questions/27877163/rgba-values-to-2d-array-using-c
    Image image("a.png");
    int w = image.columns();
    int h = image.rows();
    PixelPacket *pixels = image.getPixels(0, 0, w, h);

    int row = 0;
    int column = 0;


    for(int i = 0; i < w*h; i++){
        Color color = pixels[i]; // get first pixel color as an example

        float red = color.redQuantum();
        float blue = color.blueQuantum();
        float green = color.greenQuantum();
        float alpha = color.alphaQuantum();

        //translate the bit value into standard rgba(255,255,255) values
        if (red != 0){ red = red/256;}  //if the (r)gba  vector is 0, don't divide by 256
        if (blue != 0){ blue = blue/256;} //if the r(g)ba  vector is 0, don't divide by 256
        if (green !=0) { green = green/256;}//if the rg(b)a  vector is 0, don't divide by 256
        if (alpha !=0) { alpha = alpha/256;}//if the rgb(a)  vector is 0, don't divide by 256

        //output red,green,blue values

        cout << "R: " << red << " G: " << green << " B :" << blue << " A:" << alpha << " : "
	     << std::hex << createRGBA(red,green,blue,alpha) << endl;


    }

    return 0;
}

// Following documentation @http://www.imagemagick.org/Magick++/, the command to compile is
//c++ -O2 -o example main.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -std=gnu++11
