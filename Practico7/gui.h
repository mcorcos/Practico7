
#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_allegro5.h"
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "compressor.h"
#include "decompresor.h"
#include <boost/filesystem.hpp>

#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
namespace fs = ::boost::filesystem;

void get_all(const fs::path& root, const string& ext, vector<fs::path>& ret);

class Gui {
public:
	Gui();
	~Gui();
	void start_GUI();
private:
	void inicializa_allegro();
	void init_ImGui();

	ALLEGRO_EVENT ev;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_BITMAP* img;
	vector <boost::filesystem::path> png_files;
	int threshold2;
	int counter;
	char direct_path[200] = "";
	ImVec4 clear_color;
	bool show_demo_window;
	bool show_main_window;
	bool running;
	bool show_compress_window;
	bool show_decompress_window;
	bool show_compress_window_last;
	bool loaded;
	compressor comp;
	decompressor dcomp;

};