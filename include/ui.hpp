#ifndef UI_HPP
#define UI_HPP

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "text.hpp"


class Button : public sf::Drawable, public sf::Transformable {
public:
	Button();
	Button(const std::shared_ptr<ImageFont> font);

	bool isSelected() const;

	void setSelected(bool selected);

	void setCallback(std::function<void()> callback);

	void activate() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	bool selected;

	Text text;

	std::shared_ptr<ImageFont> font;

	std::function<void()> callback;
};

// =============================================================================

class Container : public sf::Drawable {
public:
	Container();

	void addButton(const Button& button);

	void activate() const;
	void selectPrevious();
	void selectNext();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<Button> vecButtons;

	void select(int btnIndex);

	int selectedIndex;
};


#endif // UI_HPP
