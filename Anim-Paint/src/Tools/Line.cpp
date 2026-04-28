#include "Tools/Line.hpp"
#include <iostream>

std::vector<sf::Vector2i> getPointsFromLine(sf::Vector2i start, sf::Vector2i end) {
    std::vector<sf::Vector2i> points;

    int x0 = start.x;
    int y0 = start.y;
    int x1 = end.x;
    int y1 = end.y;

    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        points.push_back({ x0, y0 });
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }

    return points;
}

namespace sf {

    Line::Line(sf::Vector2i start, sf::Vector2i end, int thickness, int outline) {
        set(start, end, thickness, outline);
    }

    void Line::set(sf::Vector2i start, sf::Vector2i end, int thickness, int outline) {
        _start = start;
        _end = end;
        _thickness = thickness;
        _outlineThickness = outline;

        updateImage();
    }

    void Line::set(sf::Vector2i start, sf::Vector2i end) {
        _start = start;
        _end = end;

        updateImage();
    }

    void Line::setThickness(int thickness) {
        _thickness = thickness;
        updateImage();
    }

    void Line::setOutlineThickness(int outlineThickness) {
        _outlineThickness = outlineThickness;
        updateImage();
    }

    void Line::setFillColor(sf::Color color) {
        _fillColor = color;
        updateImage();
    }

    void Line::setOutlineColor(sf::Color color) {
        _outlineColor = color;
        updateImage();
    }

    void Line::setPixelSize(float pixelSize) {
        _pixelSize = pixelSize;
    }

    void Line::setGlobalPosition(sf::Vector2i pos) {
        _globalPosition = pos;
    }

    void Line::drawPixel(sf::Image& img, int x, int y, sf::Color color) {
        if (x < 0 || y < 0)
            return;

        if (x >= img.getSize().x || y >= img.getSize().y)
            return; 

        img.setPixel(sf::Vector2u(x, y), color);
    }

    void Line::drawCircle(sf::Image& img, int cx, int cy, int radius, sf::Color color) {
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                if (x * x + y * y < radius * radius) {
                    drawPixel(img, cx + x, cy + y, color);
                }
            }
        }
    }

    void Line::drawLinePixels(sf::Image& img, sf::Vector2i a, sf::Vector2i b, int radius, sf::Color color) {
        int x0 = a.x;
        int y0 = a.y;
        int x1 = b.x;
        int y1 = b.y;

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);

        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;

        int err = dx - dy;

        while (true) {
            drawCircle(img, x0, y0, radius, color);

            if (x0 == x1 && y0 == y1)
                break;

            int e2 = err * 2;

            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }

            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

    void Line::updateImage() {
        sf::Vector2i start = _start;
        sf::Vector2i end = _end;

        float totalThickness = _thickness + _outlineThickness * 2.f;
        int radiusOut = std::ceil(totalThickness / 2.f);
        int radiusIn = std::ceil(_thickness / 2.f);

        int margin = radiusOut + 2;

        int minX = std::min(start.x, end.x);
        int minY = std::min(start.y, end.y);
        int maxX = std::max(start.x, end.x);
        int maxY = std::max(start.y, end.y);

        _position = {
            minX - margin,
            minY - margin
        };

        int width = (maxX - minX) + margin * 2 + 1;
        int height = (maxY - minY) + margin * 2 + 1;

        if (width <= 0 || height <= 0)
            return;

        _image.resize(sf::Vector2u(width, height), sf::Color::Transparent);

        sf::Vector2i localStart(
            std::round(start.x - _position.x),
            std::round(start.y - _position.y)
        );

        sf::Vector2i localEnd(
            std::round(end.x - _position.x),
            std::round(end.y - _position.y)
        );

        if (_outlineThickness > 0) {
            drawLinePixels(_image, localStart, localEnd, radiusOut, _outlineColor);
        }

        drawLinePixels(_image, localStart, localEnd, radiusIn, _fillColor);

        _texture.loadFromImage(_image);
        _texture.setSmooth(false);
    }

    void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::Sprite sprite(_texture);
        sprite.setPosition(sf::Vector2f(_globalPosition) + sf::Vector2f(_position) * _pixelSize);
        sprite.setScale({ _pixelSize, _pixelSize });
        target.draw(sprite, states);
    }

}