#include <memory>

#include <SFML/Graphics.hpp>

#include "text.hpp"

class BobbingText : public sf::Drawable {
private:
	float timer = 0.0f;
	std::shared_ptr<ImageFont> font;
	Text text;
public:
	BobbingText(const std::shared_ptr<ImageFont>& font);
	~BobbingText();

	void update(const sf::Time& dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};