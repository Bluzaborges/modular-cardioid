#include "Pattern.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace {

struct Point {
    float x;
    float y;
};

}

Pattern::Pattern()
    : pointCount(defaultPoints),
      multiplicationFactor(defaultMultiplier) {
}

void Pattern::render(
    SDL_Renderer* renderer,
    float availableWidth,
    float availableHeight) const {
    const float radius = std::max(0.0f, std::min(availableWidth, availableHeight) * 0.34f);
    const float centerX = availableWidth * 0.5f;
    const float centerY = availableHeight * 0.5f;

    std::vector<Point> circlePoints;
    circlePoints.reserve(static_cast<std::size_t>(pointCount));

    for (int index = 0; index < pointCount; ++index) {
        const float angle = -static_cast<float>(M_PI) * 0.5f +
            2.0f * static_cast<float>(M_PI) * static_cast<float>(index) /
            static_cast<float>(pointCount);

        circlePoints.push_back({
            centerX + radius * std::cos(angle),
            centerY + radius * std::sin(angle)
        });
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 110);
    for (int index = 0; index < pointCount; ++index) {
        const int target = (index * multiplicationFactor) % pointCount;
        const Point& start = circlePoints[static_cast<std::size_t>(index)];
        const Point& end = circlePoints[static_cast<std::size_t>(target)];

        SDL_RenderDrawLineF(renderer, start.x, start.y, end.x, end.y);
    }

    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 150);
    for (int index = 0; index < pointCount; ++index) {
        const Point& start = circlePoints[static_cast<std::size_t>(index)];
        const Point& end = circlePoints[static_cast<std::size_t>((index + 1) % pointCount)];
        SDL_RenderDrawLineF(renderer, start.x, start.y, end.x, end.y);
    }

    SDL_SetRenderDrawColor(renderer, 204, 204, 204, 255);
    for (const Point& point : circlePoints) {
        const SDL_FRect marker{point.x - 1.5f, point.y - 1.5f, 3.0f, 3.0f};
        SDL_RenderFillRectF(renderer, &marker);
    }
}

void Pattern::reset() {
    pointCount = defaultPoints;
    multiplicationFactor = defaultMultiplier;
}

int& Pattern::points() {
    return pointCount;
}

int& Pattern::multiplier() {
    return multiplicationFactor;
}
