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
