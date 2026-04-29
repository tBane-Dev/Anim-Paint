#include "Tools/ResizableTool.hpp"
#include "Element.hpp"
#include "Components/Toolbar/Toolbar.hpp"
#include "Components/Canvas.hpp"
#include "Components/MainMenu/MainMenu.hpp"
#include "WorldToTileConverter.hpp"
#include "History.hpp"
#include "DebugLog.hpp"
#include "Tools/ClipBoard.hpp"
#include "Theme.hpp"
#include "Window.hpp"

std::string replace_black_shader_source = R"(
    uniform sampler2D texture;
    uniform vec4 alphaColor;
    uniform vec4 newColor;

    void main() {
        vec2 uv = gl_TexCoord[0].xy;
		
		vec4 c = texture2D(texture, uv);

		if(c == alphaColor){
			gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
		}
		else{
			if(c == vec4(0.0, 0.0, 0.0, 1.0))
				gl_FragColor = newColor;
			else
				gl_FragColor = c;
		}
    }
)";

void pasteImageWithNewColorAndAlpha(sf::Image& dst, sf::Image& src, int dstX, int dstY, sf::Color newColor, sf::Color alphaColor)
{

	sf::Texture tex;
	tex.loadFromImage(src);

	sf::RenderTexture rtex;
	rtex.resize(src.getSize());

	sf::Shader sh;
	sh.loadFromMemory(replace_black_shader_source, sf::Shader::Type::Fragment);

	sh.setUniform("alphaColor", sf::Glsl::Vec4(alphaColor));
	sh.setUniform("newColor", sf::Glsl::Vec4(newColor));

	sf::Sprite spr(tex);

	rtex.clear(sf::Color::Transparent);
	rtex.draw(spr, &sh);
	rtex.display();

	sf::Image result = rtex.getTexture().copyToImage();

	for (int y = 0; y < (int)result.getSize().y; y++) {
		for (int x = 0; x < (int)result.getSize().x; x++) {

			int gx = wrap(dstX + x, dst.getSize().x);
			int gy = wrap(dstY + y, dst.getSize().y);

			sf::Color c = result.getPixel(sf::Vector2u(x, y));

			if (c.a == 0)
				continue;

			dst.setPixel(sf::Vector2u(gx, gy), c);
		}
	}

}

void pasteImageWithMask(sf::Image& dst, sf::Image& src, int dstX, int dstY, sf::Image& mask, sf::Color alphaColor)
{

	int dw = dst.getSize().x;
	int dh = dst.getSize().y;
	int sw = src.getSize().x;
	int sh = src.getSize().y;

	if (dw <= 0 || dh <= 0 || sw <= 0 || sh <= 0)
		return;

	sf::IntRect place(sf::Vector2i(dstX, dstY), sf::Vector2i(sw, sh));
	sf::IntRect canvas(sf::Vector2i(0, 0), sf::Vector2i(dw, dh));
	sf::IntRect clip;


	if (!place.findIntersection(canvas).has_value())
		return;

	clip = place.findIntersection(canvas).value();

	for (int y = clip.position.y; y < clip.position.y + clip.size.y; ++y) {
		for (int x = clip.position.x; x < clip.position.x + clip.size.x; ++x) {

			int sx = x - dstX;
			int sy = y - dstY;

			if (mask.getPixel(sf::Vector2u(sx, sy)) != sf::Color::Black)
				continue;

			sf::Color c = src.getPixel(sf::Vector2u(sx, sy));
			if (c == alphaColor || c.a == 0)
				continue;

			dst.setPixel(sf::Vector2u(x, y), c);
		}
	}
}


