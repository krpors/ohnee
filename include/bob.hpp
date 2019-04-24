#ifndef BOB_HPP
#define BOB_HPP

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "text.hpp"


class BobbingText : public Text {
public:

	BobbingText();
	BobbingText(const std::shared_ptr<ImageFont>& font);
	~BobbingText();

	void setFunction(const std::function< float(const sf::Time& time) >& f);

	void handleInput(const sf::Event& event);
	void update(const sf::Time& dt);

private:
	sf::Time timer;

	/**
	 * Sets the function to be invoked for bobbing the text. The function can
	 * return anything, which will be input for the scaling factor.
	 */
	std::function<float(const sf::Time& t)> function;
};


#endif // BOB_HPP
