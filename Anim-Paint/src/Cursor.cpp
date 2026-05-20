 #include "Cursor.hpp"
#include "SFML/Graphics.hpp"
#include "Dialogs/FileDialog/FileDialog.hpp"
#include "Components/Canvas.hpp"
#include "Tools/Brush.hpp"
#include "Dialogs/Palette.hpp"
#include "Tools/Selection.hpp"
#include "Controls/Slider.hpp"
#include "Components/MainMenu/MainMenu.hpp"
#include "Time.hpp" // TO-DO - to remove
#include <typeinfo> // for class name
#include "Tools/Line.hpp"

Cursor::Cursor() {

	_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::Arrow);
	_position = sf::Mouse::getPosition(*window);
	window->setMouseCursor(*_cursor);
	_hoveredElement = nullptr;
	_brushIsVisible = false;

	// cursors
	

	{
		sf::Image img = getTexture(L"tex\\cursor\\brush.png")->_texture->copyToImage();
		_brushCursor = std::make_shared<sf::Cursor>(img.getPixelsPtr(), img.getSize(), sf::Vector2u(7, 7));
	}

	{
		sf::Image img = getTexture(L"tex\\cursor\\fill.png")->_texture->copyToImage();
		_fillCursor = std::make_shared<sf::Cursor>(img.getPixelsPtr(), img.getSize(), sf::Vector2u(6, 26));
	}

	{
		sf::Image img = getTexture(L"tex\\cursor\\picker.png")->_texture->copyToImage();
		_pickerCursor = std::make_shared<sf::Cursor>(img.getPixelsPtr(), img.getSize(), sf::Vector2u(5, 26));
	}

	{
		sf::Image img = getTexture(L"tex\\cursor\\cross.png")->_texture->copyToImage();
		_crossCursor = std::make_shared<sf::Cursor>(img.getPixelsPtr(), img.getSize(), sf::Vector2u(11, 11));
	}
}

Cursor::~Cursor() {

}

void Cursor::update() {
	_position = sf::Mouse::getPosition(*window);
}

