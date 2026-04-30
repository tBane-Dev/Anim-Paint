#include "Tools/Line.hpp"
#include "Window.hpp"
#include "WorldToTileConverter.hpp"
#include "Components/Canvas.hpp"
#include "Cursor.hpp"
#include "DebugLog.hpp"
#include "Components/Toolbar/Toolbar.hpp"
#include "Tools/Brush.hpp"
#include "Tools/Filters.hpp"
#include "Dialogs/Dialog.hpp"

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

    Line::Line(sf::Vector2i start, sf::Vector2i end) {
        set(start, end);
    }

    void Line::set(sf::Vector2i start, sf::Vector2i end) {
        _start = start;
        _end = end;
        updateImage();
    }

    void Line::setFillColor(sf::Color color) {
        _color = color;
        updateImage();
    }

    void Line::setPixelSize(float pixelSize) {
        _pixelSize = pixelSize;
    }

    void Line::setGlobalPosition(sf::Vector2i pos) {
        _globalPosition = pos;
    }

    sf::Vector2i Line::getSize() {
        return sf::Vector2i(_image.getSize());
    }

    sf::Image& Line::getImage() {
        return _image;
    }

    sf::Vector2i Line::getLocalPosition() {
        return _position;
    }


    void Line::drawPixels() {
        
        sf::Vector2i localStart(
            _start.x - _position.x,
            _start.y - _position.y
        );

        sf::Vector2i localEnd(
            _end.x - _position.x,
            _end.y - _position.y
        );

        std::vector<sf::Vector2i> points = getPointsFromLine(localStart, localEnd);

        for (const auto& p : points) {
            if (p.x < 0 || p.y < 0)
                continue;

            if (p.x >= _image.getSize().x)
                continue;

            if (p.y >= _image.getSize().y)
                continue;

            _image.setPixel(sf::Vector2u(p.x, p.y), _color);
        }
    }

    void Line::updateImage() {
        int minX = std::min(_start.x, _end.x);
        int minY = std::min(_start.y, _end.y);
        int maxX = std::max(_start.x, _end.x);
        int maxY = std::max(_start.y, _end.y);

        int margin = brush->_size;

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
            _start.x - _position.x,
            _start.y - _position.y
        );

        sf::Vector2i localEnd(
            _end.x - _position.x,
            _end.y - _position.y
        );

        drawPixels();

		set_outline(_image, brush->_size, sf::Color::Transparent, _color);

        _texture.loadFromImage(_image);
        _texture.setSmooth(false);

    }

    void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::Sprite sprite(_texture);

        sprite.setPosition(
            sf::Vector2f(_globalPosition) + sf::Vector2f(_position) * _pixelSize
        );

        sprite.setScale({ _pixelSize, _pixelSize });

        target.draw(sprite, states);
    }

}

Line::Line() : ResizableTool() {
    _state = ResizableToolState::None;
    _points.clear();
}

Line::~Line() {

}

void Line::generateEdgePoints() {

    if (_points.size() < 2)
        return;

    _edgePoints.clear();

    if (_points[0] == _points[1])
        return;

    float scale = canvas->_zoom * canvas->_zoom_delta;

	sf::Vector2f sp = sf::Vector2f(_points[0].x + 0.5f, _points[0].y + 0.5f) * scale;
	sf::Vector2f ep = sf::Vector2f(_points[1].x + 0.5f, _points[1].y + 0.5f) * scale;

	sf::Vector2f startPos = sf::Vector2f(canvas->_position) + sp;
	sf::Vector2f endPos = sf::Vector2f(canvas->_position) + ep;

	_startPoint = std::make_shared<EdgePoint>(sf::Vector2i(startPos));
	_endPoint = std::make_shared<EdgePoint>(sf::Vector2i(endPos));

	
	_edgePoints.push_back(_startPoint);
	_edgePoints.push_back(_endPoint);

    //DebugLog(L"s: " + std::to_wstring(_startPoint->getPosition().x) + L" " + std::to_wstring(_startPoint->getPosition().y));
    //DebugLog(L"e: " + std::to_wstring(_endPoint->getPosition().x) + L" " + std::to_wstring(_endPoint->getPosition().y));

}

void Line::setPosition(sf::Vector2i position) {

}

void Line::resize() {

    // centering the Edge Point
    sf::Vector2i curPos;
    curPos.x = cursor->_position.x;
    curPos.y = cursor->_position.y;
    sf::Vector2i tilePos = worldToTile(curPos, canvas->_position, canvas->_zoom, canvas->_zoom_delta);

    sf::Vector2i tileGlobalPos;
    tileGlobalPos.x = (float)canvas->_position.x + (float)(tilePos.x + 0.5f) * canvas->_zoom * canvas->_zoom_delta;
    tileGlobalPos.y = (float)canvas->_position.y + (float)(tilePos.y + 0.5f) * canvas->_zoom * canvas->_zoom_delta;

    // set centering Position
    if (_clickedEdgePoint == _startPoint) {
        _startPoint->setPosition(tileGlobalPos);
    }
    else if (_clickedEdgePoint == _endPoint) {
        _endPoint->setPosition(tileGlobalPos);
    }

    _points[0] = worldToTile(_startPoint->getPosition(), canvas->_position, canvas->_zoom, canvas->_zoom_delta);
    _points[1] = worldToTile(_endPoint->getPosition(), canvas->_position, canvas->_zoom, canvas->_zoom_delta);
}

