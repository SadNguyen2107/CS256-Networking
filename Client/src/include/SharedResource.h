#pragma once

#include <boost/log/trivial.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <iomanip>
#include <sstream>
#include <string>
#include <regex>
#include <tuple>
#include <fstream>

//? For Logging Purpose
#define BOOST_LOG_DYN_LINK 1

// SCREEN DIMESIONS TO DISPLAY
#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1420

// Globally Used Window
extern SDL_Window *gWindow;

// Globally Used Window Renderer
extern SDL_Renderer *gWindowRenderer;


// Function To Check Whether The Mouse is in the Specific Area
extern bool isMouseCursorInside(SDL_Rect spriteArea);