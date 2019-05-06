#ifndef UI_HPP
#define UI_HPP

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "text.hpp"


class Button : public sf::Drawable, public sf::Transformable {
public:
	Button(const std::shared_ptr<ImageFont>& font);

	bool isSelected() const;

	void setSelected(bool selected);

	void setCallback(std::function<void()> callback);

	void activate() const;

	void setText(const std::string& txt);

	virtual void update(const sf::Time& dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	Text text;

	std::string buttonText;

	sf::Time time;

	bool selected;

	/**
	 * Struct holding button graphic properties things.
	 */
	struct {
		sf::Color selectedColor = sf::Color::Green;
		float thickness = 1;
	} gfx;
private:

	std::shared_ptr<ImageFont> font;

	float width = 200;

	float height = 25;

	std::function<void()> callback;
};

// =============================================================================

class Container : public sf::Drawable {
public:
	Container();

	void addButton(const std::shared_ptr<Button>& button);

	void activate() const;
	void selectPrevious();
	void selectNext();

	void update(const sf::Time& delta);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<std::shared_ptr<Button>> vecButtons;

	void select(int btnIndex);

	int selectedIndex;
};


#endif // UI_HPP
