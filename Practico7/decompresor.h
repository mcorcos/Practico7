#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "lodepng.h"

using namespace std;

#define PSIZE (unsigned char)4
#define ALPHA 255
#define EXTENSION_L 3


class decompressor {
public:

	decompressor();
	~decompressor();
	bool decompress(const char* compressed_img);
private:
	void recDecompressor(unsigned char* DecImage, unsigned int level);
	void get_filecode(const char* compressed_img);

	ifstream* myfile;
	unsigned int width;
	unsigned char* DecImage;
	string decompressedFile;
};
