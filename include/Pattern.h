#ifndef PATTERN_H
#define PATTERN_H

#include <SDL2/SDL.h>

class Pattern {
public:
    Pattern();

    void render(SDL_Renderer* renderer, float availableWidth, float availableHeight) const;
    void reset();

    int& points();
    int& multiplier();

private:
    static constexpr int defaultPoints = 200;
    static constexpr int defaultMultiplier = 2;

    int pointCount;
    int multiplicationFactor;
};

#endif
