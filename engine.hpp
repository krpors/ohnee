#include <memory>
#include <stack>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "text.hpp"

#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine; // forward decl


class GameState : public sf::Drawable {
public:
	GameState();
	virtual ~GameState();

	virtual	void handleInput(const sf::Event& event) = 0;
	virtual void update(const sf::Time& dt) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
};

// =============================================================================

class PauseState : public GameState {
private:
	Engine* engine;
	Text pauseText;

	static PauseState instance;

	PauseState();
	~PauseState();
public:
	void init(Engine* const engine);

	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static PauseState* getInstance();
};

// =============================================================================

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

	PlayState();
	~PlayState();

	static PlayState instance;
public:

	void init(Engine* const engine);

	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static PlayState* getInstance();
};

// =============================================================================

class Engine {
private:
	std::shared_ptr<sf::RenderWindow> renderWindow;

	std::shared_ptr<ImageFont> fontSmall;
	std::shared_ptr<ImageFont> fontLarge;

	std::stack<GameState*> stateStack;

	bool quit = false;
public:
	Engine();
	~Engine();

	const std::shared_ptr<ImageFont>& getFontSmall() const;
	const std::shared_ptr<ImageFont>& getFontLarge() const;

	void setQuit(bool quit);
	void pushState(GameState* const state);
	void popState();
	void run();

};

#endif // ENGINE_HPP