#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include "text.hpp"

#ifndef UI_HPP
#define UI_HPP

class Button : public sf::Drawable, public sf::Transformable {
private:
    std::shared_ptr<ImageFont> font;

    std::function<void()> callback;
public:
    Button();
    Button(const std::shared_ptr<ImageFont> font);

    void setCallback(std::function<void()> callback);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // UI_HPP
