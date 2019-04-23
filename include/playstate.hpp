#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP


#include <SFML/Graphics.hpp>

#include "engine.hpp"
#include "state.hpp"
#include "statestack.hpp"

/**
 * This class inherits the basic properties from the sf::View class, but also
 * has some encapsulated logic to perform some 'screen shaking' mechanism, by
 * altering the center values relative to the original 'other' view.
 */
class ShakeView : public sf::View {
public:
	ShakeView();
	/**
	 * Copy constructor to copy the original values of the 'other' View.
	 * Internalle, the 'original' view is set.
	 */
	ShakeView(const sf::View& other);

	/**
	 * When true, shake. When false, stop immediately.
	 */
	void setShaking(bool shake);

	/**
	 * Updates the view by altering the center of the view to visualize a
	 * shaking screen.
	 */
	void update(const sf::Time& dt);
private:
	// This is the original view, without shaking and stuff. This is used as a
	// reference point to change the center position to mimic shaking.
	sf::View original;

	// Timer which starts when shaking is set to true.
	sf::Time shakeTimer;

	// True if we needs to shakes.
	bool shaking;

	// The relative change to the center position of the view.
	float shakeFactor;
};

// =============================================================================


class PlayState : public GameState {
public:
	PlayState(StateStack& stack, GameState::Context context);
	~PlayState();

	void handleInput(const sf::Event& event) override;
	void update(const sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RenderWindow* window;

	ShakeView shakeView;

	Player p;

	Player other;
};

#endif // PLAYSTATE_HPP
