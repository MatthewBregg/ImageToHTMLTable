#include <iostream>
#include "/usr/include/ImageMagick/Magick++.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <set>

using namespace std;
using namespace Magick;
unsigned long createRGBA(int r, int g, int b, int a) {
    //http://stackoverflow.com/a/14375308
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
           + (a & 0xff);
}
unsigned long createRGB(int r, int g, int b)
{
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}
using Image2dVector = std::vector<std::vector<unsigned long> >;
Image2dVector processImage2Vector(Image image)
{
    //Image processing from stack overflow post,
    //http://stackoverflow.com/questions/27877163/rgba-values-to-2d-array-using-c
    //http://stackoverflow.com/questions/27877163/rgba-values-to-2d-array-using-c
    int w = image.columns();
    int h = image.rows();
    PixelPacket *pixels = image.getPixels(0, 0, w, h);

    int row = 0;
    int column = 0;

    Image2dVector imageVector;
    int count = 0;
    for(int i = 0; i < h; i++) {
	imageVector.push_back(std::vector<unsigned long>());
	for (int j = 0; j < w; ++j) {
	    ++count;
	    Color color = pixels[i*w+j]; // get first pixel color as an example

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

	    //cout << "R: " << red << " G: " << green << " B :" << blue << " A:" << alpha << " : "
	    //<< std::hex << createRGB(red,green,blue) << endl;

	    imageVector.back().push_back(createRGB(red,green,blue));


	}
    }
    cout << std::dec << "Pix count : " << count  << std::endl;

    return imageVector;
}

int main() {
    Image image("a.png");

    Image2dVector imageVector = processImage2Vector(image);


    std::vector<std::string> table;
    //Set up the styling
    std::set<std::string> styles;
    const std::string initial_style =
	"td, tr {" "\n"
	"height: 2px;" "\n"
	"width: 2px;" "\n"
	"}" "\n"
	"table {" "\n"
	"border-collapse: collapse;" "\n"
	"border: none;" "\n"
	"}" "\n";
    table.push_back(std::string("<table>"));
    for ( const auto &i : imageVector ) {
	std::stringstream row;
	row << "<tr>";
	for ( auto j : i ) {
	    std::stringstream stream;
	    stream << std::hex << j;
	    std::string hexColor(stream.str());
	    for ( auto &c : hexColor ) {
		c = ::toupper(c);
	    }
	    while( hexColor.size() != 6 ) {
		hexColor = "0"+hexColor;
	    }
	    styles.insert( ".X" + std::to_string(j) + " { background-color: #" + hexColor + "; } \n");
	    //HTML names can't start with a number, sigh...
	    //Adding an X works, although it some 5-10% to the size...
	    //Oh well, if I really wanted, could map 0-9 to an alpha character for the first char, and save a tiny bit
	    //of room.
	    //But I don't want to.

	    row << "<td class=\"X"  << j << "\">"<<  "</td>" << std::endl;
	}
	row << "</tr>" << std::endl;
	table.push_back(row.str());
    }
    table.push_back(std::string("</table>"));

    std::ofstream output;
    output.open("output.html");

    output << "<style> \n";
    output << initial_style;
    for ( const auto &i : styles ) {
	output << i << std::endl;
    }
    output << "</style> \n";


    for ( const auto &i : table ) {
	//Write the table to i
	output << i;
    }
    output.close();

}
// Following documentation @http://www.imagemagick.org/Magick++/, the command to compile is
//c++ -O2 -o example main.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -std=gnu++11
