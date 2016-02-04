#include <iostream>
#include <string>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

SDL_Texture * LoadImage(std::string file){
    SDL_Surface *loadedImage = nullptr;
    SDL_Texture *texture = nullptr;

    loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr){
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    else std::cerr << SDL_GetError() << std::endl;
    return texture;
}

int main(int argc, char *argv[]){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0 ){
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    atexit(SDL_Quit);

    window = SDL_CreateWindow(
            "SDL2 Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_OPENGL
    );
    if (window == nullptr){
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
