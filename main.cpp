#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "text.hpp"
#include "player.hpp"

int main() {
	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font-large.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+|/\\:;'\"<>,.?"
	);
	font->setKerning(2);

	BobbingText text(font);

	Player p;
	p.setPosition(150, 150);

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

			p.handleInput(event);
			text.handleInput(event);
		}

		text.update(elapsed);
		p.update(elapsed);

		window.clear();
		window.draw(text);
		window.draw(p);
		window.display();
	}

}