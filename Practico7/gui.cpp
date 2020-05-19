#include "gui.h"



Gui::Gui() {

    inicializa_allegro();
    init_ImGui();
    threshold2 = 0.0f;
    counter = 0;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    show_demo_window = true;
    show_main_window = true;
    running = true;
    show_compress_window = false;
    show_decompress_window = false;

}
Gui::~Gui() {

    // Cleanup
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(queue);
    al_destroy_display(display);

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
            if (ImGui::Button("Ok"))
                printf("conexion al backend cargar fotos para su seleccion");
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

            ImGui::SliderFloat("threshold", &threshold2, 0.0f, 255.0f);

            ImGui::InputText("Select the directory path", direct_path, IM_ARRAYSIZE(direct_path));
            if (ImGui::Button("Ok"))
                printf("conexion al backend cargar fotos para su seleccion");
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

        // Rendering
        ImGui::Render();
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        al_flip_display();
    }
}