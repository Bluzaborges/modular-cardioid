#ifndef APP_H
#define APP_H

#include "Pattern.h"

#include <SDL2/SDL.h>

class App {
public:
    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

private:
    static constexpr int initialWidth = 1200;
    static constexpr int initialHeight = 760;

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Pattern pattern;

    void processEvents();
    void renderFrame();
    void renderControlPanel();
    void configureStyle();
};

#endif
