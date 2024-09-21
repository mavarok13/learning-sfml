#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <random>
#include <ctime>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace filesys = std::filesystem;

int main(int argc, char * argv[]) {
//  *   COMMAND LINE ARGS HANDLER
    std::string textures_dir_str;

    if (argc == 2) {
        textures_dir_str = argv[1];
    } else {
        std::cout << "Usage: app <texture path>" << std::endl;
        return EXIT_FAILURE;
    }

//  * LOAD TEXTURES
    std::vector<std::string> textures_paths;

    filesys::path textures_dir(textures_dir_str);

    for (const auto & dir_it : filesys::directory_iterator(textures_dir)) {
        if (filesys::is_regular_file(dir_it.path())) {
            textures_paths.emplace_back(dir_it.path().string());
        }
    }

//  * INIT RANDOM GENERATOR
    std::mt19937 gen(std::time(nullptr));

//  * CREATE WINDOW
    unsigned width = 726u;
    unsigned height = 412u;

    sf::RenderWindow window(sf::VideoMode(width, height), "Sprites drawing");

//  * LOAD TEXTURE
    sf::Texture sprite_texture;

    if (!sprite_texture.loadFromFile(textures_paths.at(gen()%textures_paths.size()))) {
        std::cerr << "Can't load image for texture" << std::endl;
        return EXIT_FAILURE;
    }

//  * CREATE SPRITE
    float sprite_size = 5.f;

    sf::Sprite meal_sprite;
    meal_sprite.setTexture(sprite_texture);
    meal_sprite.setScale(sprite_size, sprite_size);
    meal_sprite.setPosition(window.getSize().x/2-(meal_sprite.getScale().x*meal_sprite.getTexture()->getSize().x)/2, window.getSize().y/2-(meal_sprite.getScale().y*meal_sprite.getTexture()->getSize().y)/2);

//  * MAIN WINDOW CYCLE
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.draw(meal_sprite);
        window.display();
    }

    return 0;
}