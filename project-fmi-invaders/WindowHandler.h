#ifndef __WINDOW_HANDLER_HEADER_INCLUDED__
#define __WINDOW_HANDLER_HEADER_INCLUDED__

#include <windows.h>

enum Color
{
	C_BLACK,
	C_DK_BLUE,
	C_DK_GREEN,
	C_DK_CYAN,
	C_DK_RED,
	C_DK_MAGENTA,
	C_DK_YELLOW,
	C_LT_GRAY,
	C_DK_GRAY,
	C_BLUE,
	C_GREEN,
	C_CYAN,
	C_RED,
	C_MAGENTA,
	C_YELLOW,
	C_WHITE,
};

class WindowHandler
{

public:
	WindowHandler(const WindowHandler&) = delete;
	WindowHandler& operator=(const WindowHandler&) = delete;
	static WindowHandler* i();

	void initWindow();
	void printGameLogo() const;
	void initMainMenu() const;
	void drawMainMenuSelector(unsigned char sel) const;
	void clearScreen() const;
	void setupGameWindow();
	short getGameWindowWidth() const;
	short getGameWindowHeight() const;
	void clearGameWindow();
	void drawGameWindow() const;
	void drawStatus() const;
	void drawInfo(short speed, short calcTime) const;
	void drawGameOver() const;
	void drawPauseMenu();
	void clearPauseMenu();
	void drawPauseMenuSelector(unsigned short sel) const;
	void showPauseMenuMessage(const char* text);
	char lookAt(short x, short y) const;
	void writeAt(short x, short y, char ch) const;
	void setColorAt(short x, short y, Color c);
	void destroySelf();

public:
	void setFont(const wchar_t* fontName, short width, short height, unsigned fontWeight);
	void showCursor(bool visible = false);


private:
	WindowHandler();
	~WindowHandler();
	static WindowHandler* instance;

	HANDLE console;
	COORD screenSize;
	COORD bufferSize;
	struct {
		char** characters;
		char** oldCharacters;
		char** colors;
		char** oldColors;
		short x;
		short y;
		short width;
		short height;
	} gameWindow;

	struct {
		short x;
		short y;
	} pauseMenu;


private:
	void drawGameWindowBorder() const;
	void drawGUI() const;
	void setColor(int color) const;
	void drawMenuBorder() const;
	void addTextToWindow(const char* str, unsigned char xPosInWindow, unsigned char yPosInWindow);
};

#endif // !__WINDOW_HANDLER_HEADER_INCLUDED__
