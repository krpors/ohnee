#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "text.hpp"
#include "player.hpp"

int main_() {
	std::vector<int> vec;
	for (int i = 0; i < 50; i++) {
		vec.push_back(i);
	}

	std::cout << vec.size() << std::endl;

	std::vector<int>::size_type sz = 10;
	if (vec.size() < sz) {
		sz = vec.size();
	}
	for(auto it = vec.begin(); it != vec.end() - sz; it++) {
		std::cout << *it << std::endl;
	}

	return 0;
}

int main() {
	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font-large.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+|/\\:;'\"<>,.?"
	);
	font->setKerning(2);

	BobbingText text(font);

	Player p;

	sf::ContextSettings ctx;
	ctx.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(800, 600), "OHNEE v0.0.1", sf::Style::Close, ctx);
	window.setVerticalSyncEnabled(true);

	bool paused = false;

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
				case sf::Keyboard::Space: paused = !paused; break;
				default: break;
				}
			}

			p.handleInput(event);
			text.handleInput(event);
		}

		if (paused) {
			continue;
		}

		text.update(elapsed);
		p.update(elapsed);

		window.clear();
		// window.draw(text);
		window.draw(p);

		window.display();
	}

	return 0;
}