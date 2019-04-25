#ifndef TEXT_HPP
#define TEXT_HPP

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>


class ImageFont {
private:
	std::string filename = "";
	std::string glyphs = "";

	std::map<char, sf::IntRect> glyphMap;

	sf::Image   image;
	sf::Texture texture;
public:
	ImageFont();
	ImageFont(const std::string& filename, const std::string& glyphs);
	~ImageFont();

	void load(const std::string& file, const std::string& glyphs);
	int getFontHeight() const;
	const std::map<char, sf::IntRect> getGlyphMap() const;
	const sf::Texture& getTexture() const;

	const std::string& getCharacters() const;
};

//https://stackoverflow.com/questions/15648844/using-smart-pointers-for-class-members#15649077

/**
 * Text is a class to render text using an ImageFont to the screen.
 */
class Text : public sf::Drawable, public sf::Transformable {
private:
	std::shared_ptr<ImageFont> font;
	sf::VertexArray varray;
	int kerning = 0;

	void initializeVertexArray();

protected:
	std::string text = "";

public:
	Text();
	Text(const std::shared_ptr<ImageFont>& font);
	virtual ~Text();

	int getKerning() const;
	void setKerning(int kerning);
	void setFont(const std::shared_ptr<ImageFont>& font);
	void setText(const std::string& str);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // TEXT_HPP
