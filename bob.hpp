#include <memory>

#include <SFML/Graphics.hpp>

#include "text.hpp"

class BobbingText : public sf::Drawable {
private:
	std::shared_ptr<ImageFont> font;
	Text text;
public:
	BobbingText(const std::shared_ptr<ImageFont>& font);
	~BobbingText();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};