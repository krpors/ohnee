#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "engine.hpp"
#include "playstate.hpp"
#include "state.hpp"
#include "statestack.hpp"
#include "ui.hpp"
#include "util.hpp"


/**
 * The intro of the game. Contains some buttons.
 */
class IntroState : public GameState {
public:
	IntroState(StateStack& stack, GameState::Context context);
	~IntroState();

	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:

	std::vector<std::string> introtexts;

	BobbingText text;

	sf::Time timeTotal;

	Container buttonContainer;

};

#endif // INTROSTATE_HPP
