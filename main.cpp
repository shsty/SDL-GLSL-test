#include <cstdio>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <GL/gl.h>

const char * WINDOW_NAME = "SDL & GL test";

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *window;
//SDL_Renderer *renderer = nullptr;
SDL_GLContext context;

void sdldie(const char * msg){
    fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
    SDL_Quit(); 
    exit(1);
}

void checkSDLError(int line = -1)
{
    const char * error = SDL_GetError();
    if (*error != '\0'){
        fprintf(stderr, "SDL Error:%s\n", error);
        if (line != -1) fprintf(stderr, " + line: %i\n", line);
        SDL_ClearError();
    }
}

/*SDL_Texture * LoadImage(std::string file){
    SDL_Surface *loadedImage;
    SDL_Texture *texture = nullptr;

    loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr){
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    else checkSDLError();
    return texture;
}*/

int main(int argc, char *argv[]){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0 ){
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window = SDL_CreateWindow(
            WINDOW_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_OPENGL
    );
    if (window == nullptr) sdldie("Unable to create window");

    context = SDL_GL_CreateContext(window);
    checkSDLError();

    SDL_version sdlversion;
    SDL_GetVersion(&sdlversion);
    std::cout << "SDL Version " << (int)sdlversion.major << "." << (int)sdlversion.minor << "." << (int)sdlversion.patch << std::endl;
    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
    /*renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) sdldie("Unable to create renderer");

    SDL_Texture *texture = LoadImage("hello.bmp");
    if (texture == nullptr){
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(1000);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);*/

    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);

    SDL_Delay(500);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
