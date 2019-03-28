#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "state.hpp"

#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

class Engine;

class PlayState : public GameState {
private:
	/**
	 * Engine is merely the pointer to the central Engine. The state cannot
	 * exist without the engine, and the state does not "own" the engine and
	 * is not in charge of freeing up the Engine as a resource. That's why I
	 * chose to make it a raw pointer instead of using shared_ptr/weak_ptr
	 * trickery. Let's see how this works out.
	 */
	Engine* engine;

	Text text;

	Player p;
public:
	PlayState();

	void setEngine(Engine* const engine);

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // PLAYSTATE_HPP