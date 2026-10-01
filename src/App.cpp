#include "App.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <stdexcept>
#include <string>

App::App()
    : window(nullptr),
      renderer(nullptr),
      running(true) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("Could not initialize SDL: ") + SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    window = SDL_CreateWindow(
        "Modular Cardioid",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        initialWidth,
        initialHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (window == nullptr) {
        SDL_Quit();
        throw std::runtime_error(std::string("Could not create the window: ") + SDL_GetError());
    }

    SDL_SetWindowMinimumSize(window, 720, 480);

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == nullptr) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }

    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error(std::string("Could not create the renderer: ") + SDL_GetError());
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    configureStyle();

    if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer)) {
        ImGui::DestroyContext();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Could not initialize the Dear ImGui SDL2 backend.");
    }

    if (!ImGui_ImplSDLRenderer2_Init(renderer)) {
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Could not initialize the Dear ImGui renderer backend.");
    }
}

App::~App() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void App::run() {
    while (running) {
        processEvents();
        renderFrame();
    }
}

void App::processEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(window)) {
            running = false;
        }
    }
}

void App::renderFrame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    renderControlPanel();

    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    SDL_SetRenderDrawColor(renderer, 12, 12, 12, 255);
    SDL_RenderClear(renderer);

    pattern.render(renderer, static_cast<float>(windowWidth), static_cast<float>(windowHeight));

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
}

void App::renderControlPanel() {
    ImGui::SetNextWindowPos(ImVec2(16.0f, 16.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300.0f, 190.0f), ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("CONTROLS", nullptr, flags);

    ImGui::TextUnformatted("(multiplier x i) mod points");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 4.0f));

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("POINTS");
    ImGui::SameLine(116.0f);
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::SliderInt(
        "##points",
        &pattern.points(),
        10,
        1000,
        "%d",
        ImGuiSliderFlags_AlwaysClamp
    );

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("MULTIPLIER");
    ImGui::SameLine(116.0f);
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::SliderInt(
        "##multiplier",
        &pattern.multiplier(),
        2,
        100,
        "%d",
        ImGuiSliderFlags_AlwaysClamp
    );

    ImGui::Dummy(ImVec2(0.0f, 4.0f));
    if (ImGui::Button("RESET", ImVec2(-1.0f, 30.0f))) {
        pattern.reset();
    }

    ImGui::End();
}

void App::configureStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(12.0f, 10.0f);
    style.FramePadding = ImVec2(8.0f, 5.0f);
    style.ItemSpacing = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.GrabMinSize = 12.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;

    ImVec4* colors = style.Colors;
    const ImVec4 background(0.047f, 0.047f, 0.047f, 1.0f);
    const ImVec4 foreground(0.80f, 0.80f, 0.80f, 1.0f);
    const ImVec4 dimmed(0.36f, 0.36f, 0.36f, 1.0f);

    colors[ImGuiCol_WindowBg] = background;
    colors[ImGuiCol_TitleBg] = background;
    colors[ImGuiCol_TitleBgActive] = background;
    colors[ImGuiCol_TitleBgCollapsed] = background;
    colors[ImGuiCol_Text] = foreground;
    colors[ImGuiCol_TextDisabled] = dimmed;
    colors[ImGuiCol_Border] = foreground;
    colors[ImGuiCol_FrameBg] = background;
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    colors[ImGuiCol_SliderGrab] = foreground;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    colors[ImGuiCol_Button] = background;
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
    colors[ImGuiCol_Separator] = foreground;
    colors[ImGuiCol_CheckMark] = foreground;
}
