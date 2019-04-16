#include <iostream>

#include <SFML/Graphics.hpp>

#include "util.hpp"

inline static sf::Color getPixelOrBlack(const sf::Image& source, int x, int y) {
	if (
		   x < 0
		|| x > static_cast<int>(source.getSize().x)
		|| y < 0
		|| y > static_cast<int>(source.getSize().y)) {
		return sf::Color::Black;
	}

	return source.getPixel(x, y);
}

void blurImage(const sf::Image& source, sf::Image& target) {
	// Start iterating over every pixel available.
	for (int y = 0; y < static_cast<int>(source.getSize().y); y++) {
		for (int x = 0; x < static_cast<int>(source.getSize().x); x++) {
			if (source.getPixel(x, y).a == 0) {
				continue;
			}
			// These are the target rgba values.
			int targetr = 0;
			int targetg = 0;
			int targetb = 0;
			int targeta = 0;

			// Check the surrounding pixels (around x, y) and get the color
			// values. Add them all to the targetX values. These two loops
			// will (and must) iterate 9 times exactly.
			for (int xx = x - 1; xx <= x + 1; xx++) {
				for (int yy = y - 1; yy <= y + 1; yy++) {
					sf::Color pix = getPixelOrBlack(source, xx, yy);
					targetr += pix.r;
					targetg += pix.g;
					targetb += pix.b;
					targeta += pix.a;
				}
			}

			// Average the color by dividing them by 9
			sf::Color newColor;
			newColor.r = targetr / 9;
			newColor.g = targetg / 9;
			newColor.b = targetb / 9;
			newColor.a = targeta / 9;

			// Set the target pixel in the target image to the convoluted color.
			target.setPixel(x, y, newColor);
		}
	}
}

//==============================================================================



Rng::Rng() {
	this->rng.seed(std::random_device()());
}

Rng& Rng::getInstance() {
	// Supposed to be thread safe. We don't yet use multiple threads but
	static Rng instance;
	return instance;
}

float Rng::distNeg() {
	Rng& instance = Rng::getInstance();
	return instance._distNeg(instance.rng);
}

float Rng::distPos() {
	Rng& instance = Rng::getInstance();
	return Rng::getInstance()._distPos(instance.rng);
}

//==============================================================================

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
