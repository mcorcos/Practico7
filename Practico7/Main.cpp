#include <iostream>
#include "compressor.h"
#include "decompresor.h"
#include "gui.h"


// dear imgui: standalone example application for Allegro 5
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.




int main(void)
{
    compressor compr;
    decompressor dcomp;
    Gui gui;

    gui.start_GUI();

    return 0;
}



