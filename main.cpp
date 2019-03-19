#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "text.hpp"

void test() {
	sf::RenderWindow window(sf::VideoMode(640, 480), "Yo", sf::Style::Close);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		window.clear();
		//
		window.display();
	}
}

int main() {
	// test();
	Engine e;
	e.run();

	return 0;
}