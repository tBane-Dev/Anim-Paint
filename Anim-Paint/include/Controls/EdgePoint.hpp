#pragma once
#include "Element.hpp"

enum class EdgePointType {
	LeftTop,
	Top,
	RightTop,
	Left,
	Right,
	LeftBottom,
	Bottom,
	RightBottom
};

class EdgePoint : public Element {
public:
	sf::IntRect _rect;
	EdgePointType _type;

	EdgePoint(EdgePointType type, sf::Vector2i position);
	~EdgePoint();

	void setPosition(sf::Vector2i position);
	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};