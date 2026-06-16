#include "Dialogs/FileDialog/LocationRect.hpp"
#include "Dialogs/FileDialog/FileDialog.hpp"
#include "Dialogs/FileDialog/FileFunctions.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "Theme.hpp"

LocationRect::LocationRect() : Element() {

	_rect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
	_location = nullptr;

	_clickType = LocationRectClickType::None;
	unclick();
}

LocationRect::~LocationRect() {

}

void LocationRect::setLocation(std::shared_ptr<Location> location) {

	_location = location;

	if (_text) {
		if (_location) {
			if (isDrive(_location->_path.wstring()))
				_text->setString(_location->_path.wstring());
			else
				_text->setString(_location->_path.filename().wstring());
		}
		else {
			_text->setString(L"");
		}
	}
}

void LocationRect::setSize(sf::Vector2i size) {
	_rect.size = size;
}

void LocationRect::setPosition(sf::Vector2i position) {
	_rect.position = position;
}

void LocationRect::setText(std::wstring text) {

	if (_text == nullptr) {
		_text = std::make_unique<sf::Text>(basicFont, L"", basic_text_size);
		_text->setFillColor(basic_text_color);
	}

	_text->setString(text);
}

void LocationRect::unclick() {
	_state = ButtonState::Idle;
	_clickType = LocationRectClickType::None;
}

void LocationRect::hover() {
	_state = ButtonState::Hover;
}

void LocationRect::locationClick() {
	_state = ButtonState::Pressed;
	_clickType = LocationRectClickType::ClickLocation;
	_clickTime = currentTime;
}

void LocationRect::arrowClick() {
	_state = ButtonState::Pressed;
	_clickType = LocationRectClickType::ClickArrow;
	_clickTime = currentTime;
}

void LocationRect::cursorHover() {

	if (!_location)
		return;

	if (_rect.contains(cursor->_position)) {
		Element_hovered = this->shared_from_this();
	}
}

void LocationRect::handleEvent(const sf::Event& event) {

	if (!_location)
		return;

	if (_rect.contains(cursor->_position)) {

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>();
			mbp && mbp->button == sf::Mouse::Button::Left) {

			float indent = _rect.position.x + _location->_depth * _indentDelta;

			if (indent < cursor->_position.x &&
				cursor->_position.x < indent + _arrowMargin + 8) {

				Element_pressed = this->shared_from_this();
				_clickType = LocationRectClickType::ClickArrow;
			}
			else {
				Element_pressed = this->shared_from_this();
				_clickType = LocationRectClickType::ClickLocation;
			}
		}
		else if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>();
			mbr && mbr->button == sf::Mouse::Button::Left) {

			if (Element_pressed == this->shared_from_this()) {

				if (_clickType == LocationRectClickType::ClickArrow) {
					arrowClick();
				}
				else if (_clickType == LocationRectClickType::ClickLocation) {
					locationClick();
				}
			}
		}
	}
}

void LocationRect::update() {

	if (!_location) {
		unclick();
		return;
	}

	if (_state == ButtonState::Pressed) {
		if ((currentTime - _clickTime).asSeconds() > 0.05f) {

			if (_clickType == LocationRectClickType::ClickArrow) {
				if (_onclick_arrow_func)
					_onclick_arrow_func();
			}

			if (_clickType == LocationRectClickType::ClickLocation) {
				if (_onclick_location_func)
					_onclick_location_func();
			}

			if (Element_pressed.get() == this)
				Element_pressed = nullptr;

			unclick();
		}
	}
	else if (Element_hovered.get() == this) {
		hover();
	}
	else {
		unclick();
	}
}

void LocationRect::draw() {

	if (!_location)
		return;

	sf::RectangleShape rect(sf::Vector2f(_rect.size));

	switch (_state) {
	case ButtonState::Pressed:
		rect.setFillColor(file_dialog_file_rect_press_color);
		break;

	case ButtonState::Hover:
		rect.setFillColor(file_dialog_file_rect_hover_color);
		break;

	case ButtonState::Idle:
		rect.setFillColor(file_dialog_file_rect_idle_color);
		break;
	}

	rect.setPosition(sf::Vector2f(_rect.position));
	window->draw(rect);

	float indent = _location->_depth * _indentDelta;

	std::wstring arrowTexture;

	if (_location->_isOpen)
		arrowTexture = L"tex\\file_dialog\\dictionaryIsOpened.png";
	else if (_location->_hasChildren)
		arrowTexture = L"tex\\file_dialog\\dictionaryIsClosed.png";
	else
		arrowTexture = L"tex\\file_dialog\\dictionaryIsEmpty.png";

	sf::Sprite arrow(*getTexture(arrowTexture)->_texture);
	arrow.setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(indent, 6));
	window->draw(arrow);

	FileType type;

	if (isDrive(_location->_path.wstring()))
		type = FileType::Drive;
	else if (std::filesystem::is_directory(_location->_path))
		type = FileType::Directory;
	else
		type = FileType::Empty;

	std::wstring iconTexture;

	if (type == FileType::Drive)
		iconTexture = L"tex\\file_dialog\\harddrive.png";
	else if (type == FileType::Directory)
		iconTexture = L"tex\\file_dialog\\dictionary.png";
	else
		iconTexture = L"tex\\file_dialog\\empty.png";

	sf::Sprite ico(*getTexture(iconTexture)->_texture);
	ico.setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(indent + 8 + _arrowMargin, 0));
	window->draw(ico);

	if (_text == nullptr) {
		_text = std::make_unique<sf::Text>(basicFont, L"", basic_text_size);
		_text->setFillColor(basic_text_color);
	}

	if (type == FileType::Drive)
		_text->setString(_location->_path.wstring());
	else
		_text->setString(_location->_path.filename().wstring());

	_text->setPosition(
		sf::Vector2f(_rect.position) +
		sf::Vector2f(
			indent + 20 + 4 + 8 + _arrowMargin,
			(20 - basicFont.getLineSpacing(dialog_content_font_size)) / 2.0f
		)
	);

	window->draw(*_text);
}