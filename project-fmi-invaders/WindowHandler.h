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

    /**
        Initializes and sets up the console window
    */
    void initWindow();

    /**
        Prints the game logo
    */
    void printGameLogo() const;

    /**
        Initializes and sets up the main menu
    */
    void initMainMenu() const;

    /**
        Draws the selector of the main menu at the given position
    */
    void drawMainMenuSelector(unsigned char sel) const;

    /**
        Clears the entire console
    */
    void clearScreen() const;

    /**
        Sets up the game window
    */
    void setupGameWindow();

    /**
        Returns the width of the gameWindow in characters
    */
    short getGameWindowWidth() const;

    /**
        Returns the height of the gameWindow in characters
    */
    short getGameWindowHeight() const;

    /**
        Sets the current frame to an empty one
    */
    void clearGameWindow();

    /**
        Draws the current frame
    */
    void drawGameWindow() const;

    /**
        Draws the player's health, score, lives
    */
    void drawStatus() const;

    /**
        Draws the current speed of the game (in ticks) and the amount of time take to calculate and draw the current frame
    */
    void drawInfo(short speed, short calcTime) const;

    /**
        Draws the game over text
    */
    void drawGameOver() const;

    /**
        Draws the pause menu
    */
    void drawPauseMenu();

    /**
        Clears any characters where the pause menu should be
    */
    void clearPauseMenu();

    /**
        Draws the selector of the pause menu at the given position
    */
    void drawPauseMenuSelector(unsigned short sel) const;

    /**
        Shows the specified text inside the pause menu for a specified time
    */
    void showPauseMenuMessage(const char* text, unsigned time = 1500);

    /**
        Returns the character at position (x, y) inside the gameWindow
    */
    char lookAt(short x, short y) const;

    /**
        Sets the character at position (x, y) inside the gameWindow to the specified char
    */
    void writeAt(short x, short y, char ch) const;

    /**
        Sets the color of the character at position (x, y) inside the gameWindow to the specified color
    */
    void setColorAt(short x, short y, Color c);

    /**
        Destroys the GameEngine instance
    */
    void destroySelf();

    /**
        Sets the console font to the specified fontName, width, height and fontWeight of the characters 
    */
    void setFont(const wchar_t* fontName, short width, short height, unsigned fontWeight);

    /**
        Enable or disable the console's cursor
    */
    void showCursor(bool visible = false);


private:
    WindowHandler();
    ~WindowHandler();


    static WindowHandler* instance;     // The WindowHandler instance
    HANDLE console;                     // The handle for the console
    COORD screenSize;                   // Structure containing the width and height of the console (in characters)
    COORD bufferSize;                   // Structure containing the width and height of the gameWindow (in characters)
    struct {
        char** characters;              // Matrix with characters for the current frame
        char** oldCharacters;           // Matrix with characters for the previous frame
        char** colors;                  // Matrix with colors for the current frame
        char** oldColors;               // Matrix with colors for the previous frame
        short x;                        // The x poision of the game window (relative to the console)
        short y;                        // The y poision of the game window (relative to the console)
        short width;                    // Width of the frame {gameWindow} (in characters)
        short height;                   // Height of the frame {gameWindow} (in characters)
    } gameWindow;

    struct {
        short x;                        // Pause menu's x position (relative to the console)
        short y;                        // Pause menu's y position (relative to the console)
    } pauseMenu;


private:
    /**
        Draws a border around the gameWindow
    */
    void drawGameWindowBorder() const;

    /**
        Draws the empty GUI around the gameWindow
    */
    void drawGUI() const;

    /**
        Sets the color of the characters when writing to the console
    */
    void setColor(int color) const;

    /**
        Draws a border around the pause menu
    */
    void drawMenuBorder() const;

    /**
        Draws text inside the gameWindow at the specified location
    */
    void addTextToWindow(const char* str, unsigned char xPosInWindow, unsigned char yPosInWindow);
};

#endif // !__WINDOW_HANDLER_HEADER_INCLUDED__
