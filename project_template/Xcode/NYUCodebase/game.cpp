//
//  game.cpp
//  NYUCodebase
//
//  Created by Marcus Williams on 2/10/16.
//  Copyright © 2016 SoundVoid. All rights reserved.
//

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "PerlinNoise.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


SDL_Window* displayWindow;

class Entity {
public:
    void Draw();
    float x;
    float y;
    float rotation;
    int textureID;
    float width;
    float height;
    float speed;
    float direction_x;
    float direction_y;
};

void Setup();
void ProcessEvents(SDL_Event event, bool done);
void Update(float lastFrameTicks, float elapsed);
void Cleanup();
void Render();

int main() {
    Setup();
    SDL_Event event;
    bool done = false;
    float lastFrameTicks = 0.0f;
    float elapsed = 0.0f;
    while(!done) {
        ProcessEvents(event, done);
        Update(lastFrameTicks, elapsed);
        Render();
    }
    Cleanup();
}

void Setup() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    // setup OpenGL
    // Set our projection matrix
}

void ProcessEvents(SDL_Event event, bool done) {
    // our SDL event loop
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
    }
    // check input events
    
}

void Update(float lastFrameTicks, float elapsed) {
    // move stuff and check for collisions
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    
}

void Render() {
    // for all game elements
    // setup transforms, render sprites
}

void Cleanup() {
    // cleanup joysticks, textures, etc.
}
