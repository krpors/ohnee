#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "state.hpp"
#include "statestack.hpp"

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP


class PlayState : public GameState {
private:
	// TODO: text from a resource cache/context or whatever.

	Player p;
public:
	PlayState(StateStack& stack, GameState::Context context);

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // PLAYSTATE_HPP