void copyImageWithAlpha(sf::Image& dst, sf::Image& src, sf::IntRect srcRect, sf::Color alphaColor)
{

	// (opcjonalnie, ale bezpiecznie) normalizacja, gdyby size był ujemny
	sf::IntRect r = srcRect;
	if (r.size.x < 0) { r.position.x += r.size.x; r.size.x = -r.size.x; }
	if (r.size.y < 0) { r.position.y += r.size.y; r.size.y = -r.size.y; }

	if (r.size.x <= 0 || r.size.y <= 0)
		return;

	sf::IntRect srcB(sf::Vector2i(0, 0), sf::Vector2i(src.getSize()));
	auto inter = r.findIntersection(srcB);
	if (!inter.has_value())
		return;

	sf::IntRect s = inter.value();
	if (s.size.x <= 0 || s.size.y <= 0)
		return;

	if ((int)(dst.getSize().x) < s.size.x || (int)(dst.getSize().y) < s.size.y)
		dst.resize(sf::Vector2u(s.size.x, s.size.y), sf::Color::Transparent);

	sf::Image img;
	img.resize(sf::Vector2u(s.size.x, s.size.y), sf::Color::Transparent);

	for (int y = 0; y < s.size.y; ++y) {
		for (int x = 0; x < s.size.x; ++x) {
			int xx = s.position.x + x;
			int yy = s.position.y + y;

			sf::Color c = src.getPixel(sf::Vector2u(xx, yy));
			if (c == sf::Color::Transparent || c == alphaColor)
				continue;

			img.setPixel(sf::Vector2u(x, y), c);
		}
	}

	sf::IntRect local(sf::Vector2i(0, 0), sf::Vector2i(s.size));
	if (!dst.copy(img, sf::Vector2u(0, 0), local, false)) {
		DebugError(L"copyImageWithAlpha: image copy failed");
		exit(0);
	}
}


void copyImageWithMask(sf::Image* dst, sf::Image* src, int dstX, int dstY, int srcX, int srcY, sf::Image& mask, sf::Color alphaColor)
{
	if (!src || !dst) return;

	int dw = (int)dst->getSize().x;
	int dh = (int)dst->getSize().y;
	int sw = (int)src->getSize().x;
	int sh = (int)src->getSize().y;

	if (dw <= 0 || dh <= 0 || sw <= 0 || sh <= 0) return;

	int mx0 = 0, my0 = 0, mw = 0, mh = 0;
	mw = (int)mask.getSize().x;
	mh = (int)mask.getSize().y;
	if (mw <= 0 || mh <= 0) return;

	if (dstX < 0) { srcX -= dstX;  mx0 -= dstX; dstX = 0; }
	if (dstY < 0) { srcY -= dstY;  my0 -= dstY; dstY = 0; }

	if (srcX < 0) { dstX -= srcX;  mx0 -= srcX; srcX = 0; }
	if (srcY < 0) { dstY -= srcY;  my0 -= srcY; srcY = 0; }

	int maxW = std::min(dw - dstX, sw - srcX);
	int maxH = std::min(dh - dstY, sh - srcY);
	maxW = std::min(maxW, mw - mx0);
	maxH = std::min(maxH, mh - my0);

	if (maxW <= 0 || maxH <= 0) return;

	for (int y = 0; y < maxH; ++y) {
		int sy = srcY + y;
		int dy = dstY + y;
		int my = my0 + y;

		for (int x = 0; x < maxW; ++x) {
			int sx = srcX + x;
			int dx = dstX + x;
			int mx = mx0 + x;

			const sf::Color m = mask.getPixel(sf::Vector2u(mx, my));
			if (m != sf::Color::Black) continue;

			const sf::Color c = src->getPixel(sf::Vector2u(sx, sy));
			if (c.a == 0 || c == alphaColor) continue;

			dst->setPixel(sf::Vector2u(dx, dy), c);
		}
	}
}

