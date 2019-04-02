#include <cfloat>
#include <random>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#ifndef UTIL_HPP
#define UTIL_HPP

/**
 * This function is a very, very simplistic implementation of a convolution
 * matrix to blur the source image onto the target image. TODO: refactor into
 * something more useful/reusable? I.e. a custom convolution matrix?
 */
void blurImage(const sf::Image& source, sf::Image& target);

//==============================================================================

/**
 * Random number generator singleton.
 */
class Rng {
private:
	std::mt19937 rng;

	// The nextafter is used to get an inclusive range
	std::uniform_real_distribution<> _distNeg = std::uniform_real_distribution<>(-1.0, std::nextafter(1.0, DBL_MAX));
	std::uniform_real_distribution<> _distPos = std::uniform_real_distribution<>(0.0, std::nextafter(1.0, DBL_MAX));

	Rng();

	static Rng& getInstance();
public:
	/**
	 * Prevent copying and assignment to ensure only one instance.
	 */
	Rng(const Rng&);
	Rng operator=(const Rng&);

	/**
	 * Generates a random number in the range of [-1.0, 1.0] (inclusive).
	 */
	static float distNeg();

	/**
	 * Generates a random number in the range of [0.0, 1.0] (inclusive).
	 */
	static float distPos();

};

//==============================================================================

class FpsCounter {
private:
	sf::Time timer;
	unsigned int fps;
	unsigned int frameCounter;
public:
	FpsCounter();
	~FpsCounter();

	void update(const sf::Time& delta);
	unsigned int getFps() const;
};

//==============================================================================

class Particle {
public:
	sf::Vector2f pos;
	sf::Vector2f dir;
	sf::Color    color;
	float        maximumLife; //< max life
	float        currentLife; //< life in millisecs

	Particle(float maxLife);
	~Particle();
};

//==============================================================================

class ParticleGenerator : public sf::Drawable {
private:
	std::mt19937 rng;
	std::uniform_real_distribution<> dist;

	std::vector<Particle> particles;
public:
	ParticleGenerator();
	~ParticleGenerator();

	void resetParticle(Particle& p);
	void update(const sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // UTIL_HPP
