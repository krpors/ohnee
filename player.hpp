#include <vector>

#include <SFML/Graphics.hpp>

class Pos {
public:
	int x;
	int y;
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
