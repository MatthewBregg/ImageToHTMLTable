imageToHTMLTable: main.cpp
	c++ -O2 -o imageToHTMLTable main.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -std=gnu++11
