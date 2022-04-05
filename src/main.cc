#include <SDL2/SDL.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "scene.hh"

const size_t screen_width = 1280;
const size_t screen_height = 960;

const double fov_w = 90;
const double fov_h = 110;
double dist_to_screen = 1;

std::vector<unsigned char> fill_buffer(Scene sc)
{
    std::vector<unsigned char> pixels(screen_width * screen_height * 4, 0);
    // for (size_t y = screen_height - 1; y < screen_height; y--)
    for (double y = 0; y < screen_height; y++)
    {
        for (double x = 0; x < screen_width; x++)
        {
            double x_pixel = x / screen_width;
            double y_pixel = y / screen_height;
            Ray r = sc.get_camera().get_ray(x_pixel, y_pixel);
            Vector3 pos_hit = hit(r);
            double x_hit = pos_hit.x();
            double y_hit = pos_hit.y();
            Pixel p = sc.get_pixel(x_hit, y_hit);
            Vector3 light_ray = (pos_hit - sc.get_sun().get_pos()).normalized();

            Vector3 normal = p.normal();
            if (dot(normal, r.direction()) < 0)
                normal = -normal;
            double light_intensity =
                dot(normal, light_ray) * sc.get_sun().get_intensity();

            /*
            // Reflection ray
            Vector3 S =
                (r.direction() - 2 * normal * dot(normal, direction))
                .normalized();
            Ray reflection_ray = Ray(r.origin() + S * 0.001, S);
            */
            size_t offset = (y * 4) * screen_width + (x * 4);
            Color color = p.get_color() * light_intensity;
            pixels[offset + 0] = color.blue(); // b
            pixels[offset + 1] = color.green(); // g
            pixels[offset + 2] = color.red(); // r
            pixels[offset + 3] = SDL_ALPHA_OPAQUE; // a
        }
    }
    return pixels;
}

int main()
{
    std::srand(time(NULL));
    double seed = std::rand();
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Bite", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, screen_width,
                                          screen_height, SDL_WINDOW_SHOWN);

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

    Camera cam(Vector3(16 * 16, 16 * 16, -100), fov_w / 2, fov_h / 2,
               dist_to_screen);

    Scene sc(cam, 1, seed, 32, 32, 16);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             screen_width, screen_height);

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
                sc.move_camera(Vector3(0, -10, 0));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_RIGHT == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(10, 0, 0));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_LEFT == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(-10, 0, 0));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_DOWN == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0, 10, 0));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_I == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0, 0, 5));
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_O == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0, 0, -5));
            }
        }

        // splat down some random pixels
        std::vector<unsigned char> pixels = fill_buffer(sc);

        Vector3 sunpos(i, i, -100);
        sc.move_sun(sunpos);
        // std::cout << sunpos;

        // sc.move_sun(Vector3(sc.get_camera().get_center().x(),
        //                     sc.get_camera().get_center().y(), -100));

        i = (i + 5) % (16 * 32);

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
            SDL_UpdateTexture(texture, NULL, pixels.data(), screen_width * 4);
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
