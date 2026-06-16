#pragma once
#include "Element.hpp"
#include "Dialogs/FileDialog/FileFunctions.hpp"
#include <functional>
#include "Controls/Button.hpp"

class Location;

enum class LocationRectClickType { None, ClickArrow, ClickLocation };

class LocationRect : public Element {
public:

	const float _arrowMargin = 4;
	const float _indentDelta = 12;

	std::shared_ptr<Location> _location;

	sf::IntRect _rect;
	std::unique_ptr<sf::Text> _text;

	ButtonState _state;
	LocationRectClickType _clickType;

	std::function<void()> _onclick_arrow_func;
	std::function<void()> _onclick_location_func;

	sf::Time _clickTime;

	LocationRect();
	~LocationRect();

	void setLocation(std::shared_ptr<Location> location);

	void setSize(sf::Vector2i size);
	void setPosition(sf::Vector2i position);
	void setText(std::wstring text);

	void unclick();
	void hover();
	void locationClick();
	void arrowClick();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};