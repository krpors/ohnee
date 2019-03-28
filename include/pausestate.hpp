
#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "engine.hpp"
#include "state.hpp"


#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

class Engine;

class PauseState : public GameState {
private:
	Engine* engine;

	BobbingText pauseText;

	sf::Texture screencapture;

	static PauseState instance;


	PauseState();
	~PauseState();
public:
	void setEngine(Engine* const engine);

	void init() override;
	void cleanup() override;
	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static PauseState* getInstance();
};

#endif // PAUSESTATE_HPP