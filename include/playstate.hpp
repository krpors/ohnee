#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP


#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "state.hpp"
#include "statestack.hpp"



class PlayState : public GameState {
public:
	PlayState(StateStack& stack, GameState::Context context);
	~PlayState();

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// TODO: text from a resource cache/context or whatever.

	Player p;
};

#endif // PLAYSTATE_HPP
