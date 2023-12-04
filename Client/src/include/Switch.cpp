#include "Switch.h"

Switch::Switch()
{
    // Set Everyvalue to default state
    this->switchOnTexture = nullptr;
    this->switchOffTexture = nullptr;
    this->switch_position = {0, 0, 0, 0};
    this->current_switch_state = SWITCH_SPRITE_OFF;
}

Switch::~Switch()
{
    // Set Everyvalue to default state
    if (this->switchOnTexture != nullptr)
    {
        this->switchOnTexture->FreeTexture();
    }

    if (this->switchOnTexture != nullptr)
    {
        this->switchOffTexture->FreeTexture();
    }

    this->switchOnTexture = nullptr;
    this->switchOffTexture = nullptr;
    this->switch_position = {0, 0, 0, 0};
    this->current_switch_state = SWITCH_SPRITE_OFF;
}

int Switch::GetWidth()
{
    return this->switch_position.w;
}

int Switch::GetHeight()
{
    return this->switch_position.h;
}

// Free The Resource
void Switch::FreeSwitch()
{
    // Set Everyvalue to default state
    if (this->switchOnTexture != nullptr)
    {
        this->switchOnTexture->FreeTexture();
    }

    if (this->switchOnTexture != nullptr)
    {
        this->switchOffTexture->FreeTexture();
    }

    this->switchOnTexture = nullptr;
    this->switchOffTexture = nullptr;
    this->switch_position = {0, 0, 0, 0};
    this->current_switch_state = SWITCH_SPRITE_OFF;
}

/**
 * @brief Add the SwitchTexture To this Object To use Switch::Render()
 *
 * @param texture switchOnTexture to add in to use
 */
bool Switch::AddSwitchOnTexture(Texture *texture)
{
    if (texture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Switch Texture is Null!" << std::endl;
        return false;
    }

    // Get rid of the old Texture
    if (this->switchOnTexture != nullptr)
    {
        this->switchOnTexture->FreeTexture();
    }
    this->switchOnTexture = nullptr;

    // Add the New One Instead
    this->switchOnTexture = texture;
    this->switch_position.w = texture->GetWidth();
    this->switch_position.h = texture->GetHeight();
    return true;
}

bool Switch::AddSwitchOffTexture(Texture *texture)
{
    if (texture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Switch Texture is Null!" << std::endl;
        throw Exception("Switch Texture is Null!");
    }

    // Get rid of the old Texture
    if (this->switchOffTexture != nullptr)
    {
        this->switchOffTexture->FreeTexture();
    }
    this->switchOffTexture = nullptr;

    // Add the New One Instead
    this->switchOffTexture = texture;
    this->switch_position.w = texture->GetWidth();
    this->switch_position.h = texture->GetHeight();
    return true;
}

bool Switch::AddBothSwitch(Texture *switchOnTexture, Texture *switchOffTexture)
{
    bool status = true;
    if (!this->AddSwitchOnTexture(switchOnTexture))
    {
        status = false;
    }

    if (!this->AddSwitchOffTexture(switchOffTexture))
    {
        status = false;
    }

    return status;
}

bool Switch::AddBothSwitch(const std::string &switch_on_file_path, const std::string &switch_off_file_path)
{
    // The Switch On Texture
    Texture *switchOnTexture = new Texture();
    switchOnTexture->LoadFromFile(switch_on_file_path);

    // The Switch Off Texture
    Texture *switchOffTexture = new Texture();
    switchOffTexture->LoadFromFile(switch_off_file_path);

    // Add Both Texture to this object
    if (!this->AddBothSwitch(switchOnTexture, switchOffTexture))
    {
        return false;
    }

    return true;
}

void Switch::SetPosition(int x, int y)
{
    if (x < 0 || y < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "X and Y Position cannot be Negative!" << std::endl;
        throw Exception("X and Y Position cannot be Negative!");
    }

    this->switch_position.x = x;
    this->switch_position.y = y;
}

// Handle mouse event
SwitchSpriteState Switch::HandleEvent(SDL_Event *event)
{
    // If Mouse Event Happen
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        bool isInside = isMouseCursorInside(this->switch_position);

        // Mouse is outside the button
        if (!isInside)
        {
            // Do Nothing
            return this->current_switch_state;
        }

        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (this->current_switch_state == SWITCH_SPRITE_OFF)
            {
                this->current_switch_state = SWITCH_SPRITE_ON;
            }
            else if (this->current_switch_state == SWITCH_SPRITE_ON)
            {
                this->current_switch_state = SWITCH_SPRITE_OFF;
            }
            break;
        default:
            break;
        }
    }
    return this->current_switch_state;
}

void Switch::Render()
{
    // Render to the Screen the Switch
    if (this->current_switch_state == SWITCH_SPRITE_ON)
    {
        this->switchOnTexture->Render(this->switch_position.x, this->switch_position.y);
    }
    else
    {
        this->switchOffTexture->Render(this->switch_position.x, this->switch_position.y);
    }
}

void Switch::Render(int x, int y)
{
    // Render to the Screen the Switch
    if (this->current_switch_state == SWITCH_SPRITE_ON)
    {
        this->switchOnTexture->Render(x, y);
    }
    else
    {
        this->switchOffTexture->Render(x, y);
    }
}