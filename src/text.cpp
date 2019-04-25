#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>

#include "text.hpp"
#include "util.hpp"

ImageFont::ImageFont() {
}

ImageFont::ImageFont(const std::string& file, const std::string& glyphs) {
	this->load(file, glyphs);
}

ImageFont::~ImageFont() {
	TRACE("ImageFont " << this->filename << " destroyed");
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

	TRACE("Image loaded: " << file);

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

	this->image.createMaskFromColor(sepColor);
	if (!this->texture.loadFromImage(this->image)) {
		throw "Unable to load texture from image object";
	}

	TRACE("Initialized ImageFont " << this->filename << " with " << glyphCount << " glyphs");
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

// =============================================================================

Text::Text() {
}

Text::Text(const std::shared_ptr<ImageFont>& font) {
	this->setFont(font);
}

Text::~Text() {
	TRACE("Text is destroyed (" << this->text << ")");
}

int Text::getKerning() const {
	return this->kerning;
}

void Text::setKerning(int kerning) {
	this->kerning = kerning;
}

void Text::setFont(const std::shared_ptr<ImageFont>& font) {
	this->font = font;
}

void Text::initializeVertexArray() {
	assert(this->font != nullptr);

	this->varray.clear();
	this->varray.setPrimitiveType(sf::Quads);
	float x = 0;
	float y = 0;
	const std::map<char, sf::IntRect>& map = this->font->getGlyphMap();
	for (const char& c : this->text) {
		if (c == '\n') {
			x = 0;
			y += this->font->getFontHeight();
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

			sf::Vertex topleft({ x, y}, {texrect.left, texrect.top});
			sf::Vertex topright({ x + texrect.width, y }, { texrect.left + texrect.width, texrect.top});
			sf::Vertex bottomright( { x + texrect.width, y + texrect.height}, {texrect.left + texrect.width, texrect.top + texrect.height});
			sf::Vertex bottomleft( { x, y + texrect.height}, {texrect.left, texrect.top + texrect.height});
			this->varray.append(topleft);
			this->varray.append(topright);
			this->varray.append(bottomright);
			this->varray.append(bottomleft);

			x += search->second.width + this->kerning;
 		}
	}
}

void Text::setText(const std::string& str) {
	if (str == this->text) {
		// nothing to change.
		return;
	}
	this->text = str;
	this->initializeVertexArray();
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = &font->getTexture();
	states.transform *= getTransform();

	target.draw(this->varray, states);

#ifndef NDEBUG
	sf::CircleShape pos;
	pos.setRadius(2.0f);
	pos.setFillColor(sf::Color::Red);
	pos.setPosition(getPosition());
	target.draw(pos);
#endif
}
