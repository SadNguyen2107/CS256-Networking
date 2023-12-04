#ifndef SWITCH_H
#define SWITCH_H

#include "Texture.h"

enum SwitchSpriteState
{
    SWITCH_SPRITE_OFF = 0,
    SWITCH_SPRITE_ON = 1,
    SWITCH_SPRITE_TOTAL = 2
};

class Switch
{
private:
    // Switch Texture
    Texture *switchOnTexture;
    Texture *switchOffTexture;

    // Switch Dimesions
    SDL_Rect switch_position;

    // Show Switch Sprite State
    SwitchSpriteState current_switch_state;

public:
    Switch();
    ~Switch();

    // Properties
    int GetWidth();
    int GetHeight();

    // Free the Resource
    void FreeSwitch();

    // Add the Texture to the Switch On
    bool AddSwitchOnTexture(Texture *texture);

    // Add the Texture to the Switch Off
    bool AddSwitchOffTexture(Texture *texture);

    // Add Both Switch On Texture and Switch Off Texture
    bool AddBothSwitch(Texture *switchOnTexture, Texture *switchOffTexture);

    // Add Both Switch On And On From IMGFile
    bool AddBothSwitch(const std::string &switch_on_file_path, const std::string &switch_off_file_path);

    // Set Position for the Switch
    void SetPosition(int x, int y);

    // Handle mouse event
    SwitchSpriteState HandleEvent(SDL_Event *event);

    // Show Button Sprite
    void Render();
    void Render(int x, int y);
};

#endif