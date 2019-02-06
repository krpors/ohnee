#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "text.hpp"

int main() {
	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font-large.png",
		" abcdefghijlkmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+|/\\:;'\"<>,.?"
	);
	font->setKerning(2);
	Text t1(font);
	t1.setText(20, 20, "There is some text here");

	sf::RenderWindow window(sf::VideoMode(800, 600), "OHNEE v0.0.1", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape: window.close(); break;
				default: break;
				}
			}
		}

		window.clear();
		window.draw(t1);
		window.display();
	}

}