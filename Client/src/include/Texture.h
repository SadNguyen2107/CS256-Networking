#ifndef TEXTURE_H
#define TEXTURE_H
#include "SharedResource.h"
#include "../Base/Exception.h"

class Texture
{
private:
    // The Actual Hardware Texture
    SDL_Texture *image_texture = nullptr;

    // Image Dimension
    SDL_Rect image_dimension;

    // Status to Check Whether is Text or Image
    bool isText;
    bool isIMG;

public:
    Texture();
    ~Texture();

    // Properties
    int GetWidth();
    int GetHeight();
    bool isIMGTexture();
    bool isTextTexture();

    // Set Position for Rendering
    void SetPosition(int x, int y);

    // Loading Methods
    bool LoadFromFile(std::string IMG_file, SDL_Color *colorKey = nullptr);
    bool LoadFromRenderedText(std::string text, TTF_Font* textFont = nullptr, SDL_Color textcolor = {0, 0, 0, 255});

    // Render Method
    void Render(int x, int y, SDL_Rect *clip = nullptr, double angle = 0.0, SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void Render();

    // Set Color Modulation
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set Blending
    void SetBlendMode(SDL_BlendMode blending);

    // Set Alpha Modulation
    void SetAlpha(Uint8 alpha);

    // Free Texture
    void FreeTexture();
};


#endif