#include <SFML/Graphics.hpp>

#include "state.hpp"
#include "util.hpp"

#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

/**
 * The intro of the game. Contains some buttons.
 */
class IntroState : public GameState {
private:

	static IntroState instance;

	sf::Time timeTotal;

	sf::Vector2f pos;

	IntroState();
	~IntroState();
public:

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static IntroState* getInstance();
};

#endif // INTROSTATE_HPP