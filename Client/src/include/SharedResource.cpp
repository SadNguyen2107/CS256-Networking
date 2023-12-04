#include "SharedResource.h"

// Globally Used Window
SDL_Window *gWindow = nullptr;

// Globally Used Window Renderer
SDL_Renderer *gWindowRenderer = nullptr;

// Function To Check Whether The Mouse is in the Specific Area
bool isMouseCursorInside(SDL_Rect spriteArea)
{
    // Check if mouse in the TextBox Area
    bool isInside = true;

    // Get Mouse Position
    int x, y;
    SDL_GetMouseState(&x, &y);

    // If Mouse is left of the button
    if (x < spriteArea.x)
    {
        isInside = false;
    }
    // If Mouse Right of the button
    else if (x > spriteArea.x + spriteArea.w)
    {
        isInside = false;
    }
    // Mouse is above of the button
    else if (y < spriteArea.y)
    {
        isInside = false;
    }
    // Mouse is down of the button
    else if (y > spriteArea.y + spriteArea.h)
    {
        isInside = false;
    }

    return isInside;
}