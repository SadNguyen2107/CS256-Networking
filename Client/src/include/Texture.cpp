#include "Texture.h"

/**
 * @brief Construct a new Texture:: Texture object.
 * Set everything to the default State
 */
Texture::Texture()
{
    this->image_texture = nullptr;
    this->image_dimension = {0, 0, 0, 0};
    this->isIMG = false;
    this->isText = false;
}

Texture::~Texture()
{
    // Deallocate Everything
    this->FreeTexture();
}

/**
 * @brief Get the Height of the Image Texture that Load In
 *
 * @return If the return value is 0: no Image File is loaded in yet. If the return value larger more than 1 is Correct.
 */
int Texture::GetHeight()
{
    return this->image_dimension.h;
}

/**
 * @brief Get The Width of the Image Texture that Load In
 *
 * @return If the value is 0: no image file has loaded yet. If the return value is larger than 1 is Correct
 */
int Texture::GetWidth()
{
    return this->image_dimension.w;
}

bool Texture::isIMGTexture()
{
    return this->isIMG;
}

bool Texture::isTextTexture()
{
    return this->isText;
}

void Texture::SetPosition(int x, int y)
{
    if (x < 0 || y < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "X and Y Position cannot be Negative!" << std::endl;
        throw Exception("X and Y Position cannot be Negative!");
    }

    this->image_dimension.x = x;
    this->image_dimension.y = y;
}

bool Texture::LoadFromFile(std::string IMGfile, SDL_Color *colorKey)
{
    // The final Texture
    SDL_Texture *newTexture = nullptr;

    //? Destroy the old Texture
    this->FreeTexture();

    // Load Image at Specificed Path
    SDL_Surface *loadedSurface = IMG_Load(IMGfile.c_str());
    if (loadedSurface == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to load Image " << IMGfile << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Color Key the Image before Creating a Texture
    // To remove the Color Wanted
    if (colorKey != nullptr)
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, colorKey->r, colorKey->g, colorKey->b));
    }

    // Create Texture From Surface Pixels
    newTexture = SDL_CreateTextureFromSurface(gWindowRenderer, loadedSurface);
    if (newTexture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to Create Texture from " << IMGfile << "! SDL Error: " << SDL_GetError() << std::endl;

        // Prevent Memory Leak
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    // Get Image Dimensions
    this->image_dimension.w = loadedSurface->w;
    this->image_dimension.h = loadedSurface->h;
    this->image_texture = newTexture;
    this->isIMG = true;

    // Prevent Memory Leak
    SDL_FreeSurface(loadedSurface);
    return this->isIMG;
}

bool Texture::LoadFromRenderedText(std::string text, TTF_Font *textFont, SDL_Color textcolor)
{
    // Get rid of the Pre-existing Texture
    this->FreeTexture();

    // Render Text Surface
    if (textFont == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "No Text Font Input!" << std::endl;
        throw Exception("No Text Font Input!");
    }

    // Render Text Surface
    // Must have TextFont
    SDL_Surface *textSurface = TTF_RenderText_Blended(textFont, text.c_str(), textcolor);
    if (textSurface == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to Render Text Surface! SDL_tff Error: " << TTF_GetError() << std::endl;
        throw Exception("Unable to Render Text Surface! SDL_tff Error: ");
    }

    // Create Texture From Surface Pixels
    this->image_texture = SDL_CreateTextureFromSurface(gWindowRenderer, textSurface);
    if (this->image_texture == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Unable to Create Texture from Rendered Text! SDL Error: " << SDL_GetError() << std::endl;

        // Deallocate the Resource
        SDL_FreeSurface(textSurface);
        throw Exception("Unable to Create Texture from Rendered Text! SDL Error: ");
    }

    // Get Text Dimensions
    this->image_dimension.h = textSurface->h;
    this->image_dimension.w = textSurface->w;
    this->isText = true;

    // Get rid of the old Surface
    SDL_FreeSurface(textSurface);
    return this->isText;
}

void Texture::Render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {
        x,
        y,
        this->image_dimension.w,
        this->image_dimension.h};

    if (clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(gWindowRenderer, this->image_texture, clip, &renderQuad, angle, center, flip);
}

void Texture::Render()
{
    SDL_RenderCopy(gWindowRenderer, image_texture, NULL, &image_dimension);
}

void Texture::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    // Modulate Texture
    SDL_SetTextureColorMod(this->image_texture, red, green, blue);
}

void Texture::SetBlendMode(SDL_BlendMode blending)
{
    // Set Blending function
    SDL_SetTextureBlendMode(this->image_texture, blending);
}

void Texture::SetAlpha(Uint8 alpha)
{
    // Modulate Texture Alpha
    SDL_SetTextureAlphaMod(this->image_texture, alpha);
}

void Texture::FreeTexture()
{
    //? Deallocate the Hardware Texture
    if (this->image_texture != nullptr)
    {
        SDL_DestroyTexture(this->image_texture);
    }

    // Set to the Default State
    this->image_texture = nullptr;
    this->image_dimension = {0, 0, 0, 0};
    this->isIMG = false;
    this->isText = false;
}
