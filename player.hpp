#include <vector>

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
private:

	std::vector<sf::Vector2f> positions;

	float distanceTravelled = 0.0f;

	int gapcounter = 0;

	bool hit = false;

	sf::Vector2f pos;
	int radius = 5;
	float angle = 0.0f;
	sf::Time t = sf::Time::Zero;
	sf::Color color = sf::Color::Blue;

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

	void setStartingPoint(int x, int y, float angle);
	void setColor(const sf::Color& color);
	bool isColliding(const Player& other) const;
	void handleInput(const sf::Event& event);
	void update(const sf::Time& delta);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};
