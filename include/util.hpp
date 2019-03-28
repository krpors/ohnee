#include <cfloat>
#include <random>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#ifndef UTIL_HPP
#define UTIL_HPP

/*
 * This part defines a macro so we can print debugging lines, but only when
 * the NDEBUG macro is undefined (much like from <assert.h>. The output is
 * colored for now, and can be controlled via the `filename' and `function'
 * #defines.
 *
 * The macro debug_print only has an implementation when the project is built
 * using the `debug' target.
 */
#ifndef NDEBUG
#define filename "\x1b[34;1m%s\x1b[0m"
#define function "\x1b[33;1m%s()\x1b[0m"
#define debug_print(fmt, ...) \
	do { fprintf(stderr, filename ":%d:" function "    " fmt "\n", __FILE__, __LINE__, __func__, ## __VA_ARGS__); } while (0)
// Note: the double ## is to swallow the preceding comma in case the list is empty (gcc specific)
#else
#define debug_print(fmt, ...) (void)0;
#endif // NDEBUG


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
	// TODO: adjust to get inclusive range
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
