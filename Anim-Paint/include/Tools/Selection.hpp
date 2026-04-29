#pragma once
#include "SFML/Graphics.hpp"
#include "Element.hpp"
#include "Controls/EdgePoint.hpp"
#include "Tools/Rectangle.hpp"




class Selection : public ResizableShape {
public:

	// outline
	sf::RenderTexture _outlineRenderTexture;
	sf::Vector2i _outlineOffset;
	std::shared_ptr<sf::Sprite> _outlineSprite;

	// mask
	std::shared_ptr<sf::Image> _maskImage;

	// resized things
	sf::IntRect _resizedRect;
	std::shared_ptr<sf::Image> _resizedMaskImage;
	std::shared_ptr<sf::Image> _resizedImage;

	Selection();
	~Selection();

	
	void reset();
	void selectAll();
	bool clickOnSelection(sf::Vector2i point);
	void pasteToCanvas();
	void paste(sf::Image& dst, sf::Image& src, int dstX, int dstY, sf::Image& mask, sf::Color alphaColor);
	bool paste(sf::Image& canvas, sf::Color emptyColor, sf::Image image);
	void cut();
	void copy();

	void generateMask();
	void generateResizedMask();
	void resizeRect();
	void resizeImage();
	void drawImage(bool useMask = false);
	void drawResizedImage(bool useMask = false);

	void shiftOriginIfNeeded(sf::Vector2i& point);
	void addPoint(sf::Vector2i point);
	void scale(sf::IntRect newRectSize);
	bool pointOnSegment(sf::Vector2i p, sf::Vector2i a, sf::Vector2i b);
	bool isPointInPolygon(sf::Vector2i p, std::vector<sf::Vector2i>& poly);
	void generateRect();
	void generateOutline(bool selectionComplete = false);
	void generateEdgePoints();
	void setPosition(sf::Vector2i position);

	void drawRect();
	void drawOutline();
	void drawEdgePoints();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};

extern std::shared_ptr<Selection> selection;