void removeImageWithMask(sf::Image& image, sf::IntRect rect, sf::Image& mask, sf::Color alphaColor)
{

	if (rect.size.x <= 0 || rect.size.y <= 0)
		return;

	if (mask.getSize().x <= 0 || mask.getSize().y <= 0)
		return;

	for (int y = 0; y < rect.size.y; ++y) {
		for (int x = 0; x < rect.size.x; ++x) {

			if (mask.getPixel(sf::Vector2u(x, y)) != sf::Color::Black)
				continue;

			int px = rect.position.x + x;
			int py = rect.position.y + y;

			if (px < 0 || py < 0 || px >= (int)(image.getSize().x) || py >= (int)(image.getSize().y))
				continue;

			image.setPixel(sf::Vector2u(px, py), alphaColor);
		}
	}
}


ResizableTool::ResizableTool() : Element(){

	// rect and points
	_rect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
	_points.clear();

	// edge points
	_edgePoints.clear();
	_hoveredEdgePoint = nullptr;
	_clickedEdgePoint = nullptr;
	_orginalEdgePointPosition = sf::Vector2i(0, 0);
}

ResizableTool::~ResizableTool() {

}

void ResizableTool::generateEdgePoints() {

}

bool ResizableTool::tooltypeIsShape() {
	if (
		toolbar->_toolType == ToolType::Circle ||
		toolbar->_toolType == ToolType::Triangle ||
		toolbar->_toolType == ToolType::Rectangle ||
		toolbar->_toolType == ToolType::Diamond ||
		toolbar->_toolType == ToolType::Pentagon ||
		toolbar->_toolType == ToolType::HexagonFlatTop ||
		toolbar->_toolType == ToolType::HexagonPointTop ||
		toolbar->_toolType == ToolType::Octagon)
	{

		return true;
	}
	else
		return false;

}

void ResizableTool::reset() {
	_state = ResizableToolState::None;
	_points.clear();
}

sf::Vector2i ResizableTool::getClampedPosition(sf::Vector2i newGlobalPosition) {
	int snapThreshold = 8;

	int bestDX = snapThreshold;
	int bestDY = snapThreshold;

	bool snapX = false;
	bool snapY = false;

	int snappedX = newGlobalPosition.x;
	int snappedY = newGlobalPosition.y;

	for (auto& canvas : canvases) {

		if (main_menu->canvas_repeating->_checkbox->_value == 0 &&
			!(canvas->_coords.x == 0 && canvas->_coords.y == 0))
			continue;

		if (main_menu->canvas_repeating->_checkbox->_value == 1 &&
			!(canvas->_coords.x == 0 || canvas->_coords.y == 0))
			continue;

		int left = canvas->_rect.position.x;
		int right = canvas->_rect.position.x + canvas->_rect.size.x;
		int top = canvas->_rect.position.y;
		int bottom = canvas->_rect.position.y + canvas->_rect.size.y;

		int dxLeft = abs(left - newGlobalPosition.x);
		if (dxLeft < bestDX) {
			bestDX = dxLeft;
			snappedX = left;
			snapX = true;
		}

		int dxRight = abs(right - newGlobalPosition.x);
		if (dxRight < bestDX) {
			bestDX = dxRight;
			snappedX = right;
			snapX = true;
		}

		int dyTop = abs(top - newGlobalPosition.y);
		if (dyTop < bestDY) {
			bestDY = dyTop;
			snappedY = top;
			snapY = true;
		}

		int dyBottom = abs(bottom - newGlobalPosition.y);
		if (dyBottom < bestDY) {
			bestDY = dyBottom;
			snappedY = bottom;
			snapY = true;
		}
	}

	sf::Vector2i finalPos = newGlobalPosition;

	if (snapX && bestDX < snapThreshold)
		finalPos.x = snappedX;

	if (snapY && bestDY < snapThreshold)
		finalPos.y = snappedY;

	int minDistance = INT_MAX;
	std::shared_ptr<Canvas> canvasToClamp = nullptr;

	for (auto& canvas : canvases) {

		if (main_menu->canvas_repeating->_checkbox->_value == 0 &&
			!(canvas->_coords.x == 0 && canvas->_coords.y == 0))
			continue;

		if (main_menu->canvas_repeating->_checkbox->_value == 1 &&
			!(canvas->_coords.x == 0 || canvas->_coords.y == 0))
			continue;

		int left = canvas->_rect.position.x;
		int right = canvas->_rect.position.x + canvas->_rect.size.x;
		int top = canvas->_rect.position.y;
		int bottom = canvas->_rect.position.y + canvas->_rect.size.y;

		// if point is inside the rectangle, distance is 0,
		// otherwise compute distance to the nearest edge along each axis
		int dx = std::max({ left - finalPos.x, 0, finalPos.x - right });
		int dy = std::max({ top - finalPos.y, 0, finalPos.y - bottom });
		int dist = std::sqrt(dx * dx + dy * dy);
		//

		if (dist < minDistance) {
			minDistance = dist;
			canvasToClamp = canvas;
		}
	}

	sf::Vector2i tile = worldToTile(finalPos, canvasToClamp->_position, canvasToClamp->_zoom, canvasToClamp->_zoom_delta);

	sf::Vector2i pos = tile - _offset;
	pos.x = std::clamp(pos.x, -_rect.size.x, canvasToClamp->_size.x);
	pos.y = std::clamp(pos.y, -_rect.size.y, canvasToClamp->_size.y);
	pos.x += canvasToClamp->_coords.x * canvasToClamp->_size.x;
	pos.y += canvasToClamp->_coords.y * canvasToClamp->_size.y;

	return pos;
}

