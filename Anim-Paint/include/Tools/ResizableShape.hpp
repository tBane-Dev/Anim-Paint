#pragma once
#include "SFML/Graphics.hpp"
#include "Element.hpp"
#include "Controls/EdgePoint.hpp"
#include "Tools/ResizableTool.hpp"


void pasteImageWithAlpha(sf::Image& dst, sf::Image& src, int dstX, int dstY, sf::Color alphaColor = sf::Color::Transparent);


class ResizableShape : public ResizableTool{
public:

	// edge points
	std::shared_ptr<EdgePoint> _point_left_top;
	std::shared_ptr<EdgePoint> _point_top;
	std::shared_ptr<EdgePoint> _point_right_top;
	std::shared_ptr<EdgePoint> _point_left;
	std::shared_ptr<EdgePoint> _point_right;
	std::shared_ptr<EdgePoint> _point_left_bottom;
	std::shared_ptr<EdgePoint> _point_bottom;
	std::shared_ptr<EdgePoint> _point_right_bottom;
	

	ResizableShape();
	~ResizableShape();

	virtual void addPoint(sf::Vector2i point);
	virtual bool pointOnSegment(sf::Vector2i p, sf::Vector2i a, sf::Vector2i b);
	virtual bool isPointInPolygon(sf::Vector2i p, std::vector<sf::Vector2i>& poly);
	virtual void reset();
	virtual void generateEdgePoints();
	virtual void resizeRect();
	bool clickOnSelection(sf::Vector2i point);
	void scale(sf::IntRect newRect);
	virtual void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};