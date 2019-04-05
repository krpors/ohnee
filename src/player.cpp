#include <cmath>
#include <iostream>
#include <sstream>

#include "player.hpp"
#include "util.hpp"

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

// =============================================================================

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

	sf::Vector2f newpos = sf::CircleShape::getPosition();
	if (newpos.x < 0 && dx < 0) {
		newpos.x = 1024;
	} else if (newpos.x > 1024 && dx > 0) {
		newpos.x = 0;
	} else if (newpos.y < 0 && dy < 0) {
		newpos.y = 768;
	} else if (newpos.y > 768 && dy > 0) {
		newpos.y = 0;
	} else {
		newpos.x += dx;
		newpos.y += dy;
	}

	sf::CircleShape::setPosition(newpos.x, newpos.y);
	sf::CircleShape::setRadius(this->maxradius * percentageLife);
}

// =============================================================================

BlastGenerator::BlastGenerator() {
}

BlastGenerator::~BlastGenerator() {
}

void BlastGenerator::init(const Player& player) {
	this->particles.clear();
	for (int i = 0; i < 80; i++) {
		sf::Time maxlife = sf::milliseconds(Rng::distPos() * 2000);
		// float speed = 200.0f * this->dist2(this->rng);
		float speed = 200.0f * Rng::distPos();
		// float pangle = this->dist(this->rng) * 20 + player.getAngle();
		float pangle = Rng::distNeg() * 20 + player.getAngle();
		float radius = 5 * Rng::distPos() + 1;

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

// =============================================================================

Player::Player() {
	this->reset();
}

Player::~Player() {

}

bool Player::collides(const sf::Vector2f pos1, int radius1, sf::Vector2f pos2, int radius2) const {
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	float dist = std::sqrt(dx * dx + dy * dy);
	return dist < (radius1 + radius2);
}

void Player::reset() {
	this->dead = false;
	this->emplacementCounter = sf::Time::Zero;
	this->positions.clear();
	this->drawArrow = true;
	this->totalTime = sf::Time::Zero;

	float startx = Rng::distPos() * 1028;
	float starty = Rng::distPos() * 768;
	float angle =  Rng::distPos() * 360;
	this->setStartingPoint(startx, starty, angle);
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

void Player::unmove() {
	this->moveLeft = false;
	this->moveRight = false;
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
	return false;
}

void Player::handleInput(const sf::Event& event) {
	this->moveLeft = false;
	this->moveRight = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->moveLeft = true;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->moveRight =true;
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
		this->pos.x = 1024;
	}
	if (dx > 0 && this->pos.x + this->radius > 1024) {
		this->pos.x = -this->radius;
	}
	// If we're moving up/down and going out of bounds of the screen...
	if (dy < 0 && this->pos.y < -this->radius) {
		this->pos.y = 768;
	}
	if (dy > 0 && this->pos.y + this->radius > 768) {
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
