#include <SDL2/SDL.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "scene.hh"

const size_t screen_width = 1280;
const size_t screen_height = 960;

const double fov_w = 90;
const double fov_h = 110;
double dist_to_screen = 1;
const int max_threads = std::thread::hardware_concurrency();

void move(Scene &sc)
{
    Vector3 r = sc.get_camera().get_position();
    double x_hit = r.x() - 256;
    double y_hit = r.y() - 256;
    sc.update_chunks(x_hit, y_hit);
}

void fill_buffer(const Scene &sc, double miny, double maxy,
                 std::vector<unsigned char> *pixels)
{
    // std::vector<unsigned char> pixels(screen_width * screen_height * 4, 0);
    // for (size_t y = screen_height - 1; y < screen_height; y--)
    for (double y = miny; y < maxy; y++)
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
            // if (p.get_color().green() == 84)
            //     std::cout << dot(normal, light_ray) << std::endl;

            /*
            // Reflection ray
            Vector3 S =
                (r.direction() - 2 * normal * dot(normal, direction))
                .normalized();
            Ray reflection_ray = Ray(r.origin() + S * 0.001, S);
            */
            size_t offset = (y * 4) * screen_width + (x * 4);
            Color color = p.get_color() * light_intensity;
            pixels->at(offset + 0) = color.blue(); // b
            pixels->at(offset + 1) = color.green(); // g
            pixels->at(offset + 2) = color.red(); // r
            pixels->at(offset + 3) = SDL_ALPHA_OPAQUE; // a
        }
    }
}

int main()
{
    // std::srand(time(NULL));
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

    int base_x = 0;
    int base_y = 0;
    size_t chunk_size = 16;
    size_t nb_chunks_x = 32;
    size_t nb_chunks_y = 32;
    Camera cam(Vector3(base_x + nb_chunks_x * chunk_size / 2,
                       base_y + nb_chunks_y * chunk_size / 2, -100),
               fov_w / 2, fov_h / 2, dist_to_screen);

    Scene sc(cam, 1, seed, nb_chunks_x, nb_chunks_y, base_x, base_y,
             chunk_size);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             screen_width, screen_height);

    SDL_Event event;
    bool running = true;
    bool useLocktexture = false;

    unsigned int frames = 0;
    Uint64 start = SDL_GetPerformanceCounter();

    int i = 0;
    std::vector<unsigned char> pixels(screen_width * screen_height * 4, 0);

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
                move(sc);
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_RIGHT == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(10, 0, 0));
                move(sc);
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_LEFT == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(-10, 0, 0));
                move(sc);
            }
            if (SDL_KEYDOWN == event.type
                && SDL_SCANCODE_DOWN == event.key.keysym.scancode)
            {
                sc.move_camera(Vector3(0, 10, 0));
                move(sc);
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
        // std::vector<unsigned char> pixels = fill_buffer(sc);
        double y_num = screen_height / max_threads;
        std::vector<std::thread> threads;
        for (int i = 0; i < max_threads - 1; i++)
        {
            threads.push_back(std::thread(fill_buffer, sc, i * y_num,
                                          (i + 1) * y_num, &pixels));
        }
        fill_buffer(sc, (max_threads - 1) * y_num, max_threads * y_num,
                    &pixels);
        for (int i = 0; i < max_threads - 1; i++)
        {
            threads[i].join();
        }

        Vector3 sunpos(i, i, -100);
        // sc.move_sun(sunpos);

        sc.move_sun(Vector3(i + sc.get_camera().get_position().x() - 256,
                            i + sc.get_camera().get_position().y() - 256,
                            -100));

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