sf::Vector2i ResizableTool::getClampedTilePosition(sf::Vector2i newPos)
{
	int minDistanceSq = INT_MAX;
	std::shared_ptr<Canvas> canvasToClamp = nullptr;

	if (canvases.empty())
		return newPos;

	for (auto& canvas : canvases) {

		if (main_menu->canvas_repeating->_checkbox->_value == 0 &&
			!(canvas->_coords.x == 0 && canvas->_coords.y == 0))
			continue;

		if (main_menu->canvas_repeating->_checkbox->_value == 1 &&
			!(canvas->_coords.x == 0 || canvas->_coords.y == 0))
			continue;

		sf::Vector2i canvasPos(
			canvas->_coords.x * canvas->_size.x,
			canvas->_coords.y * canvas->_size.y
		);

		int left = canvasPos.x;
		int right = canvasPos.x + canvas->_size.x;
		int top = canvasPos.y;
		int bottom = canvasPos.y + canvas->_size.y;

		int dx = std::max({ left - newPos.x, 0, newPos.x - right });
		int dy = std::max({ top - newPos.y, 0, newPos.y - bottom });

		int distSq = dx * dx + dy * dy;

		if (distSq < minDistanceSq) {
			minDistanceSq = distSq;
			canvasToClamp = canvas;
		}
	}

	sf::Vector2i canvasPos(
		canvasToClamp->_coords.x * canvasToClamp->_size.x,
		canvasToClamp->_coords.y * canvasToClamp->_size.y
	);

	sf::Vector2i pos = newPos;

	pos.x = std::clamp(pos.x, canvasPos.x - _rect.size.x, canvasPos.x + canvasToClamp->_size.x);
	pos.y = std::clamp(pos.y, canvasPos.y - _rect.size.y, canvasPos.y + canvasToClamp->_size.y);

	return pos;
}

void ResizableTool::setPosition(sf::Vector2i position) {
	sf::Vector2i delta = position - _rect.position;

	_rect.position = position;

	for (auto& p : _points) {
		p += delta;
	}

	float scale = canvas->_zoom * canvas->_zoom_delta;

	sf::Vector2f rectSize;
	rectSize.x = float(_rect.size.x) * scale;
	rectSize.y = float(_rect.size.y) * scale;

	sf::Vector2f rectPos;
	rectPos.x = float(canvas->_position.x) + float(_rect.position.x) * scale;
	rectPos.y = float(canvas->_position.y) + float(_rect.position.y) * scale;
}

