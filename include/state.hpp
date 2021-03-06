#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>

// We require a forward declaration here to prevent things from fucking up
// due to circular dependencies.
class StateStack;
class Engine;

class GameState : public sf::Drawable {
public:

	/**
	 * The Context struct can be used to pass around. It contains resources
	 * which must be 'shared' throughout the game.
	 */
	struct Context {
	public:
		sf::RenderWindow* window;

		Engine* engine;
	};

public:

	GameState(StateStack& stateStack, GameState::Context context);
	virtual ~GameState();

	virtual void activate();
	virtual void deactivate();

	virtual	void handleInput(const sf::Event& event) = 0;
	virtual void update(const sf::Time& dt) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

protected:
	StateStack* stateStack;
	GameState::Context context;
};

#endif // STATE_HPP
