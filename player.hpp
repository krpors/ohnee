#include <vector>

#include <SFML/Graphics.hpp>

class Pos {
public:
	float x;
	float y;

	Pos(float x, float y) : x(x), y(y) {}
};

class Player : public sf::Drawable, public sf::Transformable {
private:
	std::vector<Pos> positions;
	int radius = 10;
	float angle = 0.0f;
	sf::Time t = sf::Time::Zero;
public:
	Player();
	~Player();

	void handleInput(const sf::Event& event);
	void update(const sf::Time& delta);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};
