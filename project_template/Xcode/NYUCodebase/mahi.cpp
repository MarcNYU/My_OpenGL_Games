#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <vector>
using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
/*
 PONG
	1) Create each entity and define them
	2) Determine the inputs and how the affect each entity
	3) Move the entities
	4) Render every single time -> We do not know how to do this yet
	5) Cleanup everything remove all attrib arrays and joysticks and so forth
 */

SDL_Window* displayWindow;

class Entity {
public:
    // Constructor
    Entity(float xDir, float yDir, float rotation, float width, float height, float speed, float x, float y, Matrix &modelMatrix) :
    xDirection(xDir), yDirection(yDir), rotation(rotation), width(width), height(height), speed(speed), x(x), y(y), modelM(modelMatrix) {
        vertices = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
    };
    // Copy Constructor
    Entity& operator=(const Entity& otherEntity){
        xDirection = otherEntity.xDirection;
        yDirection = otherEntity.yDirection;
        rotation = otherEntity.rotation;
        textureID = otherEntity.textureID;
        width = otherEntity.width;
        height = otherEntity.height;
        speed = otherEntity.speed;
        x = otherEntity.x;
        y = otherEntity.y;
        modelM = otherEntity.modelM;
        return *this;
    }
    
    // Positioning
    Matrix modelM;
    float xDirection, yDirection, speed;
    // Textures
    int textureID;
    // Scaling
    float width, height;
    // Translation
    float x, y, rotation;
    // Vertices Vector
    vector<float> vertices;
    
    void drawEntity(ShaderProgram &program) {
        modelM.identity();
        modelM.Scale(width, height, 1.0f);
        modelM.Translate(x, y, 0.0f);
        
        program.setModelMatrix(modelM);
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
        glEnableVertexAttribArray(program.positionAttribute);
        
        
        glDrawArrays(GL_TRIANGLES, 0, ((int)vertices.size() / 2));
    }
};

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    return textureID;
}

void setup() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 360);
}

bool processEvents(SDL_Event &event, bool &done, float &elapsed, Entity& lPaddle, Entity &rPaddle) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return true;
        }
    }
    if (keys[SDL_SCANCODE_UP]){
        rPaddle.y += (1 * elapsed * rPaddle.speed);
    }
    if (keys[SDL_SCANCODE_DOWN]){
        rPaddle.y += (-1 * elapsed * rPaddle.speed);
    }
    if (keys[SDL_SCANCODE_W]) {
        lPaddle.y += (1 * elapsed * lPaddle.speed);
    }
    if (keys[SDL_SCANCODE_S]) {
        lPaddle.y += (-1 * elapsed * lPaddle.speed);
    }
    return false;
}

void updateBall(Entity &ball, float &elapsed) {
    ball.y += sinf(ball.yDirection) * elapsed * ball.speed;
    ball.x += cosf(ball.xDirection) * elapsed * ball.speed;
}

void update(string &text, float &lastFrameTicks, float &elapsed, float &angle, Entity &ball, Entity &leftPaddle, Entity &rightPaddle, Entity &topWall, Entity &bottomWall) {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    angle += elapsed;
    
    // update Ball Position
    updateBall(ball, elapsed);
}

void drawText(ShaderProgram *program, Matrix &modelM, int textureID, string &text, float size, float spacing) {
    
}

void render(ShaderProgram &program, string &text, Matrix &modelM, Matrix &viewM, Matrix &projectionM, Entity &ball, Entity &lPaddle, Entity &rPaddle, Entity &tWall, Entity &bWall) {
    (GL_COLOR_BUFFER_BIT);
    
    projectionM.setOrthoProjection(-5.325, 5.325, -3.0f, 3.0f, -1.5f, 1.5f);
    glUseProgram(program.programID);
    
    program.setProjectionMatrix(projectionM);
    program.setViewMatrix(viewM);
    
    ball.drawEntity(program);
    lPaddle.drawEntity(program);
    rPaddle.drawEntity(program);
    tWall.drawEntity(program);
    bWall.drawEntity(program);
    
    //if (text != "")
    //	drawText(&program, modelM, loadTexture("font1.png"), text, 0.3f, 0.05f);
    
    SDL_GL_SwapWindow(displayWindow);
}

void cleanup(ShaderProgram &program) {
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

float radianConverter(float degree){
    return (degree * (3.1415926 / 180.0));
}

int main(int argc, char *argv[])
{
    setup();
    
    ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    // Walls
    Entity topWall(0.0f, 0.0f, 0.0f, 10.65f, 0.25f, 0.0f, 0.0f, (3.0f / 0.25f) - .8f, modelMatrix);
    Entity bottomWall = topWall;
    bottomWall.y = -topWall.y;
    
    // Paddles
    Entity leftPaddle(0.0f, 0.0f, 0.0f, 0.25f, 1.0f, 0.5f, -20.5f, 0.0f, modelMatrix);
    Entity rightPaddle = leftPaddle;
    rightPaddle.x = -leftPaddle.x;
    
    // Ball
    Entity ball(0.0f, 0.0f, 0.0f, 0.1875f, 0.1875f, 0.0f, 0.0f, 0.0f, modelMatrix);
    
    SDL_Event event;
    bool done = false;
    
    float lastFrameTicks = 0.0f;
    float angle = 0.0f;
    float elapsed = 0.0f;
    string text = "";
    
    while (!done) {
        done = processEvents(event, done, elapsed, leftPaddle, rightPaddle);
        update(text, lastFrameTicks, elapsed, angle, ball, leftPaddle, rightPaddle, topWall, bottomWall);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        render(program, text, modelMatrix, viewMatrix, projectionMatrix, ball, leftPaddle, rightPaddle, topWall, bottomWall);
        cleanup(program);
    }
    
    
    SDL_Quit();
    return 0;
}