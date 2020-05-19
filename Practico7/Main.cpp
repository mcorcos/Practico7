#include <iostream>
#include "compressor.h"
#include "decompresor.h"
//#include "gui.h"


// dear imgui: standalone example application for Allegro 5
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.




int main(void)
{
    const char* img_in = "img1.png";
    const char* img_compressed = "img.EDA";

    compressor comp;
    decompressor dcomp;

    if (!comp.compress(img_in, img_compressed)) {
        std::cout << "could not compress" << std::endl;
        return -1;
    }
    if (!dcomp.decompress(img_compressed)) {
        std::cout << "could not decompress" << std::endl;
        return -1;
    }

    return 0;
}