void ResizableTool::generateRect() {

	if (_points.empty()) {
		_rect = sf::IntRect({ 0,0 }, { 0,0 });
		return;
	}

	int minX = std::numeric_limits<int>::max();
	int minY = std::numeric_limits<int>::max();
	int maxX = std::numeric_limits<int>::min();
	int maxY = std::numeric_limits<int>::min();

	for (auto& p : _points) {
		minX = std::min(minX, p.x);
		minY = std::min(minY, p.y);
		maxX = std::max(maxX, p.x);
		maxY = std::max(maxY, p.y);
	}

	_rect = sf::IntRect(
		sf::Vector2i(minX, minY),
		sf::Vector2i(maxX - minX, maxY - minY)
	);
}

void ResizableTool::generateImage() {
	if (_rect.size.x < 1 || _rect.size.y < 1)
		return;

	_image = std::make_shared<sf::Image>(sf::Vector2u(_rect.size), sf::Color::Transparent);
}

void ResizableTool::generatePreviewImage() {

	_previewImage = std::make_shared<sf::Image>();

	if (_rect.size.x < 1 || _rect.size.y < 1)
		return;

	if (_image == nullptr)
		return;

	if (_image->getSize().x < 1 || _image->getSize().y < 1)
		return;

	_previewImage->resize(sf::Vector2u(canvas->_size), sf::Color::Transparent);

	for (int y = 0; y < _image->getSize().y; y++) {
		for (int x = 0; x < _image->getSize().x; x++) {

			sf::Vector2i globalPos(_rect.position.x + x, _rect.position.y + y);

			bool insideAnyCanvas = false;

			for (auto& canvas : canvases) {

				if (main_menu->canvas_repeating->_checkbox->_value == 0 && !(canvas->_coords.x == 0 && canvas->_coords.y == 0))
					continue;

				if (main_menu->canvas_repeating->_checkbox->_value == 1 && !(canvas->_coords.x == 0 || canvas->_coords.y == 0))
					continue;

				sf::Vector2i canvasPos(canvas->_coords.x * canvas->_size.x, canvas->_coords.y * canvas->_size.y);
				sf::IntRect canvasRect(canvasPos, canvas->_size);

				if (canvasRect.contains(globalPos)) {
					insideAnyCanvas = true;
					break;
				}
			}

			if (insideAnyCanvas) {

				globalPos.x = (globalPos.x % Canvas::_size.x + Canvas::_size.x) % Canvas::_size.x;
				globalPos.y = (globalPos.y % Canvas::_size.y + Canvas::_size.y) % Canvas::_size.y;

				if (_image->getPixel(sf::Vector2u(x, y)) != sf::Color::Transparent)
					_previewImage->setPixel(sf::Vector2u(globalPos.x, globalPos.y), _image->getPixel(sf::Vector2u(x, y)));
			}
		}
	}
}

