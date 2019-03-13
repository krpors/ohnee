#include <cmath>
#include <iostream>
#include <sstream>

#include "player.hpp"

Arrow::Arrow() {
	this->varr.setPrimitiveType(sf::Lines);
	sf::Vertex arrowBaseStart({ 0,  0 }, sf::Color::Red);
	sf::Vertex arrowBaseEnd  ({ 20, 0 }, sf::Color::Red);

	sf::Vertex leftStart({ 20,  0 }, sf::Color::Red);
	sf::Vertex leftEnd  ({ 15, -5 }, sf::Color::Red);

	sf::Vertex rightStart({ 20, 0 }, sf::Color::Red);
	sf::Vertex rightEnd  ({ 15, 5 }, sf::Color::Red);

	// this->varr.append(arrowBaseStart);
	// this->varr.append(arrowBaseEnd);

	this->varr.append(leftStart);
	this->varr.append(leftEnd);

	this->varr.append(rightStart);
	this->varr.append(rightEnd);

	// Double arrow!
	this->varr.append(sf::Vertex({ 22, 0 }, sf::Color::Red));
	this->varr.append(sf::Vertex({ 17, -5 }, sf::Color::Red));

	this->varr.append(sf::Vertex({ 22, 0 }, sf::Color::Red));
	this->varr.append(sf::Vertex({ 17, 5 }, sf::Color::Red));

	setOrigin(6, 0);
}

Arrow::~Arrow() {
}

void Arrow::update(const sf::Time& dt) {
	this->timer += dt;
	float scale = std::fabs(1 * std::sin(2 * M_PI * this->timer.asSeconds())) + 1;
	this->setScale(scale, scale);
}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	target.draw(this->varr, states);
}

//==============================================================================

BlastParticle::BlastParticle(float speed, float angle, float radius, const sf::Time& maxlife) :
		speed(speed),
		angle(angle),
		maxradius(radius),
		maxlife(maxlife),
		life(maxlife) {
	sf::CircleShape::setPosition(20, 20);
}

BlastParticle::~BlastParticle() {
}

bool BlastParticle::isAlive() const {
	return this->life > sf::Time::Zero;
}

void BlastParticle::update(const sf::Time& dt) {
	this->life -= dt;
	if (this->life <= sf::Time::Zero) {
		return;
	}

	float percentageLife = this->life / this->maxlife;

	float rad = M_PI / 180.0f;

	float dx = std::cos(this->angle * rad) * this->speed * dt.asSeconds();
	float dy = std::sin(this->angle * rad) * this->speed * dt.asSeconds();

	sf::Vector2f curpos = sf::CircleShape::getPosition();
	sf::CircleShape::setPosition(curpos.x + dx, curpos.y + dy);
	sf::CircleShape::setRadius(this->maxradius * percentageLife);
}

//==============================================================================

BlastGenerator::BlastGenerator() {
	// TODO: investigate the usage of only two or three RNG/Dists in an application,
	// like a global var.
	this->rng.seed(std::random_device()());
	this->dist  = std::uniform_real_distribution<>(-1.0, 1.0);
	this->dist2 = std::uniform_real_distribution<>(0.0, 1.0);

}

BlastGenerator::~BlastGenerator() {
}

void BlastGenerator::init(const Player& player) {
	this->particles.clear();
	for (int i = 0; i < 80; i++) {
		sf::Time maxlife = sf::milliseconds(this->dist2(this->rng) * 2000);
		float speed = 200.0f * this->dist2(this->rng);
		float pangle = this->dist(this->rng) * 20 + player.getAngle();
		float radius = 5 * this->dist2(this->rng) + 1;

		BlastParticle p(speed, pangle, radius, maxlife);
		p.setPosition(player.getPosition());
		p.setFillColor(player.getColor());

		this->particles.push_back(p);
	}
}

void BlastGenerator::update(const sf::Time& dt) {
	this->time += dt;
	for (BlastParticle& p : this->particles) {
		p.update(dt);
	}
}

void BlastGenerator::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (const BlastParticle& p : this->particles) {
		if (p.isAlive()) {
			target.draw(p, states);
		}
	}
}

//==============================================================================

Player::Player() {
	this->rng.seed(std::random_device()());
	this->dist = std::uniform_real_distribution<>(0, 1.0);

	float startx = this->dist(this->rng) * 800;
	float starty = this->dist(this->rng) * 600;
	float angle =  this->dist(this->rng) * 360;
	this->setStartingPoint(startx, starty, angle);
}

Player::~Player() {

}

