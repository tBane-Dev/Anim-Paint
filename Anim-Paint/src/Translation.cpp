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

	/////////////////////////////////////////////////////////////////////////////

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

	/////////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_EDIT_UNDO);
	_phrases.back().set(Language::ENG, L"Undo");
	_phrases.back().set(Language::PL, L"Cofnij");

	_phrases.emplace_back(TranslationKey::MENU_EDIT_REDO);
	_phrases.back().set(Language::ENG, L"Redo");
	_phrases.back().set(Language::PL, L"Powtórz");

	/////////////////////////////////////////////////////////////////////////////

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

	/////////////////////////////////////////////////////////////////////////////

	_phrases.emplace_back(TranslationKey::MENU_SELECT_SELECT_ALL);
	_phrases.back().set(Language::ENG, L"Select All");
	_phrases.back().set(Language::PL, L"Zaznacz Wszystko");

	_phrases.emplace_back(TranslationKey::MENU_SELECT_SELECT_NONE);
	_phrases.back().set(Language::ENG, L"Select None");
	_phrases.back().set(Language::PL, L"Odznacz Wszystko");

	_phrases.emplace_back(TranslationKey::MENU_SELECT_ALIGN_CENTER);
	_phrases.back().set(Language::ENG, L"Align to Center");
	_phrases.back().set(Language::PL, L"Wyrównaj do Centrum");

	/////////////////////////////////////////////////////////////////////////////

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




}

std::unique_ptr<Translation> translation;