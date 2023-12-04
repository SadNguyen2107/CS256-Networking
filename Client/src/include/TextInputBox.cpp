#include "TextInputBox.h"

TextInputBox::TextInputBox()
{
    this->inputTextBox_Texture = nullptr;
    this->inputText = "Input Here...";
    this->inputTextBox_Position = {0, 0, 0, 0};
    this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_MOUSE_OUT;
    this->cursorVisible = true;
    this->lastCursorToggleTime = 0;

    // Set The Default of the cursorIndex
    this->cursorIndex_Str = 0;
}

TextInputBox::~TextInputBox()
{
    // Free the Texture
    if (this->inputTextBox_Texture != nullptr)
    {
        this->inputTextBox_Texture->FreeTexture();
    }

    this->inputTextBox_Texture = nullptr;
    this->inputText = "Input Here...";
    this->inputTextBox_Position = {0, 0, 0, 0};
    this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_MOUSE_OUT;

    // Set The Default of the cursorIndex
    this->cursorIndex_Str = 0;
}

// Get The Result
std::string TextInputBox::GetResult() const
{
    return this->inputText;
}

// Free the Resource
void TextInputBox::FreeTextBox()
{
    // Free the Texture
    if (this->inputTextBox_Texture != nullptr)
    {
        this->inputTextBox_Texture->FreeTexture();
    }

    this->inputTextBox_Texture = nullptr;
    this->inputText = "";
    this->inputTextBox_Position = {0, 0, 0, 0};
    this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_MOUSE_OUT;
}

// Add the Texture into this object
void TextInputBox::AddTexture(Texture *textBoxTexture)
{
    if (textBoxTexture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Text Texture Cannot Be Null!" << std::endl;
        throw Exception("Text Texture Cannot Be Null!");
    }

    if (!textBoxTexture->isTextTexture())
    {
        BOOST_LOG_TRIVIAL(error) << "Text Texture Is not Text!" << std::endl;
        throw Exception("Text Texture Is not Text!");
    }

    this->inputTextBox_Texture = textBoxTexture;
    this->inputTextBox_Position.h = textBoxTexture->GetHeight();
    this->inputTextBox_Position.w = textBoxTexture->GetWidth();
}

// Add Texture to this object then it will create a Texture
// After Adding TextFont and TextColor
void TextInputBox::AddTexture(const std::string &text)
{
    // Get Rid of the old Texture if needed
    if (this->inputTextBox_Texture != nullptr)
    {
        this->inputTextBox_Texture->FreeTexture();
        this->inputTextBox_Texture = nullptr;
    }

    // The Final Texture
    Texture *finalTexture = new Texture();

    // Create Texture From Text
    if (!finalTexture->LoadFromRenderedText(text, this->textFont, textColor))
    {
        BOOST_LOG_TRIVIAL(error) << "Text Texture Cannot Be Created!" << std::endl;
        throw Exception("Text Texture Cannot Be Null!");
    }

    this->inputTextBox_Texture = finalTexture;
    this->inputText = text;
    this->inputTextBox_Position.h = finalTexture->GetHeight();
    this->inputTextBox_Position.w = finalTexture->GetWidth();

    // Set The Default of the CursorIndex_Str
    this->cursorIndex_Str = text.length() - 1;
}

// Add the Font into this object
void TextInputBox::AddFont(TTF_Font *textFont)
{
    if (textFont == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Text Font Cannot Be Null!" << std::endl;
        throw Exception("Text Font Cannot Be Null!");
    }

    this->textFont = textFont;

    // Variables to store the Char Width and Char Height
    int charWidth, charHeight;

    // Get The size of the Char
    const char *exampleChar = "A";
    TTF_SizeText(this->textFont, exampleChar, &charWidth, &charHeight);
    this->characterSize.w = charWidth;
    this->characterSize.h = charHeight;
}

// Add the Text Color
void TextInputBox::AddTextColor(SDL_Color textColor)
{
    this->textColor = textColor;
    if (this->inputTextBox_Texture != nullptr)
    {
        this->inputTextBox_Texture->SetColor(this->textColor.r, this->textColor.g, this->textColor.b);
    }
}

