#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

class Player; // forward decl


class Arrow : public sf::Drawable, public sf::Transformable {
private:
	sf::Time timer;
	sf::VertexArray varr;
public:
	Arrow();
	~Arrow();

	void update(const sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

// =============================================================================

class BlastParticle : public sf::CircleShape {
private:
	float speed = 100.0f;
	float angle = 0.0f;
	float maxradius;
	sf::Time maxlife = sf::seconds(2);
	sf::Time life = sf::seconds(2);
public:
	BlastParticle(float speed, float angle, float maxradius, const sf::Time& maxlife);
	~BlastParticle();

	bool isAlive() const;
	void init(float speed, float angle, float maxradius, const sf::Time& maxlife);
	void update(const sf::Time& dt);
};

// =============================================================================

/**
 * This will generate a 'blast' when a player dies. It's a specific particle
 * generator.
 */
class BlastGenerator : public sf::Drawable {
private:
	sf::Time time;

	std::vector<BlastParticle> particles;

public:
	BlastGenerator();
	~BlastGenerator();

	void init(const Player& p);
	void update(const sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

// =============================================================================

class Player : public sf::Drawable {
private:
	bool drawArrow = true;
	Arrow arrow;

	std::vector<sf::CircleShape> positions;

	bool dead = false;

	sf::Vector2f pos;
	int radius = 5;
	float angle = 0.0f;
	sf::Time t = sf::Time::Zero;
	sf::Color color = sf::Color::Red;

	sf::Time totalTime = sf::Time::Zero; // The total time.
	sf::Time startAfter = sf::seconds(2); // the time when the actual trail starts.

	/**
	 * The blast generator is just a particle generator which emits some particles
	 * using the same angle of the player upon death.
	 */
	BlastGenerator blastGenerator;

	/**
	 * The emplacement counter is used to determine when to add a new position
	 * vector to the `positions` vector. This is to prevent adding too many
	 * items in the vector because it's not really necessary for collision checking
	 * and trail drawing.
	 */
	sf::Time emplacementCounter = sf::Time::Zero;

	bool moveLeft = false;
	bool moveRight = false;

	bool collides(const sf::Vector2f pos1, int radius1, sf::Vector2f pos2, int radius2) const;

	bool isCollidingWithSelf() const;
public:
	Player();
	~Player();

	/**
	 * Resets the state of the player (i.e. start over fresh).
	 */
	void reset();

	const sf::Vector2f& getPosition() const;

	float getAngle() const;
	void setStartingPoint(int x, int y, float angle);
	void setColor(const sf::Color& color);

	/**
	 * Stop moving left or right.
	 */
	void unmove();
	const sf::Color& getColor() const;
	// TODO: refactor isCollidingWithSelf.
	bool isColliding(const Player& other) const;
	void die();
	void handleInput(const sf::Event& event);
	void update(const sf::Time& delta);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	const std::string str() const;
};

#endif // PLAYER_HPP
