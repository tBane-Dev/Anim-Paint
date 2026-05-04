#pragma once
#include "Element.hpp"
#include "Controls/EdgePoint.hpp"

extern std::string replace_black_shader_source;

void pasteImageWithNewColorAndAlpha(sf::Image& dst, sf::Image& src, int dstX, int dstY, sf::Color newColor, sf::Color alphaColor = sf::Color::Transparent);
void pasteImageWithMask(sf::Image& dst, sf::Image& src, int dstX, int dstY, sf::Image& mask, sf::Color alphaColor = sf::Color::Transparent);
void copyImageWithAlpha(sf::Image& dst, sf::Image& src, sf::IntRect srcRect, sf::Color alphaColor = sf::Color::Transparent);
void copyImageWithMask(sf::Image* dst, sf::Image* src, int dstX, int dstY, int srcX, int srcY, sf::Image& mask, sf::Color alphaColor);
void removeImageWithMask(sf::Image& image, sf::IntRect rect, sf::Image& mask, sf::Color alphaColor = sf::Color::Transparent);


enum class ResizableToolState { None, Selecting, Selected, Moving, Resizing };

class ResizableTool : public Element {
public:

	ResizableToolState _state;

	// rect and points
	sf::IntRect _rect;
	std::vector<sf::Vector2i> _points;

	// edge points
	std::vector<std::shared_ptr<EdgePoint>> _edgePoints;
	std::shared_ptr<EdgePoint> _hoveredEdgePoint;
	std::shared_ptr<EdgePoint> _clickedEdgePoint;
	sf::Vector2i _orginalEdgePointPosition;

	// to move
	sf::Vector2i _offset;
	sf::Time _moveTime;
	sf::Vector2i _lastTilePosition;

	// image
	std::shared_ptr<sf::Image> _image;
	sf::Texture _texture;
	std::shared_ptr<sf::Image> _previewImage;
	sf::Shader _shader;

	ResizableTool();
	~ResizableTool();

	virtual void generateEdgePoints();
	bool tooltypeIsShape();
	void reset();
	virtual void setAlignCenter();
	sf::Vector2i getClampedPosition(sf::Vector2i newGlobalPosition);
	sf::Vector2i getClampedTilePosition(sf::Vector2i newPos);
	void setPosition(sf::Vector2i position);

	void generateRect();
	virtual void generateImage();
	virtual void generatePreviewImage();

	virtual void cut();
	virtual void copy();
	virtual void pasteToCanvas();

	void drawRect();
	void drawImage();
	virtual void drawPreviewImage();
	virtual void drawEdgePoints();

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};

extern std::shared_ptr<ResizableTool> resizable_tool;