#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "lodepng.h"

using namespace std;

#define PSIZE (unsigned char)4
#define ALPHA 255
#define EXTENSION_L 3

struct prom {

	unsigned char avR;
	unsigned char avG;
	unsigned char avB;
	int puntaje;
};

class compressor {
public:

	compressor();
	~compressor();
	bool compress(const char* img_in, const char* img_out);
private:
	void decode_img(const char* img_in);
	void compress_img(unsigned char* img, unsigned int w, unsigned int h, vector<unsigned char>& image_code);
	struct prom check_data(unsigned char* img, unsigned int w, unsigned int h);
	void save_file(const char* img_out);

	unsigned int w, h;
	unsigned char* img;
	int threshold;
	std::vector<unsigned char> image_code;
};