#include "Button.h"

Button::Button()
{
    // Set Everything to Default State
    for (size_t index = 0; index < BUTTON_SPRITE_TOTAL; index++)
    {
        this->buttonTextures[index] = nullptr;
    }
    this->button_position = {0, 0, 0, 0};
    this->current_button_state = BUTTON_SPRITE_MOUSE_OUT;
}

Button::~Button()
{
    // Free Texture and set everything to the default state
    for (size_t index = 0; index < BUTTON_SPRITE_TOTAL; index++)
    {
        if (this->buttonTextures[index] != nullptr)
        {
            this->buttonTextures[index]->FreeTexture();
            this->buttonTextures[index] = nullptr;
        }
    }
    this->button_position = {0, 0, 0, 0};
    this->current_button_state = BUTTON_SPRITE_MOUSE_OUT;
}

int Button::GetWidth()
{
    return this->button_position.w;
}

int Button::GetHeight()
{
    return this->button_position.h;
}

// Free the Resource
void Button::FreeButton()
{
    // Free Texture and set everything to the default state
    for (size_t index = 0; index < BUTTON_SPRITE_TOTAL; index++)
    {
        if (this->buttonTextures[index] != nullptr)
        {
            this->buttonTextures[index]->FreeTexture();
            this->buttonTextures[index] = nullptr;
        }
    }
    this->button_position = {0, 0, 0, 0};
    this->current_button_state = BUTTON_SPRITE_MOUSE_OUT;
}

// Add the Texture to the Button Mouse OUT
bool Button::AddButtonTexture_MouseOut(Texture *texture)
{
    if (texture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Texture Cannot Be NULL!" << std::endl;
        throw Exception("Texture Cannot Be NULL!");
    }
    // Get rid of the old buttonTexture
    if (this->buttonTextures[BUTTON_SPRITE_MOUSE_OUT] != nullptr)
    {
        this->buttonTextures[BUTTON_SPRITE_MOUSE_OUT]->FreeTexture();
    }

    this->buttonTextures[BUTTON_SPRITE_MOUSE_OUT] = texture;
    // Set Button Dimension
    this->button_position.h = texture->GetHeight();
    this->button_position.w = texture->GetWidth();
    return true;
}

// Add the Texture to the Button Mouse Over Motion
bool Button::AddButtonTexture_MouseOverMotion(Texture *texture)
{
    if (texture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Texture Cannot Be NULL!" << std::endl;
        throw Exception("Texture Cannot Be NULL!");
    }
    // Get rid of the old buttonTexture
    if (this->buttonTextures[BUTTON_SPRITE_MOUSE_OVER_MOTION] != nullptr)
    {
        this->buttonTextures[BUTTON_SPRITE_MOUSE_OVER_MOTION]->FreeTexture();
    }

    this->buttonTextures[BUTTON_SPRITE_MOUSE_OVER_MOTION] = texture;
    return true;
}

// Add the Texture to the Button Mouse Down
bool Button::AddButtonTexture_MouseDown(Texture *texture)
{
    if (texture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Texture Cannot Be NULL!" << std::endl;
        throw Exception("Texture Cannot Be NULL!");
    }
    // Get rid of the old button Texture
    if (this->buttonTextures[BUTTON_SPRITE_MOUSE_DOWN] != nullptr)
    {
        this->buttonTextures[BUTTON_SPRITE_MOUSE_DOWN]->FreeTexture();
    }

    this->buttonTextures[BUTTON_SPRITE_MOUSE_DOWN] = texture;
    return true;
}

// Add all the texture in one line of code
bool Button::AddButtonTexture_All(Texture *buttonTexture_MouseOut, Texture *buttonTexture_MouseOverMotion, Texture *buttonTexture_MouseDown)
{
    if (buttonTexture_MouseOut == nullptr || buttonTexture_MouseOverMotion == nullptr || buttonTexture_MouseDown == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Texture Cannot Be NULL!" << std::endl;
        throw Exception("Texture Cannot Be NULL!");
    }

    // Add all the Components
    this->AddButtonTexture_MouseOut(buttonTexture_MouseOut);
    this->AddButtonTexture_MouseOverMotion(buttonTexture_MouseOverMotion);
    this->AddButtonTexture_MouseDown(buttonTexture_MouseDown);

    return true;
}

// Add all the IMGFile in one line of code
void Button::AddButtonAnimations(const std::string &buttonMouseOut_filePath, const std::string &buttonMouseOverMotion_filePath, const std::string &buttonMouseDown_filePath)
{
    // All The Button Texture
    Texture *buttonMouseOut_Texture = new Texture();
    Texture *buttonMouseOverMotion_Texture = new Texture();
    Texture *buttonMouseDown_Texture = new Texture();

    // Load the Required File
    buttonMouseOut_Texture->LoadFromFile(buttonMouseOut_filePath);
    buttonMouseOverMotion_Texture->LoadFromFile(buttonMouseOverMotion_filePath);
    buttonMouseDown_Texture->LoadFromFile(buttonMouseDown_filePath);

    // Add into this object
    this->AddButtonTexture_MouseOut(buttonMouseOut_Texture);
    this->AddButtonTexture_MouseOverMotion(buttonMouseOverMotion_Texture);
    this->AddButtonTexture_MouseDown(buttonMouseDown_Texture);
}

void Button::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    // Change All The Color Textures
    for (size_t index = 0; index < BUTTON_SPRITE_TOTAL; index++)
    {
        this->buttonTextures[index]->SetColor(red, green, blue);
    }
}

// Set Position for the Button
void Button::SetPosition(int x, int y)
{
    if (x < 0 || y < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "X or Y cannot below Zero!" << std::endl;
        throw Exception("X or Y cannot below Zero!");
    }

    this->button_position.x = x;
    this->button_position.y = y;
}

// Handle mouse event
ButtonSpriteState Button::HandleEvent(SDL_Event *event)
{
    // If mouse event happen
    if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN)
    {
        bool isInside = isMouseCursorInside(this->button_position);

        // Mouse is outside the button
        if (!isInside)
        {
            // Do Nothing
            this->current_button_state = BUTTON_SPRITE_MOUSE_OUT;
            return this->current_button_state;
        }

        switch (event->type)
        {
        case SDL_MOUSEMOTION:
            this->current_button_state = BUTTON_SPRITE_MOUSE_OVER_MOTION;
            break;
        case SDL_MOUSEBUTTONDOWN:
            this->current_button_state = BUTTON_SPRITE_MOUSE_DOWN;
            break;
        default:
            break;
        }
    }
    return this->current_button_state;
}

// Show Button Sprite
void Button::Render()
{
    this->buttonTextures[this->current_button_state]->Render(this->button_position.x, this->button_position.y);
}

void Button::Render(int x, int y)
{
    this->buttonTextures[this->current_button_state]->Render(x, y);
}