void ResizableTool::cut()
{
	if (_state != ResizableToolState::Selected)
		return;

	if (_previewImage == nullptr)
		return;

	sf::Image& canvas = getCurrentAnimation()->getCurrentLayer()->_image;
	sf::Color alphaColor = (toolbar->_option_transparency->_checkbox->_value == 0) ? sf::Color::Transparent : toolbar->_second_color->_color;

	sf::IntRect r = _rect;
	if (r.size.x < 0) { r.position.x += r.size.x; r.size.x = -r.size.x; }
	if (r.size.y < 0) { r.position.y += r.size.y; r.size.y = -r.size.y; }

	sf::IntRect canvasRect(sf::Vector2i(0, 0), sf::Vector2i(canvas.getSize()));
	auto intersection = r.findIntersection(canvasRect);
	if (!intersection.has_value())
		return;

	sf::IntRect s = intersection.value();
	if (s.size.x <= 0 || s.size.y <= 0)
		return;

	sf::Texture tex(*_previewImage);
	sf::Sprite spr(tex);
	sf::RenderTexture rtex;
	rtex.resize(_previewImage->getSize());
	rtex.clear(sf::Color::Transparent);
	_shader.setUniform("newColor", sf::Glsl::Vec4(toolbar->_first_color->_color));
	rtex.draw(spr, &_shader);
	rtex.display();
	sf::Image coloredImage = rtex.getTexture().copyToImage();

	pasteImageWithMask(canvas, coloredImage, 0, 0, *_previewImage, alphaColor);
	copyImageWithAlpha(coloredImage, canvas, r, alphaColor);

	sf::Image copiedImage;
	copiedImage.resize(sf::Vector2u(s.size), sf::Color::Transparent);

	for (int y = 0; y < s.size.y; ++y) {
		for (int x = 0; x < s.size.x; ++x) {
			copiedImage.setPixel(sf::Vector2u(x, y), canvas.getPixel(sf::Vector2u(s.position.x + x, s.position.y + y)));
		}
	}

	copyImageToClipboard(copiedImage, sf::IntRect(sf::Vector2i(0, 0), s.size));

	sf::Image mask;
	mask.resize(sf::Vector2u(_previewImage->getSize()), sf::Color::Black);
	removeImageWithMask(canvas, _rect, mask, sf::Color::Transparent);
	getCurrentAnimation()->getCurrentLayer()->generateTexture();

	_image = nullptr;
	_previewImage = nullptr;
	_state = ResizableToolState::None;
	_rect = sf::IntRect(sf::Vector2i(-1, -1), sf::Vector2i(-1, -1));

	history->saveStep();
}

void ResizableTool::copy()
{
	if (_state != ResizableToolState::Selected)
		return;

	if (_previewImage == nullptr)
		return;

	sf::Image& canvas = getCurrentAnimation()->getCurrentLayer()->_image;
	sf::Color alphaColor = (toolbar->_option_transparency->_checkbox->_value == 0) ? sf::Color::Transparent : toolbar->_second_color->_color;

	sf::IntRect r = _rect;
	if (r.size.x < 0) { r.position.x += r.size.x; r.size.x = -r.size.x; }
	if (r.size.y < 0) { r.position.y += r.size.y; r.size.y = -r.size.y; }

	sf::IntRect canvasRect(sf::Vector2i(0, 0), sf::Vector2i(canvas.getSize()));
	auto intersection = r.findIntersection(canvasRect);
	if (!intersection.has_value())
		return;

	sf::IntRect s = intersection.value();
	if (s.size.x <= 0 || s.size.y <= 0)
		return;

	sf::Texture tex(*_previewImage);
	sf::Sprite spr(tex);
	sf::RenderTexture rtex;
	rtex.resize(_previewImage->getSize());
	rtex.clear(sf::Color::Transparent);
	_shader.setUniform("newColor", sf::Glsl::Vec4(toolbar->_first_color->_color));
	rtex.draw(spr, &_shader);
	rtex.display();
	sf::Image coloredImage = rtex.getTexture().copyToImage();

	pasteImageWithMask(canvas, coloredImage, 0, 0, *_previewImage, alphaColor);
	copyImageWithAlpha(coloredImage, canvas, r, alphaColor);


	sf::Image copiedImage;
	copiedImage.resize(sf::Vector2u(s.size), sf::Color::Transparent);

	for (int y = 0; y < s.size.y; ++y) {
		for (int x = 0; x < s.size.x; ++x) {
			copiedImage.setPixel(sf::Vector2u(x, y), canvas.getPixel(sf::Vector2u(s.position.x + x, s.position.y + y)));
		}
	}

	copyImageToClipboard(copiedImage, sf::IntRect(sf::Vector2i(0, 0), s.size));
}


void ResizableTool::pasteToCanvas() {

	if (_state == ResizableToolState::None)
		return;

	if (_image == nullptr)
		return;

	if (_previewImage == nullptr)
		return;

	pasteImageWithNewColorAndAlpha(getCurrentAnimation()->getCurrentLayer()->_image, *_previewImage, 0, 0, toolbar->_first_color->_color, sf::Color::Transparent);
	getCurrentAnimation()->getCurrentLayer()->generateTexture();
	history->saveStep();
}

