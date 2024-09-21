#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

int main () {
    std::cout << "Enter text: ";
    
    std::string user_text;

    char c = 0;
    while (std::cin.get(c) && c != '\n') {
        user_text.push_back(c);
    }

    sf::Font font;
    if (!font.loadFromFile("../../../resources/fonts/Marcha Stories.otf")) {
        std::cerr << "Can't load font" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned width = sf::VideoMode::getDesktopMode().width;
    unsigned height = sf::VideoMode::getDesktopMode().height;

    sf::Text text;
    text.setFont(font);
    text.setString(user_text);
    text.setCharacterSize(100u);
    text.setFillColor(sf::Color::White);
    text.setPosition((width-text.getGlobalBounds().width)/2, (height-text.getGlobalBounds().height)/2);

    sf::RenderWindow window(sf::VideoMode(width, height), "Text drawing");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }

    return 0;
}