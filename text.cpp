#include <iostream>
#include <map>
#include <memory>
#include <sstream>

#include "text.hpp"

ImageFont::ImageFont() {
}

ImageFont::ImageFont(const std::string& file, const std::string& glyphs) {
	this->load(file, glyphs);
}

ImageFont::~ImageFont() {
	std::clog << "Imagefont is destroyed" << std::endl;
}

void ImageFont::load(const std::string& file, const std::string& glyphs) {
	this->filename = file;
	this->glyphs = glyphs;

	if (!this->image.loadFromFile(file)) {
		throw "Unable to open file '" + file + "'";
	}

	// Get the separator color of the image, which is the topleft pixel of the image.
	// Make those pixels as a mask. We use the mask 'color' later on to separate the
	// glyphs.
	sf::Color sepColor = image.getPixel(0, 0);
	this->image.createMaskFromColor(sepColor);

	if (!this->texture.loadFromImage(this->image)) {
		throw "Unable to load texture from image object";
	}

	texture.setSmooth(false);

	std::clog << "ImageFont created with image file " << file << std::endl;

	// Reget the separation color. This is now a masked color.
	sepColor = image.getPixel(0, 0);

	// The vector containing rectangle for all the glyphs.
	std::vector<sf::IntRect> glyphRects;

	bool readGlyph = false;
	unsigned int glyphWidth = 0;
	unsigned glyphCount = 0;

	// Start iterating of the x-axis of the image to find the individual glyphs.
	const sf::Vector2u imageSize = image.getSize();
	for (unsigned int x = 0; x < imageSize.x; x++) {
		sf::Color currentColor = image.getPixel(x, 0);
		if (currentColor == sepColor) {
			if (readGlyph) {
				// We found a (next) separator, so we can read the glyph fully
				// by using the glyph width.
				glyphRects.emplace_back(x - glyphWidth, 0, glyphWidth, imageSize.y);
				glyphCount++;
			}

			readGlyph = false;
			glyphWidth = 0;
		} else {
			// We are not reading a separator, so increase the glyph width,
			// until we hit the next separator color.
			readGlyph = true;
			glyphWidth++;
		}
	}

	if (glyphCount != this->glyphs.size()) {
		std::stringstream ss;
		ss << "Glyph count " << glyphCount << " does not equal character count " << this->glyphs.size();
		std::cerr << ss.str() << std::endl;
		throw ss.str();
	}

	int counter = 0;
	for (auto& rect : glyphRects) {
		// map the current character in the 'glyphs' string to the rectangle.
		this->glyphMap[this->glyphs[counter++]] = rect;
	}

	std::clog << "Initialized ImageFont with " << glyphCount << " glyphs" << std::endl;
}

int ImageFont::getFontHeight() const {
	return this->image.getSize().y;
}

const std::map<char, sf::IntRect> ImageFont::getGlyphMap() const {
	return this->glyphMap;
}

const sf::Texture& ImageFont::getTexture() const {
	return this->texture;
}

const std::string& ImageFont::getCharacters() const {
	return this->glyphs;
}

int ImageFont::getKerning() const {
	return this->kerning;
}

void ImageFont::setKerning(int kerning) {
	this->kerning = kerning;
}

// =============================================================================
// Text class.
// =============================================================================

Text::Text() {
}

Text::Text(const std::shared_ptr<ImageFont>& font) {
	this->setFont(font);
}

Text::~Text() {
	std::clog << "Text is destroyed" << std::endl;
}

void Text::setFont(const std::shared_ptr<ImageFont>& font) {
	this->font = font;
}

void Text::setText(float x, float y, const std::string& str) {
	setPosition(x, y);
	this->text = str;

	this->varray.clear();
	this->varray.setPrimitiveType(sf::Quads);
	sf::Vector2f position = getPosition();
	int kerning = this->font->getKerning();
	const std::map<char, sf::IntRect>& map = this->font->getGlyphMap();
	for (const char& c : this->text) {
		if (c == '\n') {
			position.x = getPosition().x;
			position.y += this->font->getFontHeight();
			continue;
		}

		auto search = map.find(c);
		if (search != map.end()) {
			sf::IntRect bleh = search->second;
			sf::FloatRect texrect;
			texrect.left   = static_cast<float>(bleh.left);
			texrect.top    = static_cast<float>(bleh.top);
			texrect.width  = static_cast<float>(bleh.width);
			texrect.height = static_cast<float>(bleh.height);

			sf::Vertex topleft({ position.x, position.y}, {texrect.left, texrect.top});
			sf::Vertex topright({ position.x + texrect.width, position.y }, { texrect.left + texrect.width, texrect.top});
			sf::Vertex bottomright( { position.x + texrect.width, position.y + texrect.height}, {texrect.left + texrect.width, texrect.top + texrect.height});
			sf::Vertex bottomleft( { position.x, position.y + texrect.height}, {texrect.left, texrect.top + texrect.height});
			this->varray.append(topleft);
			this->varray.append(topright);
			this->varray.append(bottomright);
			this->varray.append(bottomleft);

			position.x += search->second.width + kerning;
 		}
	}
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = &font->getTexture();
	target.draw(this->varray, states);
}