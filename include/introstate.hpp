#include <iostream>

#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "playstate.hpp"
#include "state.hpp"
#include "statestack.hpp"
#include "text.hpp"
#include "util.hpp"
#include "ui.hpp"

#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

/**
 * The intro of the game. Contains some buttons.
 */
class IntroState : public GameState {
private:

	Text text;

	sf::Time timeTotal;

	Button btnStuff;

public:
	IntroState(StateStack& stack, GameState::Context context);
	~IntroState();

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

#endif // INTROSTATE_HPP
