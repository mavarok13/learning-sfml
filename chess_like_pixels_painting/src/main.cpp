#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    sf::VideoMode desktop_video_mode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(sf::VideoMode(desktop_video_mode.width/2, desktop_video_mode.height/2), "SFML test window");
    window.setVerticalSyncEnabled(true);

    unsigned width = desktop_video_mode.width/2;
    unsigned height = desktop_video_mode.height/2;
    unsigned channels = 4u;

    sf::Uint8 * pixels = new sf::Uint8[width * height * channels];

    unsigned pixel_size = 9u;

    sf::Texture bg_texture;
    if (!bg_texture.create(width, height)) {
        std::cout << "Couldn't create a texture" << std::endl;
        return EXIT_FAILURE;
    }
    bg_texture.update(pixels);

    sf::Sprite bg_sprite(bg_texture, sf::IntRect(0, 0, width, height));

    std::mt19937 gen(std::time(nullptr));

    int r_channel = gen()%255;
    int g_channel = gen()%255;
    int b_channel = gen()%255;

//  * Transition variables
    int r_channel_direction = 1;
    int g_channel_direction = 1;
    int b_channel_direction = 1;
//  * Transition parameter
    unsigned transition_speed = 10u;

//  * Main cycle
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

//  *   * UPDATE PIXELS: BEGIN
        bool even_row = false;
        for (unsigned y_pixel_idx = 0; y_pixel_idx < height; ++y_pixel_idx) {
            bool even_pixel = false;

            if (y_pixel_idx % (pixel_size+1) == 0) {
                even_row = !even_row;
            }

            for (unsigned x_pixel_idx = 0; x_pixel_idx < width; ++x_pixel_idx) {
                if (even_row) {
                    if (x_pixel_idx % (pixel_size+1) == 0) {
                        even_pixel = !even_pixel;
                    }

                    pixels[y_pixel_idx*width*channels+x_pixel_idx*channels] = even_pixel*r_channel;
                    pixels[y_pixel_idx*width*channels+x_pixel_idx*channels+1] = even_pixel*g_channel;
                    pixels[y_pixel_idx*width*channels+x_pixel_idx*channels+2] = even_pixel*b_channel;
                } else {
                    pixels[y_pixel_idx*width*channels+x_pixel_idx*channels] = even_pixel*r_channel;
                    pixels[y_pixel_idx*width*channels+x_pixel_idx*channels+1] = even_pixel*g_channel;
                    pixels[y_pixel_idx*width*channels+x_pixel_idx*channels+2] = even_pixel*b_channel;

                    if ((x_pixel_idx+1) % (pixel_size+1) == 0) {
                        even_pixel = !even_pixel;
                    }
                }
            }
        }

        if (r_channel >= 255) {
            r_channel_direction = -1;
        } else if (r_channel <= 0) {
            r_channel_direction = 1;
        }

        if (g_channel >= 255) {
            g_channel_direction = -1;
        } else if (g_channel <= 0) {
            g_channel_direction = 1;
        }

        if (b_channel >= 255) {
            b_channel_direction = -1;
        } else if (b_channel <= 0) {
            b_channel_direction = 1;
        }

        r_channel += transition_speed*r_channel_direction;
        g_channel += transition_speed*g_channel_direction;
        b_channel += transition_speed*b_channel_direction;

        r_channel = std::clamp(r_channel, 0, 255);
        g_channel = std::clamp(g_channel, 0, 255);
        b_channel = std::clamp(b_channel, 0, 255);
//  *   * UPDATE PIXELS: END
        bg_texture.update(pixels);

        window.draw(bg_sprite);

        window.display();
    }

    return 0;
}