void ResizableTool::drawRect() {
	if (!
		(_points.size() >= 3 &&
			(
				_state == ResizableToolState::Selecting ||
				_state == ResizableToolState::Selected ||
				_state == ResizableToolState::Moving ||
				_state == ResizableToolState::Resizing)))
		return;

	if (_rect.size.x < 1 || _rect.size.y < 1)
		return;

	float scale = canvas->_zoom * canvas->_zoom_delta;

	sf::Vector2f rectSize;
	rectSize.x = float(_rect.size.x) * scale;
	rectSize.y = float(_rect.size.y) * scale;

	sf::RectangleShape rect(rectSize);

	sf::Vector2f rectPos;
	rectPos.x = float(canvas->_position.x) + float(_rect.position.x) * scale;
	rectPos.y = float(canvas->_position.y) + float(_rect.position.y) * scale;
	rect.setPosition(rectPos);

	rect.setFillColor(selection_color);
	rect.setOutlineColor(selection_border_color);
	rect.setOutlineThickness((float)(selection_border_width));

	window->draw(rect);
}

void ResizableTool::drawImage() {

	if (!_image)
		return;

	if (_image->getSize().x == 0 || _image->getSize().y == 0)
		return;

	if (!_previewImage)
		return;

	if (_previewImage->getSize().x == 0 || _previewImage->getSize().y == 0)
		return;


	sf::Texture texture(*_image);
	sf::Sprite sprite(texture);

	float scale = canvas->_zoom * canvas->_zoom_delta;
	float sx = float(_rect.size.x) / float(_image->getSize().x) * scale;
	float sy = float(_rect.size.y) / float(_image->getSize().y) * scale;
	sprite.setScale(sf::Vector2f(sx, sy));

	sprite.setPosition(sf::Vector2f(_rect.position) * scale + sf::Vector2f(canvas->_position));

	_shader.setUniform("alphaColor", sf::Glsl::Vec4(sf::Color::Transparent));
	_shader.setUniform("newColor", sf::Glsl::Vec4(toolbar->_first_color->_color));

	window->draw(sprite, &_shader);
}

void ResizableTool::drawPreviewImage() {

	if (!_previewImage)
		return;

	if (_previewImage->getSize().x < 1 || _previewImage->getSize().y < 1)
		return;

	sf::Texture texture(*_previewImage);
	sf::Sprite sprite(texture);

	for (auto& canvas : canvases) {

		if (main_menu->canvas_repeating->_checkbox->_value == 0 && !(canvas->_coords.x == 0 && canvas->_coords.y == 0))
			continue;
		if (main_menu->canvas_repeating->_checkbox->_value == 1 && !(canvas->_coords.x == 0 || canvas->_coords.y == 0))
			continue;

		sprite.setScale(sf::Vector2f(canvas->_zoom * canvas->_zoom_delta, canvas->_zoom * canvas->_zoom_delta));
		sprite.setPosition(sf::Vector2f(canvas->_position));

		_shader.setUniform("alphaColor", sf::Glsl::Vec4(sf::Color::Transparent));
		_shader.setUniform("newColor", sf::Glsl::Vec4(toolbar->_first_color->_color));

		window->draw(sprite, &_shader);
	}

}

void ResizableTool::drawEdgePoints() {

	if (!(_points.size() >= 3 && (_state == ResizableToolState::Selected || _state == ResizableToolState::Resizing)))
		return;

	for (auto& point : _edgePoints) {
		point->draw();
	}
}

void ResizableTool::cursorHover() {

}

void ResizableTool::handleEvent(const sf::Event& event) {

}

void ResizableTool::update() {

}

void ResizableTool::draw() {
	//drawImage(); // not needed because now we operate on previewImage
	drawPreviewImage();
	drawRect();
	drawEdgePoints();
}

std::shared_ptr<ResizableTool> resizable_tool;