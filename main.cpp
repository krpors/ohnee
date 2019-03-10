#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "text.hpp"
#include "player.hpp"
#include "util.hpp"

int main() {
	std::shared_ptr<ImageFont> font = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+|/\\:;'\"<>,.?"
	);
	font->setKerning(2);

	ParticleGenerator pgen;
	FpsCounter fps;

	Text fpsText(font);
	Text playerText(font);
	playerText.setPosition(0, 40);

	BobbingText text(font);

	Player p;

	BlastGenerator blastGenerator;

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
				case sf::Keyboard::L: blastGenerator.init(p); break;
				case sf::Keyboard::Space: paused = !paused; break;
				default: break;
				}
			}

			p.handleInput(event);
			text.handleInput(event);
		}

		if (!paused) {
			fps.update(elapsed);
			text.update(elapsed);
			p.update(elapsed);
			pgen.update(elapsed);
			blastGenerator.update(elapsed);
		}

		std::stringstream ss;
		ss << "FPS: " << fps.getFps();
		fpsText.setText(ss.str());
		playerText.setText(p.str());

		window.clear();
		window.draw(p);
		window.draw(fpsText);
		window.draw(playerText);
		window.draw(blastGenerator);

		window.display();
	}

	return 0;
}