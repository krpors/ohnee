#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "engine.hpp"
#include "state.hpp"


class PauseState : public GameState {
private:
	sf::Texture screencapture;

	BobbingText pauseText;

public:
	PauseState(StateStack& stack, GameState::Context context);
	~PauseState();

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

#endif // PAUSESTATE_HPP
