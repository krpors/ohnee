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
		this->angle -= 8;
		break;
	case sf::Keyboard::Right:
		this->angle += 8;
	default: break;
	}
}

void Player::update(const sf::Time& dt) {
	t += dt;

	float bleh = M_PI / 180.0f;

	float dx = std::cos(this->angle * bleh) * 120 * dt.asSeconds();
	float dy = std::sin(this->angle * bleh) * 120 * dt.asSeconds();

	pos.x += dx;
	pos.y += dy;

	int threshold = this->radius / 3; // in pixels

	// Calculate the distance travelled by adding the delta x and y
	this->distanceTravelled += std::fabs(dx) + std::fabs(dy);

	// if the distance travelled exceeds the threshold, add the current position
	// to the history of positions so we can draw the path we travelled later on.
	// This is more of an optimization step rather than a visual step.
	if (this->distanceTravelled > threshold) {
		this->positions.emplace_back(pos);
		this->distanceTravelled = 0.0f;
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::CircleShape shape;
	shape.setRadius(this->radius);
	shape.setPosition(this->pos);
	shape.setFillColor(sf::Color::Red);

	target.draw(shape, states);

	sf::VertexArray varr(sf::Points);
	varr.append(sf::Vertex(this->pos, sf::Color::Yellow));

	target.draw(varr, states);

	// Draw the path of the player
	for (sf::Vector2f p : this->positions) {
		sf::CircleShape a;
		a.setRadius(this->radius);
		a.setPosition(p.x, p.y);
		a.setFillColor(sf::Color::Red);
		target.draw(a, states);
	}
}