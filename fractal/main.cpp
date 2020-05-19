//
//  main.cpp
//  fractal
//
//  Created by Anton Segerkvist on 2020-05-11.
//  Copyright © 2020 Anton Segerkvist. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>

struct Point
{
    double x;
    double y;
};

struct Rect
{
    double x;
    double y;
    double w;
    double h;
};

void drawPixel(SDL_Renderer* renderer,
               const Rect& pixelRect,
               const Rect& areaRect,
               const Point& pixel);

int main(int argc, const char * argv[])
{
    const int windowWidth = 1024;
    const int windowHeight = 720;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Mandelbrot set",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          windowWidth,
                                          windowHeight,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return -1;
    }
    
    bool quit = false;
    SDL_Event event;

    Rect pixelView;
    pixelView.x = 0;
    pixelView.y = 0;
    pixelView.w = windowWidth;
    pixelView.h = windowHeight;
    
    Rect zoomView;
    zoomView.x = -2;
    zoomView.y = -1;
    zoomView.w = 3;
    zoomView.h = 2;
    
    while(!quit)
    {
        while(SDL_PollEvent(&event) != 0)
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        for (int i = 0; i < windowWidth; ++i)
        {
            for (int j = 0; j < windowHeight; ++j)
            {
                Point point;
                point.x = i;
                point.y = j;
                
                drawPixel(renderer,
                          pixelView,
                          zoomView,
                          point);
            }
        }
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

void drawPixel(SDL_Renderer* renderer,
               const Rect& pixelRect,
               const Rect& areaRect,
               const Point& pixel)
{
    // Map to artificial coordinate system.
    const double cx = areaRect.w / pixelRect.w * pixel.x + areaRect.x;
    const double cy = areaRect.h / pixelRect.h * pixel.y + areaRect.y;
    double x = 0;
    double y = 0;
    
    // Perform iterations.
    
    const int maxIterations = 100;
    int iterations = 0;
    while (x*x + y*y < 4 && iterations < maxIterations) {
        const double xtemp = x * x - y * y + cx;
        y = 2 * x * y + cy;
        x = xtemp;
        iterations += 1;
    }
    
    const int color = (255.0 / maxIterations) * iterations;
    SDL_SetRenderDrawColor(renderer, color, color, color, 255);
    SDL_RenderDrawPoint(renderer, pixel.x, pixel.y);
}
