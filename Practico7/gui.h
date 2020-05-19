
#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_allegro5.h"
#include <stdio.h>

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
	float threshold2;
	int counter;
	char direct_path[200] = "";
	ImVec4 clear_color;
	bool show_demo_window;
	bool show_main_window;
	bool running;
	bool show_compress_window;
	bool show_decompress_window;

};