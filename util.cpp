#include "util.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <random>

FpsCounter::FpsCounter() :
		timer(sf::Time::Zero),
		fps(0),
		frameCounter(0) {
}

FpsCounter::~FpsCounter() {
}

void FpsCounter::update(const sf::Time& delta) {
	this->timer += delta;

	if (this->timer > sf::seconds(1)) {
		this->timer = sf::Time::Zero;
		this->fps = frameCounter;
		this->frameCounter = 0;
	} else {
		this->frameCounter++;
	}
}

unsigned int FpsCounter::getFps() const {
	return this->fps;
}

//==============================================================================

Particle::Particle(float maxLife) :
	maximumLife(maxLife) {
}

Particle::~Particle() {
}

//==============================================================================

ParticleGenerator::ParticleGenerator() {
	rng.seed(std::random_device()());
	dist = std::uniform_real_distribution<>(-10, 10);

	std::uniform_real_distribution<> dist2(100, 8000);

	for (int i = 0; i < 100; i++) {
		Particle p(dist2(this->rng));
		this->resetParticle(p);
		this->particles.push_back(p);
	}
}

ParticleGenerator::~ParticleGenerator() {
}

void ParticleGenerator::resetParticle(Particle& p) {
		p.pos.x = 200;
		p.pos.y = 200;
		p.dir.x = this->dist(this->rng);
		p.dir.y = this->dist(this->rng);
		p.currentLife = p.maximumLife;
		p.color = sf::Color(255, 255, 255);
		// std::cout << "curentlife is set to " << p.currentLife << std::endl;
}

void ParticleGenerator::update(const sf::Time& dt) {
	float lol = dt.asSeconds();
	for (auto &p : this->particles) {
		p.pos.x += p.dir.x * lol;
		p.pos.y += p.dir.y * lol;

		p.currentLife -= dt.asSeconds() * 1000;
		if (p.currentLife <= 0) {
			this->resetParticle(p);
		}
	}
}

void ParticleGenerator::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::VertexArray arr(sf::Points);
	for (const auto& p : this->particles) {
		sf::Vertex vertex;
		vertex.position = { p.pos.x, p.pos.y };
		vertex.color = p.color;
		arr.append(vertex);
	}

	target.draw(arr);
}
