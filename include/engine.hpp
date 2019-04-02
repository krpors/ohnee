#include <memory>
#include <stack>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "player.hpp"
#include "state.hpp"
#include "text.hpp"

#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine {
private:
	// TODO: unique_ptr
	std::shared_ptr<sf::RenderWindow> renderWindow;

	std::shared_ptr<ImageFont> fontSmall;
	std::shared_ptr<ImageFont> fontLarge;

	bool quit = false;
public:
	Engine();
	~Engine();

	const std::shared_ptr<ImageFont> getFontSmall() const;
	const std::shared_ptr<ImageFont> getFontLarge() const;

	const sf::RenderWindow& getRenderWindow() const;
	void setQuit(bool quit);
	void run();

};

#endif // ENGINE_HPP
