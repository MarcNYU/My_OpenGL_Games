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

GLuint LoadTexture(const char *image_path);
void makeTile(float xCord, float yCord, Matrix m, ShaderProgram sp);


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 1280, 720);
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    float lastFrameTicks = 0.0f;
    float elapsed = 0.0f;
    
    //Block
    float PositionY = -1.0f;
    float PositionX = -2.5f;
    
    float timer = 0.2f;
//    float num = -3.0f;
//    std::vector<float> points;
//    for (int i = 0; i < 12; i++) {
//        points[i] = num;
//        num += 0.5f;
//    }
    
//    int cord1 = rand() % 3 - 2;
//    int cord2 = rand() % 3;
//    int cord3 = rand() % 3 - 3;
//    int cord1 = points[rand()%12];
//    int cord2 = points[rand()%12];
//    int cord3 = points[rand()%12];
    
//    unsigned char levelData[1080][720];
    
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        //Tile
        timer -= elapsed;
//        if (timer <= 0){
//            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//            glClear(GL_COLOR_BUFFER_BIT);
//            timer = 0.2f;
//        }
        
//        int cord1 = rand() % 3 - 2;
//        int cord2 = rand() % 3;
//        int cord3 = rand() % 3 - 3;
        
        glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        const Uint8 * keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_UP] == 1){
            PositionY += elapsed * 3.1415926f / 2.0f;
            if (PositionY + 0.35f >= 2.0f) {
                PositionY -= elapsed * 3.1415926f / 2.0f;
            }
        }
        if (keys[SDL_SCANCODE_DOWN] == 1){
            PositionY -= elapsed * 3.1415926f / 2.0f;
            if (PositionY - 0.35f <= -2.0f) {
                PositionY += elapsed * 3.1415926f / 2.0f;
            }
        }
        if (keys[SDL_SCANCODE_RIGHT] == 1){
            PositionX += elapsed * 3.0f / 2.0f;
        }
        if (keys[SDL_SCANCODE_LEFT] == 1){
            PositionX -= elapsed * 3.0f / 2.0f;
        }
        
        
        glUseProgram(program.programID);
        
        //Player
        modelMatrix.identity();
        modelMatrix.Translate(PositionX, PositionY, 0.0f);
        modelMatrix.Scale(0.25f, 0.25f, 0.0f);
        
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        float square[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, square);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texcoords[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texcoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /*---------------------------------------------------------------------------------------*/
        //LBanner
        modelMatrix.identity();
        modelMatrix.Translate(-3.5f, 0.0f, 0.0f);
        modelMatrix.Scale(1.0f, 4.0f, 0.0f);
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        float leftBanner[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, leftBanner);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texcoordsLeft[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texcoordsLeft);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /*---------------------------------------------------------------------------------------*/
        //RBanner
        modelMatrix.identity();
        modelMatrix.Translate(3.5f, 0.0f, 0.0f);
        modelMatrix.Scale(1.0f, 4.0f, 0.0f);
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        float rightBanner[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, rightBanner);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texcoordsRight[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texcoordsRight);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        /*---------------------------------------------------------------------------------------*/
        //Tile
        modelMatrix.identity();
        modelMatrix.Translate(1.5f, 1.0f, 0.0f);
        modelMatrix.Scale(0.5f, 2.5f, 0.0f);
        
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        float s1[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, s1);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float t1[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, t1);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /*---------------------------------------------------------------------------------------*/
        //Tile
        modelMatrix.identity();
        modelMatrix.Translate(-1.5f, -1.0f, 0.0f);
        modelMatrix.Scale(0.5f, 2.5f, 0.0f);
        
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        float s2[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, s2);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float t2[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, t2);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /*---------------------------------------------------------------------------------------*/
        //End
        modelMatrix.identity();
        modelMatrix.Translate(-PositionX, -PositionY, 0.0f);
        modelMatrix.Scale(0.25f, 0.25f, 0.0f);
        
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        float s3[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, s3);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float t3[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, t3);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /*---------------------------------------------------------------------------------------*/
        //Tile
        modelMatrix.identity();
        modelMatrix.Translate(0.0, 0.0, 0.0f);
        modelMatrix.Scale(0.25f, 0.25f, 0.0f);
        
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        float s4[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, s4);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float t4[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, t4);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        /*---------------------------------------------------------------------------------------*/

        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.positionAttribute);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

