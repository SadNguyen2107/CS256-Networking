#ifndef TEXT_INPUT_BOX_H
#define TEXT_INPUT_BOX_H

#include "Texture.h"

enum TextInputBoxState
{
    TEXT_INPUT_BOX_SPRITE_MOUSE_OUT = 0,
    TEXT_INPUT_BOX_SPRITE_CLICK = 1,
    TEXT_INPUT_BOX_SPRITE_INPUT = 2,
    TEXT_INPUT_BOX_SPRITE_ENTER = 3,
    TEXT_INPUT_BOX_SPRITE_TOTAL = 4
};

class TextInputBox
{
private:
    // The Texture will be Render to the Screen
    Texture *inputTextBox_Texture;

    // Each Character Size
    SDL_Rect characterSize;

    // The String Will hold the Result
    std::string inputText;
    TTF_Font *textFont;
    SDL_Color textColor;

    // To Create A Blinking Cursor Effect
    int cursorIndex_Str; // To Store the Position of the Char in A Text based on the Cursor At (String Based)
    int cursorIndex_IMG;             // To Store the Position of the Char in A Text based on the Cursor At (Render Based)
    bool cursorVisible;
    int lastCursorToggleTime;
    
    // The Position of the TextBox
    SDL_Rect inputTextBox_Position;

    // The State Of The Input Text Box
    TextInputBoxState current_text_box_state;

public:
    TextInputBox();
    ~TextInputBox();

    // Get The Result
    std::string GetResult() const;

    // Free Texture
    void FreeTextBox();

    // Add the Texture into this object
    void AddTexture(Texture *textBoxTexture);

    // Use This Instead of AddTexture(Texture *textBoxTexture)
    void AddTexture(const std::string &text);

    // Add the Font into this object
    void AddFont(TTF_Font *textFont);

    // Add the Text Color
    void AddTextColor(SDL_Color textColor = {0, 0, 0, 255});

    // Event Handler
    TextInputBoxState HandleEvent(SDL_Event *event);

    // Set Position for the Button
    void SetPosition(int x, int y);

    // Show the TextBox
    void Render();
    void Render(int x, int y);

    // Making A box around text with Sharp corners
    void RenderSurroundingBorder();
};

#endif