void Cursor::handleEvent() {

	brush->setPosition(worldToTile(_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta));

	std::shared_ptr<TextInput> input = dynamic_pointer_cast<TextInput>(Element_pressed);
	if (input != nullptr && input->_editState == TextInputEditState::Selecting) {
		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::Text);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	if(main_menu->_state != MainMenuStates::Closed)
		return;

	_hoveredElement = Element_hovered;
	
	/*
	if (_hoveredElement) {
		std::string className = typeid(*_hoveredElement).name();		// get class name
		std::wstring wClassName(className.begin(), className.end());	// convert to wide_string
		DebugLog(wClassName);
	}
	else {
		DebugLog(L"nullptr");
	}
	*/
	
	if (dynamic_cast<TextInput*>(_hoveredElement.get()) != nullptr) {
		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::Text);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	if (dynamic_cast<Slider*>(_hoveredElement.get()) != nullptr) {
		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::Hand);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	if (dynamic_cast<Button*>(_hoveredElement.get()) != nullptr) {
		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::Arrow);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	std::shared_ptr<EdgePoint> edgePoint = std::dynamic_pointer_cast<EdgePoint>(Element_hovered);
	if(edgePoint == nullptr)
		edgePoint = std::dynamic_pointer_cast<EdgePoint>(Element_pressed);

	if (edgePoint) {
		sf::Cursor::Type cursorType = sf::Cursor::Type::Arrow;

		switch (edgePoint->_type) {
		case EdgePointType::LeftTop:
			cursorType = sf::Cursor::Type::SizeTopLeft;
			break;
		case EdgePointType::Top:
			cursorType = sf::Cursor::Type::SizeTop;
			break;
		case EdgePointType::RightTop:
			cursorType = sf::Cursor::Type::SizeTopRight;
			break;
		case EdgePointType::Left:
			cursorType = sf::Cursor::Type::SizeLeft;
			break;
		case EdgePointType::Right:
			cursorType = sf::Cursor::Type::SizeRight;
			break;
		case EdgePointType::LeftBottom:
			cursorType = sf::Cursor::Type::SizeBottomLeft;
			break;
		case EdgePointType::Bottom:
			cursorType = sf::Cursor::Type::SizeBottom;
			break;
		case EdgePointType::RightBottom:
			cursorType = sf::Cursor::Type::SizeBottomRight;
			break;
		}

		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(cursorType);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	
	bool canvasIsMoving = false;
	for(auto& canvas : canvases) {
		if (canvas->_state == CanvasState::Moving) {
			canvasIsMoving = true;
			break;
		}
	}
	if (canvasIsMoving) {
		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::SizeAll);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}


	if (resizable_tool != nullptr && (Element_pressed == nullptr || canvasIsPressed() || Element_pressed == resizable_tool) && resizable_tool->_state == ResizableToolState::Selecting) {
		window->setMouseCursorVisible(true);
		_cursor = _crossCursor;
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	sf::Vector2i tile = worldToTile(cursor->_position, canvas->_position, canvas->_zoom, canvas->_zoom_delta);
	if ((_hoveredElement == resizable_tool && resizable_tool != nullptr && resizable_tool->_state == ResizableToolState::Selected && !(palette!=nullptr && palette->_rect.contains(_position))) ||
		(_hoveredElement == resizable_tool && resizable_tool != nullptr && resizable_tool->_state == ResizableToolState::Moving)) {

		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::SizeAll);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	if (dynamic_cast<LocationAndFilesSeparator*>(_hoveredElement.get()) != nullptr || dynamic_cast<LocationAndFilesSeparator*>(Element_pressed.get()) != nullptr) {
		window->setMouseCursorVisible(true);
		_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::SizeHorizontal);
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	if (dynamic_cast<PaletteValues*>(_hoveredElement.get()) != nullptr || dynamic_cast<PaletteValues*>(Element_pressed.get()) != nullptr) {
		window->setMouseCursorVisible(true);
		_cursor = _crossCursor;
		window->setMouseCursor(*_cursor);
		_brushIsVisible = false;
		return;
	}

	std::shared_ptr<Canvas> hoveredCanvas = canvasIsHovered();
	std::shared_ptr<Canvas> pressedCanvas = canvasIsPressed();

	if (pressedCanvas || (hoveredCanvas && Element_pressed==nullptr)) {
		if (toolbar->_toolType == ToolType::Brush) {
			window->setMouseCursorVisible(true);
			_cursor = _brushCursor;
			window->setMouseCursor(*_cursor);
			_brushIsVisible = true;
		}
		else if (toolbar->_toolType == ToolType::Eraser) {
			window->setMouseCursorVisible(true);
			_cursor = _brushCursor;
			window->setMouseCursor(*_cursor);
			_brushIsVisible = true; 
		}
		else if (toolbar->_toolType == ToolType::Fill) {
			window->setMouseCursorVisible(true);
			_cursor = _fillCursor;
			window->setMouseCursor(*_cursor);
			_brushIsVisible = false;	
		}
		else if (toolbar->_toolType == ToolType::Picker) {
			window->setMouseCursorVisible(true);
			_cursor = _pickerCursor;
			window->setMouseCursor(*_cursor);
			_brushIsVisible = false;
		}
		else {
			window->setMouseCursorVisible(true);
			_cursor = _crossCursor;
			window->setMouseCursor(*_cursor);
			_brushIsVisible = false;
		}

		return;
	}

	

	int d = 4;
	if (_position.x <= d || _position.y <= d || _position.x >= int(window->getSize().x - d) || _position.y >= int(window->getSize().y - d)) {
		return;
	}

	window->setMouseCursorVisible(true);
	_cursor = std::make_shared<sf::Cursor>(sf::Cursor::Type::Arrow);
	window->setMouseCursor(*_cursor);
	_brushIsVisible = false;
}

void Cursor::draw() {

	int d = 4;
	if (_position.x <= d || _position.y <= d || _position.x >= int(window->getSize().x - d) || _position.y >= int(window->getSize().y - d)) {
		return;
	}

	if (_brushIsVisible == true) {
		brush->draw();
	}
}

std::shared_ptr<Cursor> cursor;