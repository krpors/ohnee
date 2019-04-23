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

	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RenderWindow* window;

	sf::Time dt;
	sf::View view;

	Player p;

	Player other;
};

#endif // PLAYSTATE_HPP