// Event Handler
TextInputBoxState TextInputBox::HandleEvent(SDL_Event *event)
{
    // If The User Click Mouse But Not this Sprite
    // Stop Input
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (!isMouseCursorInside(this->inputTextBox_Position))
        {
            this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_MOUSE_OUT;
        }
    }

    // Mouse Must Click on the TextBox
    // THen Can Be input
    if (this->current_text_box_state == TEXT_INPUT_BOX_SPRITE_MOUSE_OUT)
    {
        if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN)
        {
            // Check if mouse in the TextBox Area
            bool isInside = isMouseCursorInside(this->inputTextBox_Position);

            // Mouse is outside the button
            if (!isInside)
            {
                // Do Nothing
                this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_MOUSE_OUT;
                return this->current_text_box_state;
            }

            // Check If Click or Not
            if (event->type == SDL_MOUSEBUTTONDOWN)
            {
                this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_CLICK;
            }
        }
    }

    // If the User Click
    if (this->current_text_box_state == TEXT_INPUT_BOX_SPRITE_CLICK || this->current_text_box_state == TEXT_INPUT_BOX_SPRITE_INPUT)
    {
        // If in input mode, toggle cursor visibility every 500 milliseconds
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - this->lastCursorToggleTime >= 500)
        {
            this->cursorVisible = !this->cursorVisible;
            this->lastCursorToggleTime = currentTime;
        }

        // Start Input Phase
        // Enable Text Input
        SDL_StartTextInput();

        if (event->type == SDL_KEYDOWN)
        {
            // Handle backspace
            if (event->key.keysym.sym == SDLK_BACKSPACE && this->inputText.length() > 0)
            {
                // Erase that Character From that Cursor Index
                this->inputText.erase(this->cursorIndex_Str, 1);

                // Decrease The Cursor X Position when Arrow Left
                if (this->cursorIndex_Str >= 0)
                {
                    this->cursorIndex_Str--;
                }

                this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_INPUT;
            }
            // Handle Copy
            else if (event->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
            {
                SDL_SetClipboardText(this->inputText.c_str());
            }
            // Handle Paste
            else if (event->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
            {
                this->inputText = SDL_GetClipboardText();

                // Update The Cursor Position
                this->cursorIndex_Str = this->inputText.size();
                this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_INPUT;
            }
            // Handle Arrow Left
            else if (event->key.keysym.sym == SDLK_LEFT)
            {
                // Decrease The Cursor X Position when Arrow Left
                if (this->cursorIndex_Str > 0)
                {
                    this->cursorIndex_Str--;
                }
            }
            // Handle Arrow Right
            else if (event->key.keysym.sym == SDLK_RIGHT)
            {
                // Increase The Cursor X Position when Arrow Right
                if (this->cursorIndex_Str < this->inputText.length())
                {
                    this->cursorIndex_Str++;
                }
            }
            // Handle Enter
            else if (event->key.keysym.sym == SDLK_KP_ENTER || event->key.keysym.sym == SDLK_RETURN)
            {
                this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_ENTER;
                return this->current_text_box_state;
            }
        }
        // Special text input event
        else if (event->type == SDL_TEXTINPUT)
        {
            // Not copy or pasting
            if (!(SDL_GetModState() & KMOD_CTRL && (event->text.text[0] == 'c' || event->text.text[0] == 'C' || event->text.text[0] == 'v' || event->text.text[0] == 'V')))
            {
                // Insert At that Position
                this->inputText.insert(this->cursorIndex_Str + 1, event->text.text);

                // Update The Cursor Index (string Based)
                this->cursorIndex_Str++;

                this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_INPUT;
            }
        }

        if (this->current_text_box_state == TEXT_INPUT_BOX_SPRITE_INPUT)
        {
            // Text is not empty
            if (!this->inputText.empty())
            {
                this->inputTextBox_Texture->LoadFromRenderedText(this->inputText, this->textFont, this->textColor);
            }
            // If text is empty
            else
            {
                this->inputTextBox_Texture->LoadFromRenderedText(" ", this->textFont, this->textColor);
            }
            // Resize the Texture
            this->inputTextBox_Position.w = this->inputTextBox_Texture->GetWidth();
            this->inputTextBox_Position.h = this->inputTextBox_Texture->GetHeight();
        }
    }

    // If the user press enter
    if (this->current_text_box_state == TEXT_INPUT_BOX_SPRITE_ENTER)
    {
        BOOST_LOG_TRIVIAL(info) << "Finished Input!" << std::endl;
        BOOST_LOG_TRIVIAL(info) << "Text: " << this->inputText << std::endl;
        this->current_text_box_state = TEXT_INPUT_BOX_SPRITE_MOUSE_OUT;
        SDL_StopTextInput();
    }

    return this->current_text_box_state;
}

