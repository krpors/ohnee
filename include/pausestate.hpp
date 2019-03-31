
#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "engine.hpp"
#include "state.hpp"


#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

class PauseState : public GameState {
private:
	sf::Texture screencapture;

public:
	PauseState(StateStack& stack, GameState::Context context);

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

#endif // PAUSESTATE_HPP