void Line::generateImage()
{
    if (_points.size() < 2)
        return;

    if (_points[0] == _points[1])
        return;

    sf::Line line(_points[0], _points[1]);
    line.setFillColor(toolbar->_first_color->_color);

    _image = std::make_shared<sf::Image>(line.getImage());

    _rect.position = line.getLocalPosition();
    _rect.size = sf::Vector2i(_image->getSize());
}


void Line::drawEdgePoints() {

    if (!(_points.size() >= 2 && (_state != ResizableToolState::None)))
        return;

    for (auto& point : _edgePoints) {
        point->draw();
    }
}


void Line::cursorHover() {

    if (!dialogs.empty()) {
        return;
    }

    if (_state == ResizableToolState::None)
        return;

    sf::Vector2i tile = worldToTile(cursor->_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta);

    if ((Element_pressed.get() == this && (_state == ResizableToolState::Selecting || _state == ResizableToolState::Resizing))) {
        Element_hovered = this->shared_from_this();
    }

    if (_clickedEdgePoint != nullptr) {
        Element_hovered = _clickedEdgePoint;
        _hoveredEdgePoint = _clickedEdgePoint;
        return;
    }

    for (auto& edgePoint : _edgePoints) {
        edgePoint->cursorHover();
        if (Element_hovered == edgePoint) {
            _hoveredEdgePoint = edgePoint;
            return;
        }
    }
}

void Line::handleEvent(const sf::Event& event) {

    // shapes resizing
    if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
        if (_state == ResizableToolState::Selected && _hoveredEdgePoint != nullptr && Element_hovered == _hoveredEdgePoint) {
            _clickedEdgePoint = _hoveredEdgePoint;
            Element_pressed = _clickedEdgePoint;
			_orginalEdgePointPosition = _clickedEdgePoint->getPosition();
            _lastTilePosition = worldToTile(cursor->_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta);
            _state = ResizableToolState::Resizing;
            return;
        }
    }
    else if (_state == ResizableToolState::Resizing) {
        if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
            if (Element_pressed == _clickedEdgePoint)
                Element_pressed = nullptr;
            _clickedEdgePoint = nullptr;
            _state = ResizableToolState::Selected;
        }
        return;
    }

    if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
        
        if ((Element_hovered == nullptr || Element_pressed.get() == this || canvasIsHovered() || canvasIsPressed()) && (_state == ResizableToolState::None || _state == ResizableToolState::Selected)) {
            
            //sf::Vector2i tile = worldToTile(cursor->_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta);
            //_points.clear();
            //_points.push_back(tile);
            //_points.push_back(tile);
            //_state = ResizableToolState::Selecting;
            //generateRect();
            //generateImage();
            //generateEdgePoints();
            //generatePreviewImage();
            //_lastTilePosition = tile;

            if (toolbar->_toolType == ToolType::Line) {
                if (canvasIsHovered()) {
                    if (_image != nullptr) {
                        pasteToCanvas();
                        _image = nullptr;
                        
                    }

                    _state = ResizableToolState::Selecting;
                    sf::Vector2i tile = worldToTile(cursor->_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta);
                    _lastTilePosition = tile;
                    Element_pressed = this->shared_from_this();
                    _points.clear();
                    _points.push_back(tile);
                    _points.push_back(tile);
                    generateRect();
                    generateImage();
                    generateEdgePoints();
                    generatePreviewImage();
                }
                else {
                    if (_image != nullptr) {
                        pasteToCanvas();
                        _image = nullptr;
                        _points.clear();
                        generateRect();
                        generateImage();
                        generatePreviewImage();
                        _state = ResizableToolState::None;
                    }
                }
            }
        }

    }
    else if(const auto* mm = event.getIf<sf::Event::MouseMoved>(); mm) {
        if (_state == ResizableToolState::Selecting) {
            sf::Vector2i tile = worldToTile(cursor->_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta);

            if (tile != _lastTilePosition) {
                _points[1] = tile;
                generateRect();
                generateImage();
                generateEdgePoints();
                generatePreviewImage();
				_lastTilePosition = tile;
           }
            
        }
	}
    else if(const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
        if (_state == ResizableToolState::Selecting) {
            _points[1] = worldToTile(cursor->_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta);
            _state = ResizableToolState::Selected;
            generateRect();
            generateImage();
            generateEdgePoints();
            generatePreviewImage();
        }
	}
}

void Line::update() {

    if (_state == ResizableToolState::Resizing) {
        for (auto& point : _edgePoints) {
            point->update();
        }

        resize();
        generateImage();
        generatePreviewImage();
    }
}

void Line::draw() {
    
	ResizableTool::draw();
}