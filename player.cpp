#include <iostream>
#include <cmath>

#include "player.hpp"

Player::Player() {
}

Player::~Player() {

}

bool Player::collides(const sf::Vector2f pos1, int radius1, sf::Vector2f pos2, int radius2) const {
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	float dist = std::sqrt(dx * dx + dy * dy);
	return dist < (radius1 + radius2);
}

void Player::setStartingPoint(int x, int y, float angle) {
	this->pos.x = x;
	this->pos.y = y;
	this->angle = angle;

}

void Player::setColor(const sf::Color& color) {
	this->color = color;
}

bool Player::isCollidingWithSelf() const {
	// Do not check for all previous positions or we would collide with
	// ourselves right away. Instead. we iterate from start to end - 10.
	std::vector<sf::Vector2f>::size_type tosubtract = 10;
	if (this->positions.size() < tosubtract) {
		tosubtract = this->positions.size();
	}

	for (auto it = this->positions.begin(); it != this->positions.end() - tosubtract; it++) {
		if (this->collides(this->pos, this->radius, *it, this->radius)) {
			return true;
		}
	}

	return false;
}

bool Player::isColliding(const Player& other) const {
	// std::cout << std::endl;
	return false;
}

void Player::handleInput(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft = true;  break;
		case sf::Keyboard::Right: this->moveRight = true; break;
		default: break;
		}
	} else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::Left:  this->moveLeft =  false;  break;
		case sf::Keyboard::Right: this->moveRight = false; break;
		default: break;
		}
	}
}

void Player::update(const sf::Time& dt) {
	t += dt;

	if (this->moveLeft) {
		this->angle -= 140 * dt.asSeconds();
	}
	if (this->moveRight) {
		this->angle += 140 * dt.asSeconds();
	}

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

	this->hit = false;
	if (this->isCollidingWithSelf()) {
		this->hit = true;
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Draw the path of the player
	for (const sf::Vector2f& p : this->positions) {
		sf::CircleShape a;
		a.setRadius(this->radius);
		a.setPosition(p.x, p.y);
		a.setFillColor(this->color);
		target.draw(a, states);
	}

	sf::Color c = sf::Color::Blue;
	if (this->hit) {
		c = sf::Color::Red;
	}

	sf::CircleShape shape;
	shape.setRadius(this->radius);
	shape.setPosition(this->pos);
	shape.setFillColor(c);

	target.draw(shape, states);

	sf::VertexArray varr(sf::Points);
	varr.append(sf::Vertex(this->pos, sf::Color::Yellow));

	target.draw(varr, states);
}