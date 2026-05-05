#pragma once
#include <SFML/Graphics.hpp>
#include "Tools/ResizableTool.hpp"

std::vector<sf::Vector2i> getPointsFromLine(sf::Vector2i start, sf::Vector2i end);
bool lineOutOfBounds(sf::Vector2i start, sf::Vector2i end, sf::Vector2i size);

namespace sf {

    class Line : public sf::Drawable {
    private:
        sf::Vector2i _start;
        sf::Vector2i _end;
        float _pixelSize = 1.f;
        sf::Vector2i _position;
        sf::Vector2i _globalPosition;
        sf::Color _color = sf::Color::Black;
        sf::Image _image;
        sf::Texture _texture;

    public:
        Line(sf::Vector2i start, sf::Vector2i end);

        void set(sf::Vector2i start, sf::Vector2i end);
        void setFillColor(sf::Color color);
        void setPixelSize(float pixelSize);
        void setGlobalPosition(sf::Vector2i pos);
        sf::Vector2i getSize();
        sf::Image& getImage();
        void updateImage();
        sf::Vector2i getLocalPosition();

    private:
        void drawPixels();
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    };
}

class Line : public ResizableTool {
public:

	std::shared_ptr<EdgePoint> _startPoint;
	std::shared_ptr<EdgePoint> _endPoint;

    Line();
    ~Line();

    void reset();
    void setAlignCenter();
    virtual void generateEdgePoints();
    void setPosition(sf::Vector2i position);
    void resize();

    virtual void cut();
	virtual void copy();

    virtual void generateImage();
	virtual void drawEdgePoints();
	void drawPreviewImage();
    
    virtual void cursorHover();
    virtual void handleEvent(const sf::Event& event);
    virtual void update();
    virtual void draw();
};