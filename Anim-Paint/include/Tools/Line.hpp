#pragma once
#include <SFML/Graphics.hpp>

std::vector<sf::Vector2i> getPointsFromLine(sf::Vector2i start, sf::Vector2i end);
bool lineOutOfBounds(sf::Vector2i start, sf::Vector2i end, sf::Vector2i size);

namespace sf {

    class Line : public sf::Drawable {
    private:
        sf::Vector2i _start;
        sf::Vector2i _end;

        int _thickness = 1;
        int _outlineThickness = 0;

        sf::Color _fillColor = sf::Color::White;
        sf::Color _outlineColor = sf::Color::Black;

        sf::Image _image;
        sf::Texture _texture;
        sf::Vector2i _position;
        sf::Vector2i _globalPosition = sf::Vector2i(0,0);

        float _pixelSize = 1.f;

    public:
        Line(sf::Vector2i start, sf::Vector2i end, int thickness = 1, int outline = 0);

        void set(sf::Vector2i start, sf::Vector2i end, int thickness, int outline = 0);
        void set(sf::Vector2i start, sf::Vector2i end);

        void setThickness(int thickness);
        void setOutlineThickness(int outlineThickness);

        void setFillColor(sf::Color color);
        void setOutlineColor(sf::Color color);

        void setPixelSize(float pixelSize);
        void setGlobalPosition(sf::Vector2i pos);

    private:
        void updateImage();

        void drawPixel(sf::Image& img, int x, int y, sf::Color color);
        void drawCircle(sf::Image& img, int cx, int cy, int radius, sf::Color color);
        void drawLinePixels(sf::Image& img, sf::Vector2i a, sf::Vector2i b, int radius, sf::Color color);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

}