// Set Position for the Button
void TextInputBox::SetPosition(int x, int y)
{
    if (x < 0 || y < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "X or Y Position cannot below 0!" << std::endl;
        throw Exception("X or Y Position cannot below 0!");
    }

    this->inputTextBox_Position.x = x;
    this->inputTextBox_Position.y = y;
}

// Show the TextBox
void TextInputBox::Render()
{
    RenderSurroundingBorder();
    this->inputTextBox_Texture->Render(this->inputTextBox_Position.x, this->inputTextBox_Position.y);

    // Render the cursor if it's visible
    if (this->current_text_box_state >= TEXT_INPUT_BOX_SPRITE_CLICK && this->cursorVisible)
    {
        int maxCursorX = this->inputTextBox_Position.x + this->inputTextBox_Texture->GetWidth();
        int posX = this->cursorIndex_Str + 1;

        int cursorX = this->inputTextBox_Position.x + posX * this->characterSize.w;
        if (cursorX > maxCursorX)
        {
            cursorX = maxCursorX;
        }

        int cursorY = this->inputTextBox_Position.y;
        SDL_SetRenderDrawColor(gWindowRenderer, this->textColor.r, this->textColor.g, this->textColor.b, this->textColor.a);
        SDL_Rect cursorRect = {cursorX, cursorY, 2, this->inputTextBox_Texture->GetHeight()};
        SDL_RenderFillRect(gWindowRenderer, &cursorRect);
    }
}

void TextInputBox::Render(int x, int y)
{
    RenderSurroundingBorder();
    this->inputTextBox_Texture->Render(x, y);

    // Render the cursor if it's visible
    if (this->current_text_box_state >= TEXT_INPUT_BOX_SPRITE_CLICK && this->cursorVisible)
    {
        int maxCursorX = this->inputTextBox_Position.x + this->inputTextBox_Texture->GetWidth();
        int posX = this->cursorIndex_Str + 1;

        int cursorX = this->inputTextBox_Position.x + posX * this->characterSize.w;
        if (cursorX > maxCursorX)
        {
            cursorX = maxCursorX;
        }

        int cursorY = this->inputTextBox_Position.y;
        SDL_SetRenderDrawColor(gWindowRenderer, this->textColor.r, this->textColor.g, this->textColor.b, this->textColor.a);
        SDL_Rect cursorRect = {cursorX, cursorY, 2, this->inputTextBox_Texture->GetHeight()};
        SDL_RenderFillRect(gWindowRenderer, &cursorRect);
    }
}

void TextInputBox::RenderSurroundingBorder()
{
    SDL_SetRenderDrawColor(gWindowRenderer, this->textColor.r, this->textColor.g, this->textColor.b, this->textColor.a);
    SDL_Rect borderRect = {
        this->inputTextBox_Position.x - 2,            // Adjust the border width as needed
        this->inputTextBox_Position.y - 2,            // Adjust the border width as needed
        this->inputTextBox_Texture->GetWidth() + 4,   // Adjust the border width as needed
        this->inputTextBox_Texture->GetHeight() + 4}; // Adjust the border width as needed

    SDL_RenderDrawRect(gWindowRenderer, &borderRect);
}