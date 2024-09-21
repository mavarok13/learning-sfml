#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <random>
#include <ctime>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

int main (int argc, char * argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: app <images dir path>" << std::endl; 
        return EXIT_FAILURE;
    }

//  * INITIALIZE IMAGES PATHS
    std::vector<std::string> images_paths;

    for (auto & file : fs::directory_iterator(argv[1])) {
        if (fs::is_regular_file(file.path())) {
            images_paths.emplace_back(file.path().string());
        }
    }

//  * CREATE RANDOM GENERATOR
    std::mt19937 gen(std::time(nullptr));

//  * SFML
    unsigned width = sf::VideoMode::getDesktopMode().width;
    unsigned height = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(width, height), "Sprite drawer");

    float food_sprite_size = 2.5f;

    std::vector<sf::Sprite> sprites;
    std::vector<sf::Texture> textures;
    textures.reserve(images_paths.size());

    for (auto & texture_path : images_paths) {
        sf::Texture texture;

        if (!texture.loadFromFile(texture_path)) {
            std::cerr << "Couldn't load texture" << std::endl;
            return EXIT_FAILURE;
        }

        textures.emplace_back(texture);
    }

    while (window.isOpen()) {
        sf::Event e;
        while(window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            } else if (e.type == sf::Event::MouseButtonReleased) {
                unsigned mouse_position_x = e.mouseButton.x;
                unsigned mouse_position_y = e.mouseButton.y;

                sf::Texture & picked_texture = textures.at(gen()%textures.size());

                sf::Sprite food_sprite(picked_texture);
                food_sprite.setScale(food_sprite_size, food_sprite_size);
                food_sprite.setPosition(mouse_position_x-(food_sprite_size*picked_texture.getSize().x)/2, mouse_position_y-(food_sprite_size*picked_texture.getSize().y)/2);
                
                sprites.emplace_back(std::move(food_sprite));
            }
        }

        window.clear(sf::Color::Black);

        for(sf::Sprite sprite : sprites) {
            window.draw(sprite);
        }

        window.display();
    }

    return 0;
}