#include <iostream>
#include <cmath>

#include "player.hpp"


Player::Player() {
}

Player::~Player() {

}

void Player::handleInput(const sf::Event& event) {
	if (event.type != sf::Event::KeyPressed) {
		return;
	}

	switch (event.key.code) {
	case sf::Keyboard::Left:
		std::cout << "Herro!" << std::endl;
		this->angle -= 8;
		break;
	case sf::Keyboard::Right:
		this->angle += 8;
	default: break;
	}
}

void Player::update(const sf::Time& dt) {
	t += dt;

	sf::Vector2f pos = this->getPosition();

	float bleh = M_PI / 180.0f;

	float dx = std::cos(this->angle * bleh) * 80 * dt.asSeconds();
	float dy = std::sin(this->angle * bleh) * 80 * dt.asSeconds();

	std::cout << "Angle: " << this->angle
		<< ", dx: " << dx
		<< ", dy: " << dy
		<< std::endl;

	move(dx, dy);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	sf::CircleShape shape;
	shape.setRadius(this->radius);
	shape.setPosition(getPosition());
	shape.setFillColor(sf::Color::Red);

	target.draw(shape, states);
}