#include <SDL2/SDL.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "scene.hh"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

const double fov_w = 90;
const double fov_h = 110;
double dist_to_screen = 1;

std::vector<unsigned char> fill_buffer(const unsigned int texWidth,
                                       const unsigned int texHeight, Scene sc)
{
    std::vector<unsigned char> pixels(texWidth * texHeight * 4, 0);
    for (size_t y = SCREEN_HEIGHT - 1; y < SCREEN_HEIGHT; y--)
    // for (size_t y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (size_t x = 0; x < SCREEN_WIDTH; x++)
        {
            Ray r = sc.get_camera().get_ray(x, y);
            Vector3 pos_hit = hit(r);
            size_t hit_x = static_cast<size_t>(pos_hit.x());
            size_t hit_y = static_cast<size_t>(pos_hit.y());
            /*if (hit_x > 16*32)
                std::cout << "x: " << hit_x << " real: " << pos_hit;

            if (hit_y > 16*32)
                std::cout << "y: " << hit_y << " real: " << pos_hit;
            */
            Pixel p = sc.get_pixel(hit_x, hit_y);
            double light_intensity = dot(
                p.normal(), (sc.get_sun().get_pos() - pos_hit).normalized());
            // std::cout << light_intensity << std::endl;

            size_t offset = (y * 4) * SCREEN_WIDTH + (x * 4);
            pixels[offset + 0] = p.get_color().blue() * light_intensity; // b
            pixels[offset + 1] = p.get_color().green() * light_intensity; // g
            pixels[offset + 2] = p.get_color().red() * light_intensity; // r
            pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a
        }
    }
    return pixels;
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Bite", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                          SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    std::cout << "Renderer name: " << info.name << std::endl;
    std::cout << "Texture formats: " << std::endl;
    for (Uint32 i = 0; i < info.num_texture_formats; i++)
    {
        std::cout << SDL_GetPixelFormatName(info.texture_formats[i])
                  << std::endl;
    }

    Camera cam(Vector3(16 * 16, 16 * 16, -0.05), fov_w / 2, fov_h / 2,
               dist_to_screen);

    Scene sc(cam, 1, 2711, 32, 32);

    const unsigned int texWidth = SCREEN_WIDTH;
    const unsigned int texHeight = SCREEN_HEIGHT;
    SDL_Texture *texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                          SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight);

    SDL_Event event;
    bool running = true;
    bool useLocktexture = false;

    unsigned int frames = 0;
    Uint64 start = SDL_GetPerformanceCounter();

    int i = 0;

    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event))
        {
            if ((SDL_QUIT == event.type)
                || (SDL_KEYDOWN == event.type
                    && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
            {
                running = false;
                break;
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_L == event.key.keysym.scancode)
            {
                useLocktexture = !useLocktexture;
                std::cout << "Using "
                          << (useLocktexture ? "SDL_LockTexture() + memcpy()"
                                             : "SDL_UpdateTexture()")
                          << std::endl;
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_UP == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0, -1, 0));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_RIGHT == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(1, 0, 0));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_LEFT == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(-1, 0, 0));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_DOWN == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0, 1, 0));
            }
            /*if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_I == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0,0,-0.5));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_O == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0,0,0.5));
            }*/
        }

        // splat down some random pixels
        std::vector<unsigned char> pixels =
            fill_buffer(texWidth, texHeight, sc);

        sc.move_sun(Vector3(i / 10.0, 0, -10));

        i = (i + 1) % 100;

        if (useLocktexture)
        {
            unsigned char *lockedPixels = nullptr;
            int pitch = 0;
            SDL_LockTexture(texture, NULL,
                            reinterpret_cast<void **>(&lockedPixels), &pitch);
            std::memcpy(lockedPixels, pixels.data(), pixels.size());
            SDL_UnlockTexture(texture);
        }
        else
        {
            SDL_UpdateTexture(texture, NULL, pixels.data(), texWidth * 4);
        }

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        frames++;
        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = (end - start) / static_cast<double>(freq);
        if (seconds > 2.0)
        {
            std::cout << frames << " frames in " << std::setprecision(1)
                      << std::fixed << seconds
                      << " seconds = " << std::setprecision(1) << std::fixed
                      << frames / seconds << " FPS (" << std::setprecision(3)
                      << std::fixed << (seconds * 1000.0) / frames
                      << " ms/frame)" << std::endl;
            start = end;
            frames = 0;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
