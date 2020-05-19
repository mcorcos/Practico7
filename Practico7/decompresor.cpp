
#include "decompresor.h"



decompressor::decompressor() {
	myfile = nullptr;
	DecImage = nullptr;
	width = 0;
};
decompressor::~decompressor() {

};


bool decompressor::decompress(const char* compressed_img) {

	bool state = false;

	get_filecode(compressed_img);

	string decompresed_name = create_decompressed_name(compressed_img);

	recDecompressor(DecImage, 0);


	lodepng_encode32_file(decompresed_name.c_str(), DecImage, width, width);

	myfile->close();
	delete[] DecImage;
	delete myfile;
	state = true;

	return state;
}


void decompressor::get_filecode(const char* compressed_img) {

	myfile = new ifstream(compressed_img, ios::binary);

	if (myfile->is_open())
	{
		unsigned char exp = myfile->get();

		width = (unsigned int)pow(2, exp);

		unsigned int height = width;

		DecImage = new unsigned char[width * height * PSIZE];

	}
	else
		cout << "No se pudo abrir el archivo";

}

string decompressor::create_decompressed_name(const char* compressed_img)
{
	string compressed_img_string = compressed_img;
	compressed_img_string.replace(compressed_img_string.length() - strlen(".EDA"), strlen(".EDA"), "_nuevo.png");	//cambia la terminacion .var por .png elegida para el archivo comprimido
	return compressed_img_string;
}


void decompressor::recDecompressor(unsigned char* DecImage, unsigned int level)
{
	unsigned char c = myfile->get();
	unsigned int num = (unsigned int)pow(2, level + 1);

	if (c == 1)
	{
		//a continuacion las llamadas a las funciones para cada cuadrante recorriendolos de izquierda a derecha de arriba abajo.
		recDecompressor(DecImage, level + 1);
		recDecompressor(DecImage + PSIZE * (width / num), level + 1);
		recDecompressor(DecImage + PSIZE * (unsigned int)pow(width, 2) / num, level + 1);
		recDecompressor(DecImage + PSIZE * width / num + PSIZE * (unsigned int)pow(width, 2) / num, level + 1);
	}
	else if (c == 0)
	{
		unsigned char red = myfile->get();
		unsigned char green = myfile->get();
		unsigned char blue = myfile->get();
		for (int fila = 0; fila < width / pow(2, level); fila++)
		{
			for (int col = 0; col < width / pow(2, level); col++)
			{
				DecImage[PSIZE * col + fila * width * PSIZE] = red;
				DecImage[PSIZE * col + fila * width * PSIZE + 1] = green;
				DecImage[PSIZE * col + fila * width * PSIZE + 2] = blue;
				DecImage[PSIZE * col + fila * width * PSIZE + 3] = ALPHA;
			}
		}
	}
	else
	{
		cout << "Error algo salio mal durante el proceso." << endl;
	}
}


