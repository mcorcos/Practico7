#include "gui.h"


Gui::Gui() {

    inicializa_allegro();
    init_ImGui();
    threshold2 = 0;
    counter = 0;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    show_demo_window = true;
    show_main_window = true;
    running = true;
    show_compress_window = false;
    show_decompress_window = false;
    show_compress_window_last = false;
    loaded = false;
    onetime = false;
    checkbox = false;
    show_decompress_window_last = false;
    pos_img_y = 400;
    vec_imgs.clear();
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

    al_set_window_title(display, "MMB Compressor");

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
}



void Gui::start_GUI() {
    while (running)
    {

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
                //if (loaded == false)
                //{
                //    get_all(direct_path, ".EDA", EDAvec);
                //    loaded = true;
                //}
                show_decompress_window = false;
                show_decompress_window_last = true;

                //for (int j = 0; j < png_files.size(); j++)
                //{
                //    if (!dcomp.decompress(png_files[j].string().c_str())) {
                //        std::cout << "could not compress" << std::endl;
                //    }
                //}
                //running = false;
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


        if (show_decompress_window_last) {


            ImGui::Begin("Decompress");
            if (onetime == false)
            {
                get_all(direct_path, ".EDA", EDAvec);
                checkboxesEDA.resize(EDAvec.size());
                for (int k = 0; k < EDAvec.size(); k++)
                {
                    checkboxesEDA.at(k) = false;
                }
                onetime = true;
            }

            for (int j = 0; j < EDAvec.size(); j++) {

                bool c = checkboxesEDA.at(j);

                if (ImGui::Checkbox(EDAvec[j].string().c_str(), &c)) {
                    checkboxesEDA.at(j) = c;
                }

            }

            if (ImGui::Button("Select All")) {
                for (int j = 0; j < EDAvec.size(); j++) {
                    checkboxesEDA.at(j) = true;
                }
            }

            if (ImGui::Button("Select None")) {
                for (int j = 0; j < EDAvec.size(); j++) {
                    checkboxesEDA.at(j) = false;
                }
            }

            if (ImGui::Button("Ok")) {


                for (int i = 0;i < checkboxesEDA.size();i++) {
                    if (checkboxesEDA.at(i)) {
                        vec_EDA.push_back(EDAvec[i].string());
                    }
                }


                for (int j = 0; j < vec_EDA.size(); j++) {

                    if (!dcomp.decompress(vec_EDA[j].c_str())) {
                        std::cout << "could not compress" << std::endl;
                    }
                }

                show_compress_window_last = false;
                running = false;

            }
            ImGui::End();


        }



        if (show_compress_window)
        {
            ImGui::Begin("Compress");             

            ImGui::Text("Select Parameters to compress");

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
                get_all(direct_path, ".png", imagenes);
                checkboxes.resize(imagenes.size());
                for (int k = 0; k < imagenes.size(); k++)
                {
                    checkboxes.at(k) = false;
                }
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
                    110 * (i % 10), pos_img_y, 100, 100, NULL);
            }
            pos_img_y = 400;
            for (int j = 0; j < imagenes.size(); j++) {
                bool c = checkboxes.at(j);

                if (ImGui::Checkbox(imagenes[j].string().c_str(), &c)) {
                    checkboxes.at(j) = c;
                }
                
            }
            if (ImGui::Button("Select All")) {
                for (int j = 0; j < imagenes.size(); j++) {
                    checkboxes.at(j) = true;
                }
            }
            if (ImGui::Button("Select None")) {
                for (int j = 0; j < imagenes.size(); j++) {
                    checkboxes.at(j) = false;
                }
            }

            if (ImGui::Button("Ok")) {

                compr.setthreshold(threshold2);

                for (int i = 0;i < checkboxes.size();i++) {
                    if (checkboxes.at(i)) {
                        vec_imgs.push_back(imagenes[i].string());
                    }
                }


                for (int j = 0; j < vec_imgs.size(); j++) {

                    if (!compr.compress(vec_imgs[j].c_str())) {
                        std::cout << "could not compress" << std::endl;
                    }
                }


                show_compress_window_last = false;
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