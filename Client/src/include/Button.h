#ifndef BUTTON_H
#define BUTTON_H

#include "Texture.h"

enum ButtonSpriteState
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_TOTAL = 3
};

class Button
{
private:
    // Button Texture
    Texture* buttonTextures[BUTTON_SPRITE_TOTAL];

    // Button Dimesions
    SDL_Rect button_position;

    // Show Button Sprite State
    ButtonSpriteState current_button_state;

public:
    Button();
    ~Button();

    // Properties
    int GetWidth();
    int GetHeight();

    // Free The Resource
    void FreeButton();

    // Add the Texture to the Button Mouse OUT
    bool AddButtonTexture_MouseOut(Texture* texture);
    
    // Add the Texture to the Button Mouse Over Motion
    bool AddButtonTexture_MouseOverMotion(Texture* texture);

    // Add the Texture to the Button Mouse Down
    bool AddButtonTexture_MouseDown(Texture* texture);

    // Add all the Texture in one line of code
    bool AddButtonTexture_All(Texture* buttonTexture_MouseOut, Texture* buttonTexture_MouseOverMotion, Texture* buttonTexture_MouseDown);

    // Add all the IMGFile in one line of code
    void AddButtonAnimations(const std::string& buttonMouseOut_filePath, const std::string& buttonMouseOverMotion_filePath, const std::string& buttonMouseDown_filePath);

    // Change Color Modulation
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set Position for the Button
    void SetPosition(int x, int y);

    // Handle mouse event
    ButtonSpriteState HandleEvent(SDL_Event *event);

    // Show Button Sprite
    void Render();
    void Render(int x, int y);
};


#endif