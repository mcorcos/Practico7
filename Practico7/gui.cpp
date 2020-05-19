#include "gui.h"




Gui::Gui() {

    inicializa_allegro();
    init_ImGui();
    threshold2 = 0;
    counter = 0;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    show_demo_window = true;
    show_main_window = true;
    running = true;
    show_compress_window = false;
    show_decompress_window = false;
    show_compress_window_last = false;
    loaded = false;
    pos_img_y = 100;
}
Gui::~Gui() {

    // Cleanup
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_bitmap(img);

}

void Gui::init_ImGui() {

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplAllegro5_Init(display);
}
void Gui::inicializa_allegro(void)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    display = al_create_display(1280, 720);
    queue = al_create_event_queue();

    al_set_window_title(display, "Dear ImGui Allegro 5 example");

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
}

void Gui::start_GUI() {
    while (running)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

        while (al_get_next_event(queue, &ev))
        {
            ImGui_ImplAllegro5_ProcessEvent(&ev);

            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                running = false;

            if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            {
                ImGui_ImplAllegro5_InvalidateDeviceObjects();
                al_acknowledge_resize(display);
                ImGui_ImplAllegro5_CreateDeviceObjects();
            }
        }


        al_clear_to_color(al_map_rgb_f(0, 0, 0));
        // Start the Dear ImGui frame
        ImGui_ImplAllegro5_NewFrame();
        ImGui::NewFrame();

        //if (show_demo_window)
        //    ImGui::ShowDemoWindow(&show_demo_window);
        if (show_main_window)
        {
            ImGui::Begin("MMB Compressor");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Select the operation to perform");               // Display some text (you can use a format strings too)

            if (ImGui::Button("Compress"))
            {
                show_main_window = false;
                show_compress_window = true;
            }

            ImGui::SameLine();

            if (ImGui::Button("Decompress"))
            {
                show_main_window = false;
                show_decompress_window = true;
            }
            ImGui::End();
        }

        if (show_decompress_window)
        {
            ImGui::Begin("Decompress");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Select the path of the images to decompress");               // Display some text (you can use a format strings too)

            ImGui::InputText("Select the directory path", direct_path, IM_ARRAYSIZE(direct_path));
            if (ImGui::Button("Ok")) {
                if (loaded == false)
                {
                    get_all(direct_path, ".EDA", png_files);
                    loaded = true;
                }
                for (int j = 0; j < png_files.size(); j++)
                {
                    if (!dcomp.decompress(png_files[0].string().c_str())) {
                        std::cout << "could not compress" << std::endl;
                    }
                }
                running = false; //quitar esto al dejar todo listo pipicucu
            }
            //lo que sea que haga con ok (conexion al backend);
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                show_main_window = false;
                show_compress_window = false;
                //ojo aca con la borrada de datos.
                running = false;
            }
            ImGui::End();
        }

        if (show_compress_window)
        {
            ImGui::Begin("Compress");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Select Parameters to compress");               // Display some text (you can use a format strings too)

            ImGui::InputText("Select the directory path", direct_path, IM_ARRAYSIZE(direct_path));
            if (ImGui::Button("Ok")) {
                show_compress_window = false;
                show_compress_window_last = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                show_main_window = false;
                show_compress_window = false;
                //ojo aca con la borrada de datos.
                running = false;
            }
            ImGui::End();
        }

        if (show_compress_window_last)
        {
            ImGui::Begin("Compress");
            ImGui::SliderInt("threshold", &threshold2, 0, 255);
            if (loaded == false)
            {
                get_all(direct_path, ".png", png_files);
                loaded = true;
            }
            for (int i = 0; i < png_files.size(); i++)
            {
            img = al_load_bitmap(png_files[i].string().c_str()); //recordar destruir estos objetos en el destructor. ademas deberia ser parte de la clase este que sera un vector de allegro bitmaps
            if ((i) % 10 == 0 && i != 0) //+1 para que no me tome el cero
            {
                pos_img_y += 100;
            }
            al_draw_scaled_bitmap(img, 0, 0, al_get_bitmap_width(img), al_get_bitmap_height(img), 
                                    100*(i%10), pos_img_y, 100, 100, NULL);
            }
            pos_img_y = 100;
            if (ImGui::Button("Ok")) {
                compr.setthreshold(threshold2);
                for (int j = 0; j < png_files.size(); j++) {
                    if (!compr.compress(png_files[j].string().c_str(), "img.EDA")) {
                        std::cout << "could not compress" << std::endl;
                    }
                }
                running = false; 
            }
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        al_flip_display();
    }
}


void get_all(const fs::path& root, const string& ext, vector<fs::path>& ret)
{
    if (!fs::exists(root) || !fs::is_directory(root)) return;

    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator endit;

    while (it != endit)
    {
        if (fs::is_regular_file(*it) && it->path().extension() == ext) ret.push_back(it->path().filename());
        ++it;

    }

}