bool Player::collides(const sf::Vector2f pos1, int radius1, sf::Vector2f pos2, int radius2) const {
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	float dist = std::sqrt(dx * dx + dy * dy);
	return dist < (radius1 + radius2);
}

const sf::Vector2f& Player::getPosition() const {
	return this->pos;
}

float Player::getAngle() const {
	return this->angle;
}

void Player::setStartingPoint(int x, int y, float angle) {
	this->pos.x = x;
	this->pos.y = y;
	this->angle = angle;

}

void Player::setColor(const sf::Color& color) {
	this->color = color;
}

const sf::Color& Player::getColor() const {
	return this->color;
}

bool Player::isCollidingWithSelf() const {
	// Do not check for all previous positions or we would collide with
	// ourselves right away. Instead. we iterate from start to end - 10.
	std::vector<sf::Vector2f>::size_type tosubtract = 10;
	if (this->positions.size() < tosubtract) {
		tosubtract = this->positions.size();
	}

	for (auto it = this->positions.begin(); it != this->positions.end() - tosubtract; it++) {
		if (this->collides(this->pos, this->radius, it->getPosition(), this->radius)) {
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
	this->t += dt;
	this->totalTime += dt;
	this->emplacementCounter += dt;

	if (this->dead) {
		// only update the death anim when dead.
		blastGenerator.update(dt);
		return;
	}

	if (this->moveLeft) {
		this->angle -= 240 * dt.asSeconds();
	}
	if (this->moveRight) {
		this->angle += 240 * dt.asSeconds();
	}

	float bleh = M_PI / 180.0f;

	float speed = 200;
	float dx = std::cos(this->angle * bleh) * speed * dt.asSeconds();
	float dy = std::sin(this->angle * bleh) * speed * dt.asSeconds();

	this->pos.x += dx;
	this->pos.y += dy;

	// If we're moving left/right and moving out of bounds...
	if (dx < 0 && this->pos.x < -this->radius) {
		this->pos.x = 800;
	}
	if (dx > 0 && this->pos.x + this->radius > 800) {
		this->pos.x = -this->radius;
	}
	// If we're moving up/down and going out of bounds of the screen...
	if (dy < 0 && this->pos.y < -this->radius) {
		this->pos.y = 600;
	}
	if (dy > 0 && this->pos.y + this->radius > 600) {
		this->pos.y = -this->radius;
	}

	// TODO: the threshold doesn't exactly work like we want to if the dx and
	// dy is too large. We will see gaps instead. We probably need to calculate
	// the diff between the previous and new position, and see if we need to add
	// extra circles etc.
	if (this->emplacementCounter > sf::milliseconds(10) && this->totalTime > this->startAfter) {
		this->drawArrow = false;

		if (this->t < sf::milliseconds(2000)) {
			// As a poor man's optimalization technique, I prematurely create the
			// circleshapes to draw instead of adding the pure positions. This prevents
			// a lot of constructing in the draw() method.
			sf::CircleShape shape;

			shape.setFillColor(this->color);
			shape.setPosition(this->pos);
			shape.setRadius(this->radius);
			this->positions.push_back(shape);
		} else if (this->t > sf::milliseconds(2200)) {
			this->t = sf::Time::Zero;
		}

		this->emplacementCounter = sf::Time::Zero;
	}

	this->dead = false;
	if (this->isCollidingWithSelf()) {
		this->dead = true;
		blastGenerator.init(*this);
	}

	if (this->drawArrow) {
		this->arrow.update(dt);
		this->arrow.setPosition(pos.x + this->radius, pos.y + this->radius);
		this->arrow.setRotation(this->angle);
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Draw the path of the player
	for (const sf::CircleShape& p : this->positions) {
		target.draw(p, states);
	}

	if (this->dead) {
		target.draw(blastGenerator, states);
	}

	sf::CircleShape shape;
	shape.setRadius(this->radius);
	shape.setPosition(this->pos);
	shape.setFillColor(this->color);
	target.draw(shape, states);

	// draw direction arrow
	if (this->drawArrow) {
		target.draw(arrow, states);
	}
}

const std::string Player::str() const {
	float anglerads = this->angle * M_PI / 180.0f;
	std::stringstream ss;
	ss << "Player at " << this->pos.x << ", " << this->pos.y << std::endl;
	ss << "Angle: " << this->angle << " deg, " << anglerads << " rad" << std::endl;
	ss << "  dx: " << std::cos(anglerads) << std::endl;
	ss << "  dy: " << std::sin(anglerads) << std::endl;
	return ss.str();
}