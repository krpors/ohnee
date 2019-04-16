#ifndef BOB_HPP
#define BOB_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "text.hpp"


class BobbingText : public Text {
private:
	float timer = 0.0f;
public:

	BobbingText();
	BobbingText(const std::shared_ptr<ImageFont>& font);
	~BobbingText();

	void handleInput(const sf::Event& event);
	void update(const sf::Time& dt);
};


#endif // BOB_HPP
