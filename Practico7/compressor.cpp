#include "compressor.h"




compressor::compressor() {
	image_code.clear();
	w = 0;
	h = 0;
	img = nullptr;
	threshold = 1;
};
compressor::~compressor() {

};

bool compressor::compress(const char* img_in, const char* img_out) {

	decode_img(img_in);

	compress_img(img, image_code, w, h);
	save_file(img_out);
	return true;
}



void compressor::decode_img(const char* img_in) {

	unsigned error = lodepng_decode32_file(&img, &w, &h, img_in);

	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	image_code.push_back((int)log2(w));
}

void compressor::compress_img(unsigned char* img, vector<unsigned char>& image_code, unsigned int w, unsigned int h) {

	struct prom data = check_data(img, w, h);


	if (data.puntaje <= threshold) {
		image_code.push_back(0);
		image_code.push_back(data.avR);
		image_code.push_back(data.avG);
		image_code.push_back(data.avB);
		return;
	}

	if (w == 1 || h == 1) {
		image_code.push_back(0);
		image_code.push_back(data.avR);
		image_code.push_back(data.avG);
		image_code.push_back(data.avB);
		return;
	}

	image_code.push_back(1);
	w /= 2;
	h /= 2;
	compress_img(img, image_code, w , h );
	compress_img(img + 4 * (w ), image_code, w , h );
	compress_img(img + 4 * ((this->w) * (h)), image_code, w, h);
	compress_img(img + 4 * ((this->w) * (h) + (w)), image_code, w, h);



}

struct prom compressor::check_data(unsigned char* img, unsigned int width, unsigned int h) {

	struct prom temp;

	unsigned char maxR = 0;
	unsigned char maxG = 0;
	unsigned char maxB = 0;
	unsigned char minR = 255;
	unsigned char minG = 255;
	unsigned char minB = 255;
	unsigned int promR = 0, promG = 0, promB = 0;
	temp.avR = 0;
	temp.avG = 0;
	temp.avB = 0;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < h; j++) {

			int offset = (j + i * (this->w)) * PSIZE;

			//take the max and min values
			if (maxR < img[offset]) {
				maxR = img[offset];
			}
			if (maxG < img[offset + 1]) {
				maxG = img[offset + 1];
			}
			if (maxB < img[offset + 2]) {
				maxB = img[offset + 2];
			}
			if (minR > img[offset]) {
				minR = img[offset];
			}
			if (minG > img[offset + 1]) {
				minG = img[offset + 1];
			}
			if (minB > img[offset + 2]) {
				minB = img[offset + 2];
			}

			//take the sum of color RGB
			promR += img[offset];
			promG += img[offset + 1];
			promB += img[offset + 2];
		}
	}

	//take the average of color RGB

	//promR /= (width * h);
	//promG /= (width * h);
	//promB /= (width * h);

	temp.avR = (unsigned char)(promR / (width * h));
	temp.avG = (unsigned char)(promG / (width * h));
	temp.avB = (unsigned char)(promB / (width * h));

	temp.puntaje = maxR - minR + maxG - minG + maxB - minB;

	return temp;
}



void compressor::save_file(const char* img_out) {

	std::ofstream MyFile;
	MyFile.open(img_out, ios::binary);		//creo archivo 

	if (MyFile.is_open())		//verifico si se creo y puedo abrirlo
	{

		for (const auto& e : image_code) {

			MyFile << e;
		}
		MyFile.close();
	}
	else
	{
		cout << "ERROR CREANDO ARCHIVO" << endl;
	}
}




