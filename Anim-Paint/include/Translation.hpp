#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <memory>

enum class Language { ENG, PL };

#define TRANSLATION_KEYS \
    X(MENU_FILE) \
    X(MENU_EDIT) \
    X(MENU_TOOLS) \
    X(MENU_SELECT) \
    X(MENU_WINDOWS) \
	\
	X(MENU_FILE_NEW) \
	X(MENU_FILE_SAVE_AS) \
	X(MENU_FILE_LOAD) \
	X(MENU_FILE_EXPORT) \
	X(MENU_FILE_IMPORT) \
	\
	X(MENU_EDIT_UNDO) \
	X(MENU_EDIT_REDO) \
	\
	X(MENU_TOOLS_RESIZE) \
	X(MENU_TOOLS_ROTATION) \
	X(MENU_TOOLS_BRIGHTNESS_CONTRAST) \
	X(MENU_TOOLS_HUE_SATURATION) \
	X(MENU_TOOLS_SEPIA) \
	X(MENU_TOOLS_OUTLINE) \
	X(MENU_TOOLS_INVERT) \
	X(MENU_TOOLS_CHESSBOARD) \
	X(MENU_TOOLS_SMOOTHING) \
	\
	X(MENU_SELECT_SELECT_ALL) \
	X(MENU_SELECT_SELECT_NONE) \
	X(MENU_SELECT_ALIGN_CENTER) \
	\
	X(MENU_WINDOWS_CANVAS_REPEATING) \
	X(MENU_WINDOWS_ANIMATIONS) \
	X(MENU_WINDOWS_FRAMES) \
	X(MENU_WINDOWS_LAYERS) \
	X(MENU_WINDOWS_PREVIEW_ANIMATION) 


enum class TranslationKey {
#define X(name) name,
	TRANSLATION_KEYS
#undef X
};

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)

std::wstring enumToString(TranslationKey key);


class Phrase {
public:
	TranslationKey _key;
	std::wstring _ENG;
	std::wstring _PL;

	Phrase(TranslationKey key);
	~Phrase();

	void set(Language language, std::wstring text);
	std::wstring get(Language language);
};

class Translation {
public:
	std::vector<Phrase> _phrases;
	Language _currentLanguage;

	Translation();
	~Translation();
	std::wstring get(TranslationKey key);
	void loadPhrases();
};

extern std::unique_ptr<Translation> translation;
