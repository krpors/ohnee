#include <vector>

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
private:

	std::vector<sf::Vector2f> positions;

	float distanceTravelled = 0.0f;

	sf::Vector2f pos;
	int radius = 10;
	float angle = 0.0f;
	sf::Time t = sf::Time::Zero;
	sf::Color color = sf::Color::Red;

	bool moveLeft = false;
	bool moveRight = false;
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
