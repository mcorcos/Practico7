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
	void setthreshold(int);
	bool compress(const char* img_in);
private:
	void decode_img(const char* img_in);
	void compress_img(unsigned char* img, vector<unsigned char>& image_code, unsigned int w, unsigned int h);
	struct prom check_data(unsigned char* img, unsigned int w, unsigned int h);
	void save_file(const char* img_out);

	string create_compressed_name(const char* compressed_img);
	//bool ok_data(const char* img_in);
	unsigned int w, h;
	unsigned char* img;
	int threshold;
	std::vector<unsigned char> image_code;
};