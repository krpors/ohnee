#include <SFML/Graphics.hpp>

#ifndef STATE_HPP
#define STATE_HPP

// We require a forward declaration here to prevent things from fucking up
// due to circular dependencies.
class StateStack;
class Engine;

class GameState : public sf::Drawable {
public:

	class Context {
		public:
			Engine* engine;
	};

public:

	GameState(StateStack& stateStack, GameState::Context context);
	virtual ~GameState();



	/**
	 * Called when the state is pushed onto the stack.
	 */
	virtual void init() = 0;

	/**
	 * Called when the state is popped from the stack.
	 */
	virtual void cleanup() = 0;

	virtual	void handleInput(const sf::Event& event) = 0;
	virtual void update(const sf::Time& dt) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

protected:
	StateStack* stateStack;
	GameState::Context context;
};

#endif // STATE_HPP