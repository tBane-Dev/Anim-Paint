#include "Translation.hpp"

std::wstring enumToString(TranslationKey key)
{
	switch (key) {
	#define X(name) case TranslationKey::name: return WIDEN(#name);
			TRANSLATION_KEYS
	#undef X
	default: return L"UNKNOWN";
	}
}

Phrase::Phrase(TranslationKey key) {
	_key = key;
	_ENG = _PL = enumToString(_key);
}

Phrase::~Phrase() {

}


void Phrase::set(Language language, std::wstring text) {
	if (language == Language::ENG)
		_ENG = text;
	if (language == Language::PL)
		_PL = text;
}

std::wstring Phrase::get(Language language) {
	if (language == Language::ENG)
		return _ENG;
	if (language == Language::PL)
		return _PL;

	return enumToString(_key);
}

Translation::Translation() {

	_currentLanguage = Language::PL;
	loadPhrases();
}

Translation::~Translation() { }

std::wstring Translation::get(TranslationKey key) {
	for (auto& v : _phrases) {
		if (v._key == key)
			return v.get(_currentLanguage);
	}

	return enumToString(key);
}

void Translation::loadPhrases() {
	_phrases.clear();

	// MAIN MENU ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_FILE);
	_phrases.back().set(Language::ENG, L"File");
	_phrases.back().set(Language::PL, L"Plik");

	_phrases.emplace_back(TranslationKey::MENU_EDIT);
	_phrases.back().set(Language::ENG, L"Edit");
	_phrases.back().set(Language::PL, L"Edycja");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS);
	_phrases.back().set(Language::ENG, L"Tools");
	_phrases.back().set(Language::PL, L"Narzędzia");

	_phrases.emplace_back(TranslationKey::MENU_SELECT);
	_phrases.back().set(Language::ENG, L"Select");
	_phrases.back().set(Language::PL, L"Zaznaczenie");

	_phrases.emplace_back(TranslationKey::MENU_WINDOWS);
	_phrases.back().set(Language::ENG, L"Windows");
	_phrases.back().set(Language::PL, L"Okna");

	// MAIN MENU FILE ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_FILE_NEW);
	_phrases.back().set(Language::ENG, L"New File");
	_phrases.back().set(Language::PL, L"Nowy Plik");

	_phrases.emplace_back(TranslationKey::MENU_FILE_SAVE_AS);
	_phrases.back().set(Language::ENG, L"Save As");
	_phrases.back().set(Language::PL, L"Zapisz Jako");

	_phrases.emplace_back(TranslationKey::MENU_FILE_LOAD);
	_phrases.back().set(Language::ENG, L"Load");
	_phrases.back().set(Language::PL, L"Wczytaj");

	_phrases.emplace_back(TranslationKey::MENU_FILE_EXPORT);
	_phrases.back().set(Language::ENG, L"Export");
	_phrases.back().set(Language::PL, L"Exportuj");

	_phrases.emplace_back(TranslationKey::MENU_FILE_IMPORT);
	_phrases.back().set(Language::ENG, L"Import");
	_phrases.back().set(Language::PL, L"Importuj");

	// MAIN MENU EDIT ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_EDIT_UNDO);
	_phrases.back().set(Language::ENG, L"Undo");
	_phrases.back().set(Language::PL, L"Cofnij");

	_phrases.emplace_back(TranslationKey::MENU_EDIT_REDO);
	_phrases.back().set(Language::ENG, L"Redo");
	_phrases.back().set(Language::PL, L"Powtórz");

	// MAIN MENU TOOLS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_RESIZE);
	_phrases.back().set(Language::ENG, L"Resize");
	_phrases.back().set(Language::PL, L"Zmiana rozmiaru");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_ROTATION);
	_phrases.back().set(Language::ENG, L"Rotation");
	_phrases.back().set(Language::PL, L"Obrót");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_BRIGHTNESS_CONTRAST);
	_phrases.back().set(Language::ENG, L"Brightness and Contrast");
	_phrases.back().set(Language::PL, L"Jasność i Kontrast");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_HUE_SATURATION);
	_phrases.back().set(Language::ENG, L"Hue and Saturation");
	_phrases.back().set(Language::PL, L"Barwa i Nasycenie");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_SEPIA);
	_phrases.back().set(Language::ENG, L"Sepia");
	_phrases.back().set(Language::PL, L"Sepia");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_OUTLINE);
	_phrases.back().set(Language::ENG, L"Outline");
	_phrases.back().set(Language::PL, L"Obrys");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_INVERT);
	_phrases.back().set(Language::ENG, L"Invert Colors");
	_phrases.back().set(Language::PL, L"Odwróć Kolory");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_CHESSBOARD);
	_phrases.back().set(Language::ENG, L"Chessboard");
	_phrases.back().set(Language::PL, L"Szachownica");

	_phrases.emplace_back(TranslationKey::MENU_TOOLS_SMOOTHING);
	_phrases.back().set(Language::ENG, L"Smoothing");
	_phrases.back().set(Language::PL, L"Wygładzanie");

	// MAIN MENU SELECT ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_SELECT_SELECT_ALL);
	_phrases.back().set(Language::ENG, L"Select All");
	_phrases.back().set(Language::PL, L"Zaznacz Wszystko");

	_phrases.emplace_back(TranslationKey::MENU_SELECT_SELECT_NONE);
	_phrases.back().set(Language::ENG, L"Select None");
	_phrases.back().set(Language::PL, L"Odznacz");

	_phrases.emplace_back(TranslationKey::MENU_SELECT_ALIGN_CENTER);
	_phrases.back().set(Language::ENG, L"Align to Center");
	_phrases.back().set(Language::PL, L"Wyrównaj do Centrum");

	// MAIN MENU WINDOWS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_WINDOWS_CANVAS_REPEATING);
	_phrases.back().set(Language::ENG, L"Canvas Repeating");
	_phrases.back().set(Language::PL, L"Powtarzanie Tła");

	_phrases.emplace_back(TranslationKey::MENU_WINDOWS_ANIMATIONS);
	_phrases.back().set(Language::ENG, L"Animations");
	_phrases.back().set(Language::PL, L"Animacje");

	_phrases.emplace_back(TranslationKey::MENU_WINDOWS_FRAMES);
	_phrases.back().set(Language::ENG, L"Frames");
	_phrases.back().set(Language::PL, L"Klatki");

	_phrases.emplace_back(TranslationKey::MENU_WINDOWS_LAYERS);
	_phrases.back().set(Language::ENG, L"Layers");
	_phrases.back().set(Language::PL, L"Warstwy");

	_phrases.emplace_back(TranslationKey::MENU_WINDOWS_PREVIEW_ANIMATION);
	_phrases.back().set(Language::ENG, L"Preview Animation");
	_phrases.back().set(Language::PL, L"Podgląd Animacji");

	// TOLBAR BOTTOM TEXTS ///////////////////////////////////////////////////////////////////////////
	_phrases.emplace_back(TranslationKey::TOOLBAR_BOTTOM_TEXT_CLIPBOARD);
	_phrases.back().set(Language::ENG, L"clipboard");
	_phrases.back().set(Language::PL, L"schowek");

	_phrases.emplace_back(TranslationKey::TOOLBAR_BOTTOM_TEXT_TOOLS);
	_phrases.back().set(Language::ENG, L"tools");
	_phrases.back().set(Language::PL, L"narzędzia");

	_phrases.emplace_back(TranslationKey::TOOLBAR_BOTTOM_TEXT_SHAPES);
	_phrases.back().set(Language::ENG, L"shapes");
	_phrases.back().set(Language::PL, L"kształty");

	_phrases.emplace_back(TranslationKey::TOOLBAR_BOTTOM_TEXT_SIZES);
	_phrases.back().set(Language::ENG, L"size");
	_phrases.back().set(Language::PL, L"rozmiar");

	_phrases.emplace_back(TranslationKey::TOOLBAR_BOTTOM_TEXT_COLOR1);
	_phrases.back().set(Language::ENG, L"color");
	_phrases.back().set(Language::PL, L"kolor");

	_phrases.emplace_back(TranslationKey::TOOLBAR_BOTTOM_TEXT_COLOR2);
	_phrases.back().set(Language::ENG, L"color");
	_phrases.back().set(Language::PL, L"kolor");

	_phrases.emplace_back(TranslationKey::TOOLBAR_BOTTOM_TEXT_COLORS);
	_phrases.back().set(Language::ENG, L"colors");
	_phrases.back().set(Language::PL, L"kolory");
	

	// TOOLBAR CLIPBOARD ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_PASTE);
	_phrases.back().set(Language::ENG, L"Paste");
	_phrases.back().set(Language::PL, L"Wklej");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_PASTE_FROM_FILE);
	_phrases.back().set(Language::ENG, L"Paste From File");
	_phrases.back().set(Language::PL, L"Wklej z Pliku");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_TRANSPARENCY);
	_phrases.back().set(Language::ENG, L"Transparency");
	_phrases.back().set(Language::PL, L"Przezroczystość");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_CUT);
	_phrases.back().set(Language::ENG, L"Cut");
	_phrases.back().set(Language::PL, L"Wytnij");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_COPY);
	_phrases.back().set(Language::ENG, L"Copy");
	_phrases.back().set(Language::PL, L"Kopiuj");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_SELECT);
	_phrases.back().set(Language::ENG, L"Select");
	_phrases.back().set(Language::PL, L"Zaznacz");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_LASSO);
	_phrases.back().set(Language::ENG, L"Lasso");
	_phrases.back().set(Language::PL, L"Lasso");

	// TOOLBAR CLIPBOARD NAMES ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_PASTE_NAME);
	_phrases.back().set(Language::ENG, L"Paste");
	_phrases.back().set(Language::PL, L"Wklej");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_PASTE_FROM_FILE_NAME);
	_phrases.back().set(Language::ENG, L"Paste From File");
	_phrases.back().set(Language::PL, L"Wklej z Pliku");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_TRANSPARENCY_NAME);
	_phrases.back().set(Language::ENG, L"Transparency");
	_phrases.back().set(Language::PL, L"Przezroczystość");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_CUT_NAME);
	_phrases.back().set(Language::ENG, L"Cut");
	_phrases.back().set(Language::PL, L"Wytnij");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_COPY_NAME);
	_phrases.back().set(Language::ENG, L"Copy");
	_phrases.back().set(Language::PL, L"Kopiuj");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_SELECT_NAME);
	_phrases.back().set(Language::ENG, L"Selection Tool");
	_phrases.back().set(Language::PL, L"Zaznacz");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_LASSO_NAME);
	_phrases.back().set(Language::ENG, L"Lasso Tool");
	_phrases.back().set(Language::PL, L"Lasso");

	// TOOLBAR CLIPBOARD DESCRIPTIONS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_PASTE_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Paste the selection from the clipboard onto the canvas");
	_phrases.back().set(Language::PL, L"Wklej zaznaczenie ze schowka na płótno");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_PASTE_FROM_FILE_DESCRIPTION);
	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_TRANSPARENCY_DESCRIPTION);

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_CUT_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Cut the selection from the canvas and place it on the clipboard");
	_phrases.back().set(Language::PL, L"Wytnij zaznaczenie z płótna do schowka");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_COPY_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Copy the selection from the canvas to the clipboard");
	_phrases.back().set(Language::PL, L"Skopiuj zaznaczenie z płótna do schowka");
	
	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_SELECT_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Select an area of the canvas move, copy, or cut");
	_phrases.back().set(Language::PL, L"Zaznacz obszar płótna do przeniesienia, skopiowania lub wycięcia");

	_phrases.emplace_back(TranslationKey::TOOLBAR_CLIPBOARD_LASSO_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Select an area of the canvas with a freehand selection to move, copy or cut");
	_phrases.back().set(Language::PL, L"Zaznacz obszar płótna za pomocą swobodnego zaznaczenia, aby przenieść, skopiować lub wyciąć");

	// TOOLBAR TOOLS NAMES ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_BRUSH_NAME);
	_phrases.back().set(Language::ENG, L"Brush Tool");
	_phrases.back().set(Language::PL, L"Pędzel");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_ERASER_NAME);
	_phrases.back().set(Language::ENG, L"Eraser Tool");
	_phrases.back().set(Language::PL, L"Gumka");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_PICKER_NAME);
	_phrases.back().set(Language::ENG, L"Color Picker Tool");
	_phrases.back().set(Language::PL, L"Pobierz kolor");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_FILL_NAME);
	_phrases.back().set(Language::ENG, L"Fill Tool");
	_phrases.back().set(Language::PL, L"Wypełnij kolorem");

	// TOOLBAR TOOLS DESCRIPTIONS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_BRUSH_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw on the canvas using the primary color with LMB or the secondary color with RMB");
	_phrases.back().set(Language::PL, L"Rysuj na płótnie używając głównego koloru poprzez LPM lub koloru pomocniczego poprzez PPM");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_ERASER_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Erase parts of the canvas using the secondary color with LMB or the primary color with RMB");
	_phrases.back().set(Language::PL, L"Wymaż części płótna używając koloru pomocniczego poprzez LPM lub koloru głównego poprzez PPM");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_PICKER_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Pick a color from the canvas and set it as the active color");
	_phrases.back().set(Language::PL, L"Pobierz kolor z płótna klikając LPM dla aktywnego koloru");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_FILL_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Fill an area of the canvas using the primary color with LMB or the secondary color with RMB");
	_phrases.back().set(Language::PL, L"Wypełnij obszar płótna głównym kolorem poprzez LPM lub kolorem pomocniczym poprzez PPM");

	// TOOLBAR SHAPES NAMES ///////////////////////////////////////////////////////////////////////////
	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_CIRCLE_NAME);
	_phrases.back().set(Language::ENG, L"Circle Shape Tool");
	_phrases.back().set(Language::PL, L"Koło");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_TRIANGLE_NAME);
	_phrases.back().set(Language::ENG, L"Triangle Shape Tool");
	_phrases.back().set(Language::PL, L"Trójkąt");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_RECTANGLE_NAME);
	_phrases.back().set(Language::ENG, L"Rectangle Shape Tool");
	_phrases.back().set(Language::PL, L"Prostokąt");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_DIAMOND_NAME);
	_phrases.back().set(Language::ENG, L"Diamond Shape Tool");
	_phrases.back().set(Language::PL, L"Romb");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_PENTAGON_NAME);
	_phrases.back().set(Language::ENG, L"Pentagon Shape Tool");
	_phrases.back().set(Language::PL, L"Pięciokąt");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_HEXAGON_FLAT_TOP_NAME);
	_phrases.back().set(Language::ENG, L"Hexagon (Flat Top) Shape Tool");
	_phrases.back().set(Language::PL, L"Sześciokąt (Płaski Wierzch) ");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_HEXAGON_POINT_TOP_NAME);
	_phrases.back().set(Language::ENG, L"Hexagon (Point Top) Shape Tool");
	_phrases.back().set(Language::PL, L"Sześciokąt (Ostry Wierzch) ");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_OCTAGON_NAME);
	_phrases.back().set(Language::ENG, L"Octagon Shape Tool");
	_phrases.back().set(Language::PL, L"Ośmiokąt");

	// TOOLBAR SHAPES DESCRIPTIONS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_CIRCLE_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw a circle filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj koło wypełnione kolorem pomocniczym używając koloru głównego");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_TRIANGLE_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw a triangle filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj trójkąt wypełniony kolorem pomocniczym używając koloru głównego");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_RECTANGLE_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw a rectangle filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj prostokąt wypełniony kolorem pomocniczym używając koloru głównego");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_DIAMOND_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw a diamond filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj romb wypełniony kolorem pomocniczym używając koloru głównego");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_PENTAGON_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw a pentagon filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj pięciokąt wypełniony kolorem pomocniczym używając koloru głównego");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_HEXAGON_FLAT_TOP_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw a flat-top hexagon filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj sześciokąt z płaskim wierzchem wypełniony kolorem pomocniczym używając koloru głównego");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_HEXAGON_POINT_TOP_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw a point-top hexagon filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj sześciokąt z ostrym wierzchem wypełniony kolorem pomocniczym używając koloru głównego");

	_phrases.emplace_back(TranslationKey::TOOLBAR_TOOLS_OCTAGON_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Draw an octagon filled with the secondary color using the primary color");
	_phrases.back().set(Language::PL, L"Rysuj ośmiokąt wypełniony kolorem pomocniczym używając koloru głównego");

	// TOOLBAR SIZES NAMES ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_SIZES_DECREASE_NAME);
	_phrases.back().set(Language::ENG, L"Decrease Size");
	_phrases.back().set(Language::PL, L"Zmniejsz Rozmiar");

	_phrases.emplace_back(TranslationKey::TOOLBAR_SIZES_INCREASE_NAME);
	_phrases.back().set(Language::ENG, L"Increase Size");
	_phrases.back().set(Language::PL, L"Zwiększ Rozmiar");

	// TOOLBAR SIZES DESCRIPTIONS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_SIZES_DECREASE_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Decrease the size of the brush, eraser or shape outline");
	_phrases.back().set(Language::PL, L"Zmniejsz rozmiar pędzla, gumki lub obrysu kształtu");

	_phrases.emplace_back(TranslationKey::TOOLBAR_SIZES_INCREASE_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Increase the size of the brush, eraser or shape outline");
	_phrases.back().set(Language::PL, L"Zwiększ rozmiar pędzla, gumki lub obrysu kształtu");

	// TOOLBAR MAIN COLORS NAMES ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR1_NAME);
	_phrases.back().set(Language::ENG, L"Primary Color");
	_phrases.back().set(Language::PL, L"Kolor Główny");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR2_NAME);
	_phrases.back().set(Language::ENG, L"Secondary Color");
	_phrases.back().set(Language::PL, L"Kolor Pomocniczy");

	// TOOLBAR MAIN COLORS DESCRIPTIONS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR1_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Set the primary color used for drawing and filling");
	_phrases.back().set(Language::PL, L"Ustaw kolor główny używany do rysowania i wypełniania");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR2_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Set the secondary color used for drawing and filling");
	_phrases.back().set(Language::PL, L"Ustaw kolor pomocniczy używany do rysowania i wypełniania");

	// TOOLBAR COLORS NAMES ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_BLACK_NAME);
	_phrases.back().set(Language::ENG, L"Black");
	_phrases.back().set(Language::PL, L"Czarny");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_WHITE_NAME);
	_phrases.back().set(Language::ENG, L"White");
	_phrases.back().set(Language::PL, L"Biały");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_GREY_NAME);
	_phrases.back().set(Language::ENG, L"Dark Grey");
	_phrases.back().set(Language::PL, L"Ciemny Szary");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_GREY_NAME);
	_phrases.back().set(Language::ENG, L"Light Grey");
	_phrases.back().set(Language::PL, L"Jasny Szary");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_RED_NAME);
	_phrases.back().set(Language::ENG, L"Dark Red");
	_phrases.back().set(Language::PL, L"Ciemny Czerwony");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_RED_NAME);
	_phrases.back().set(Language::ENG, L"Light Red");
	_phrases.back().set(Language::PL, L"Jasny Czerwony");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_ORANGE_NAME);
	_phrases.back().set(Language::ENG, L"Dark Orange");
	_phrases.back().set(Language::PL, L"Ciemny Pomarańczowy");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_ORANGE_NAME);
	_phrases.back().set(Language::ENG, L"Light Orange");
	_phrases.back().set(Language::PL, L"Jasny Pomarańczowy");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_YELLOW_NAME);
	_phrases.back().set(Language::ENG, L"Dark Yellow");
	_phrases.back().set(Language::PL, L"Ciemny Żółty");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_YELLOW_NAME);
	_phrases.back().set(Language::ENG, L"Light Yellow");
	_phrases.back().set(Language::PL, L"Jasny Żółty");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_GREEN_NAME);
	_phrases.back().set(Language::ENG, L"Dark Green");
	_phrases.back().set(Language::PL, L"Ciemny Zielony");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_GREEN_NAME);
	_phrases.back().set(Language::ENG, L"Light Green");
	_phrases.back().set(Language::PL, L"Jasny Zielony");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_CYAN_NAME);
	_phrases.back().set(Language::ENG, L"Dark Cyan");
	_phrases.back().set(Language::PL, L"Ciemny Cyjan");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_CYAN_NAME);
	_phrases.back().set(Language::ENG, L"Light Cyan");
	_phrases.back().set(Language::PL, L"Jasny Cyjan");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_BLUE_NAME);
	_phrases.back().set(Language::ENG, L"Dark Blue");
	_phrases.back().set(Language::PL, L"Ciemny Niebieski");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_BLUE_NAME);
	_phrases.back().set(Language::ENG, L"Light Blue");
	_phrases.back().set(Language::PL, L"Jasny Niebieski");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_DARK_MAGENTA_NAME);
	_phrases.back().set(Language::ENG, L"Dark Magenta");
	_phrases.back().set(Language::PL, L"Ciemny Magenta");

	_phrases.emplace_back(TranslationKey::TOOLBAR_COLOR_LIGHT_MAGENTA_NAME);
	_phrases.back().set(Language::ENG, L"Light Magenta");
	_phrases.back().set(Language::PL, L"Jasny Magenta");

	// COMPONENT ANIMATIONS PANEL NAMES ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_TITLE);
	_phrases.back().set(Language::ENG, L"Animations");
	_phrases.back().set(Language::PL, L"Animacje");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_FIRST_BTN_NAME);
	_phrases.back().set(Language::ENG, L"First animation");
	_phrases.back().set(Language::PL, L"Pierwsza animacja");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_PREV_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Previous animation");
	_phrases.back().set(Language::PL, L"Poprzednia animacja");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_NEXT_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Next animation");
	_phrases.back().set(Language::PL, L"Następna animacja");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_LAST_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Last animation");
	_phrases.back().set(Language::PL, L"Ostatnia animacja");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_ADD_ANIM_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Add animation");
	_phrases.back().set(Language::PL, L"Dodaj animację");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_REMOVE_ANIM_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Remove animation");
	_phrases.back().set(Language::PL, L"Usuń animację");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_MOVE_BACK_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Move animation back");
	_phrases.back().set(Language::PL, L"Przesuń animację wstecz");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_MOVE_NEXT_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Move animation next");
	_phrases.back().set(Language::PL, L"Przesuń Animację do przodu");

	// COMPONENT ANIMATIONS PANEL DESCRIPTIONS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_FIRST_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the first animation");
	_phrases.back().set(Language::PL, L"Przejdź do pierwszej animacji");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_PREV_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the previous animation");
	_phrases.back().set(Language::PL, L"Przejdź do poprzedniej animacji");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_NEXT_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the next animation");
	_phrases.back().set(Language::PL, L"Przejdź do następnej animacji");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_LAST_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the last animation");
	_phrases.back().set(Language::PL, L"Przejdź do ostatniej animacji");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_ADD_ANIM_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Add a new animation after the current one");
	_phrases.back().set(Language::PL, L"Dodaj nową animację po aktualnej");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_REMOVE_ANIM_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Remove the current animation");
	_phrases.back().set(Language::PL, L"Usuń aktualną animację");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_MOVE_BACK_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Move the current animation one position back");
	_phrases.back().set(Language::PL, L"Przesuń aktualną animację o jedną pozycję wstecz");

	_phrases.emplace_back(TranslationKey::ANIMATIONS_PANEL_MOVE_NEXT_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Move the current animation one position forward");
	_phrases.back().set(Language::PL, L"Przesuń aktualną animację o jedną pozycję do przodu");

	// COMPONENT FRAMES PANEL NAMES ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_TITLE);
	_phrases.back().set(Language::ENG, L"Frames");
	_phrases.back().set(Language::PL, L"Klatki");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_FIRST_BTN_NAME);
	_phrases.back().set(Language::ENG, L"First frame");
	_phrases.back().set(Language::PL, L"Pierwsza klatka");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_PREV_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Previous frame");
	_phrases.back().set(Language::PL, L"Poprzednia klatka");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_NEXT_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Next frame");
	_phrases.back().set(Language::PL, L"Następna klatka");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_LAST_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Last frame");
	_phrases.back().set(Language::PL, L"Ostatnia klatka");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_ADD_FRAME_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Add frame");
	_phrases.back().set(Language::PL, L"Dodaj klatkę");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_REMOVE_FRAME_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Remove frame");
	_phrases.back().set(Language::PL, L"Usuń klatkę");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_MOVE_BACK_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Move frame back");
	_phrases.back().set(Language::PL, L"Przesuń klatkę wstecz");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_MOVE_NEXT_BTN_NAME);
	_phrases.back().set(Language::ENG, L"Move frame next");
	_phrases.back().set(Language::PL, L"Przesuń klatkę do przodu");

	// COMPONENT FRAMES PANEL DESCRIPTIONS ///////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_FIRST_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the first frame");
	_phrases.back().set(Language::PL, L"Przejdź do pierwszej klatki");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_PREV_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the previous frame");
	_phrases.back().set(Language::PL, L"Przejdź do poprzedniej klatki");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_NEXT_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the next frame");
	_phrases.back().set(Language::PL, L"Przejdź do następnej klatki");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_LAST_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Go to the last frame");
	_phrases.back().set(Language::PL, L"Przejdź do ostatniej klatki");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_ADD_FRAME_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Add a new frame after the current one");
	_phrases.back().set(Language::PL, L"Dodaj nową klatkę po aktualnej");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_REMOVE_FRAME_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Remove the current frame");
	_phrases.back().set(Language::PL, L"Usuń aktualną klatkę");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_MOVE_BACK_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Move the current frame one position back");
	_phrases.back().set(Language::PL, L"Przesuń aktualną klatkę o jedną pozycję wstecz");

	_phrases.emplace_back(TranslationKey::FRAMES_PANEL_MOVE_NEXT_BTN_DESCRIPTION);
	_phrases.back().set(Language::ENG, L"Move the current frame one position forward");
	_phrases.back().set(Language::PL, L"Przesuń aktualną klatkę o jedną pozycję do przodu");





}

std::unique_ptr<Translation> translation;