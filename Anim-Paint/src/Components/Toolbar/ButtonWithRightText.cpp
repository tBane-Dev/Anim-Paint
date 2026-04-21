#include "Components/Toolbar/ButtonWithRightText.hpp"
#include "Theme.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "Window.hpp"
#include "Translation.hpp"

ButtonWithRightText::ButtonWithRightText(std::wstring text, sf::Color textColor, sf::Color hoverTextColor, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : Button()
{
	_rectIdleColor = tools_button_idle_color;
	_rectHoverColor = tools_button_hover_color;
	_rectPressColor = tools_button_press_color;
	_rectSelectColor = tools_button_select_color;

	_rectBorderWidth = tools_border_width;
	_rectIdleBorderColor = tools_button_idle_border_color;
	_rectHoverBorderColor = tools_button_hover_border_color;
	_rectPressBorderColor = tools_button_press_border_color;
	_rectSelectBorderColor = tools_button_select_border_color;

	_textColor = textColor;
	_hoverTextColor = hoverTextColor;

	_texture = texture;
	_hoverTexture = hoverTexture;
	_pressTexture = pressTexture;

	_text = std::make_unique<sf::Text>(basicFont, text, 13);
	_text->setFillColor(_textColor);

	sf::Vector2i rectSize;
	rectSize.x = std::max(64, (int)(_text->getGlobalBounds().size.x + 32 + 8 + 2 * _rectBorderWidth));
	rectSize.y = 32;
	_rect = sf::IntRect(sf::Vector2i(0, 0), rectSize);

	setPosition(position);

	_state = ButtonState::Idle;

	_hover_func = { };
	_onclick_func = { };
	_clickTime = currentTime;

}

ButtonWithRightText::~ButtonWithRightText() {

}

void ButtonWithRightText::setText(std::wstring text) {
	_text->setString(text);
	sf::Vector2i rectSize;
	rectSize.x = std::max(64, (int)(_text->getGlobalBounds().size.x + 32 + 8 + 2 * _rectBorderWidth));
	rectSize.y = 32;
	_rect.size = rectSize;
	setPosition(_rect.position);
}

void ButtonWithRightText::setPosition(sf::Vector2i position) {
	_rect.position = position;
	_text->setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(32, 24 - basicFont.getLineSpacing(13)));
}

void ButtonWithRightText::unclick() {
	_state = ButtonState::Idle;
	_text->setFillColor(_textColor);
}

void ButtonWithRightText::hover() {
	_state = ButtonState::Hover;
	_text->setFillColor(_hoverTextColor);
}

void ButtonWithRightText::click() {
	_state = ButtonState::Pressed;
	_clickTime = currentTime;
	_text->setFillColor(_hoverTextColor);
}

void ButtonWithRightText::draw() {

	sf::Vector2f rectSize;
	rectSize.x = float(_rect.size.x - 2 * _rectBorderWidth);
	rectSize.y = float(_rect.size.y - 2 * _rectBorderWidth);

	sf::RectangleShape rect(rectSize);
	switch (_state) {
	case ButtonState::Pressed:
		rect.setFillColor(_rectPressColor);
		rect.setOutlineThickness((float)_rectBorderWidth);
		rect.setOutlineColor(_rectPressBorderColor);
		break;
	case ButtonState::Hover:
		rect.setFillColor(_rectHoverColor);
		rect.setOutlineThickness((float)_rectBorderWidth);
		rect.setOutlineColor(_rectHoverBorderColor);
		break;
	case ButtonState::Idle:
		if (_isSelected) {
			rect.setFillColor(_rectSelectColor);
			rect.setOutlineThickness((float)_rectBorderWidth);
			rect.setOutlineColor(_rectSelectBorderColor);
		}
		else {
			rect.setFillColor(_rectIdleColor);
			rect.setOutlineThickness((float)_rectBorderWidth);
			rect.setOutlineColor(_rectIdleBorderColor);
		};
		break;
	};

	sf::Vector2f rectPosition;
	rectPosition.x = float(_rect.position.x + _rectBorderWidth);
	rectPosition.y = float(_rect.position.y + _rectBorderWidth);
	rect.setPosition(rectPosition);
	window->draw(rect);

	std::shared_ptr<Texture> tex;
	switch (_state) {
		case ButtonState::Pressed:
			tex = _pressTexture;
			break;
		case ButtonState::Hover:
			tex = _hoverTexture;
			break;
		case ButtonState::Idle:
			tex = _texture;
			break;
		default:
			tex = _texture;
	};

	sf::Sprite sprite(*tex->_texture);
	sprite.setPosition(sf::Vector2f(_rect.position));
	window->draw(sprite);

	window->draw(*_text);
}