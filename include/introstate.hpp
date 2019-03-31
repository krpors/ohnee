#include <iostream>

#include <SFML/Graphics.hpp>

#include "state.hpp"
#include "statestack.hpp"
#include "util.hpp"

#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

/**
 * The intro of the game. Contains some buttons.
 */
class IntroState : public GameState {
private:

	sf::Time timeTotal;

	sf::Vector2f pos;

public:
	IntroState(StateStack& stack, GameState::Context context);

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

#endif // INTROSTATE_HPP