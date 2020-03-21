/**
    FMI OOP 2018 Final Project
    WindowsHandler.cpp

    @author Alexander Dimitrov
    @version 1.42   03/07/18
*/



#include <iostream>
#include <cmath>
#include <io.h>
#include <fcntl.h>
#include "WindowHandler.h"
#include "GameEngine.h"
 
const short     MIN_WIDTH              = 58;
const short     MIN_HEIGHT             = 30;
const short     MAX_WIDTH              = 90;
const short     MAX_HEIGHT             = 40;
const short     MIN_GAME_WINDOW_X      = 1;
const short     MIN_GAME_WINDOW_Y      = 2;
const short     PAUSE_MENU_WIDTH       = 20;
const short     PAUSE_MENU_HEIGHT      = 9;
const wchar_t   BORDER_CHAR            = L'█';
const wchar_t   HEART                  = L'♥';
const wchar_t   HEALTH[]               = L"█░▒▓";
const wchar_t   PAUSE_MENU_SELECTOR    = L'►';

const short     TOP_GUI_OFFSET         = 2;
const short     BOTTOM_GUI_OFFSET      = 1;

const wchar_t*  GAME_OVER_TEXT[]       =
{
    L" _____   ___  ___  ___ _____   _____  _   _  _____ ______ ",
    L"|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | ||  ___|| ___ \\",
    L"| |  \\// /_\\ \\| .  . || |__   | | | || | | || |__  | |_/ /",
    L"| | __ |  _  || |\\/| ||  __|  | | | || | | ||  __| |    / ",
    L"| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ /| |___ | |\\ \\ ",
    L"\\_____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/ \\____/ \\_| \\_|"
};
const short     GAME_OVER_TEXT_LEN     = 58;
const short     GAME_OVER_TEXT_HEIGHT  = 6;

WindowHandler* WindowHandler::instance = nullptr;


WindowHandler::WindowHandler() :
    gameWindow({ nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0 })
{}


WindowHandler::~WindowHandler() 
{
    for (unsigned short i = 0; i < gameWindow.height; ++i) {
        delete[] gameWindow.characters[i];
        delete[] gameWindow.oldCharacters[i];
        delete[] gameWindow.colors[i];
        delete[] gameWindow.oldColors[i];
    }
    delete[] gameWindow.characters;
    delete[] gameWindow.oldCharacters;
    delete[] gameWindow.colors;
    delete[] gameWindow.oldColors;
}


WindowHandler* WindowHandler::i() 
{
    if (!instance)
        instance = new WindowHandler();
    return instance;
}


void WindowHandler::initWindow() 
{
    // Setting the font
    //setFont(L"Consolas", 16, 16, FW_BOLD);

    // Setting to fullscreen
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleDisplayMode(console, CONSOLE_FULLSCREEN_MODE, 0);

    // Windows console set to output UTF-8
    _setmode(_fileno(stdout), _O_U8TEXT);
    
    // Setting the console buffer size and draw gameWindow size
    bufferSize = GetLargestConsoleWindowSize(console);

    // Setting size to minimal so we can resize the buffer smaller than the gameWindow
    // (Otherwise it doesn't let us)
    SetConsoleDisplayMode(console, CONSOLE_WINDOWED_MODE, 0);
    SMALL_RECT const minimal_window = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(console, TRUE, &minimal_window);
    screenSize = bufferSize;
    bufferSize.X -= 2;    // tiny fix to account for the scrollbar on the right

    // Setting the desired console buffer size
    int Status = SetConsoleScreenBufferSize(console, bufferSize);
    SMALL_RECT const maximal_window = { 0, 0, bufferSize.X, bufferSize.Y };
    SetConsoleWindowInfo(console, TRUE, &maximal_window);
    SetConsoleDisplayMode(console, CONSOLE_FULLSCREEN_MODE, 0);
    
    // MAGIC NUMBERSSSS
    bufferSize.X -= 6;    //    Tiny adjustments so we dont write outside
    bufferSize.Y -= 10;    //    of the console buffer and cause scrollbars
    
    // Clamping to min/max size
    bufferSize.X = std::max(MIN_WIDTH, std::min(bufferSize.X, MAX_WIDTH));
    bufferSize.Y = std::max(MIN_HEIGHT, std::min(bufferSize.Y, MAX_HEIGHT));

    // Hide the cursor
    showCursor(false);
}


void WindowHandler::printGameLogo() const 
{
    const wchar_t** logo = GameEngine::gameLogo;
    unsigned char logoLength = wcslen(*logo);
    unsigned char leftOffset = screenSize.X / 2 - logoLength / 2 + 1;

    // Setting the color
    // MAGIC COLOR (NUMBERSSS)
    setColor(C_DK_RED);
    for (unsigned char i = 2; *logo; ++i) {
        SetConsoleCursorPosition(console, {leftOffset, i});
        std::wcout << *logo;
        ++logo;
    }
    setColor(C_WHITE);
}


void WindowHandler::initMainMenu() const 
{
    // (Ignore all the magic numbers)

    // Writing out the Main menu options
    unsigned char currY = screenSize.Y / 2;
    const wchar_t selection1[] = L"Start a new game";
    SetConsoleCursorPosition(console, { screenSize.X / 5 * 2 - (short)wcslen(selection1) / 2, currY });
    std::wcout << selection1;
    currY += 2;
    // If there is a save file add a Load option
    if (GameEngine::i()->saveFileExists()) {
        const wchar_t selection2[] = L"Load last saved game";
        SetConsoleCursorPosition(console, { screenSize.X / 5 * 2 - (short)wcslen(selection2) / 2, currY });
        std::wcout << selection2;
        currY += 2;
    }
    const wchar_t selection3[] = L"Quit game";
    SetConsoleCursorPosition(console, { screenSize.X / 5 * 2 - (short)wcslen(selection3) / 2, currY });
    std::wcout << selection3;
    

    // If there isn't a save file (we assume it's a new player so we print out the Controls)
    if (!GameEngine::i()->saveFileExists()) {
        wchar_t tips1[] = L" - CONTROLS - ";
        SetConsoleCursorPosition(console, { screenSize.X / 5 * 4 - (short)wcslen(tips1) / 2, screenSize.Y / 2 - 1 });
        std::wcout << tips1;
        wchar_t tips2[] = L"WASD to move";
        SetConsoleCursorPosition(console, { screenSize.X / 5 * 4 - (short)wcslen(tips2) / 2, screenSize.Y / 2 });
        std::wcout << tips2;
        wchar_t tips3[] = L"[Space] to shoot";
        SetConsoleCursorPosition(console, { screenSize.X / 5 * 4 - (short)wcslen(tips3) / 2, screenSize.Y / 2 + 1 });
        std::wcout << tips3;
        wchar_t tips4[] = L"[Enter] to select";
        SetConsoleCursorPosition(console, { screenSize.X / 5 * 4 - (short)wcslen(tips4) / 2, screenSize.Y / 2 + 2 });
        std::wcout << tips4;
        wchar_t tips5[] = L"[Esc] to pause";
        SetConsoleCursorPosition(console, { screenSize.X / 5 * 4 - (short)wcslen(tips5) / 2, screenSize.Y / 2 + 3 });
        std::wcout << tips5;
    }
}


void WindowHandler::drawMainMenuSelector(unsigned char sel) const 
{
    // Clear out all the spaces where the previous selector could have been
    for (unsigned char i = 0; i < 6; i+=2) {
        SetConsoleCursorPosition(console, { screenSize.X / 5, screenSize.Y / 2 + i });
        std::wcout << L' ';
    }

    // Setting the place where the selector will be drawn
    if (sel != 0 && !GameEngine::i()->saveFileExists())
        SetConsoleCursorPosition(console, { screenSize.X / 5, screenSize.Y / 2 + sel * 2 - 2 });
    else 
        SetConsoleCursorPosition(console, { screenSize.X / 5, screenSize.Y / 2 + sel * 2 });

    // Drawing the selector
    std::wcout << PAUSE_MENU_SELECTOR;
}


void WindowHandler::clearScreen() const 
{
    for (unsigned char i = 0; i < screenSize.Y; ++i)
        for (unsigned char j = 0; j < screenSize.X; ++j)
            std::wcout << L' ';
}


short WindowHandler::getGameWindowWidth() const { return gameWindow.width; }
short WindowHandler::getGameWindowHeight() const { return gameWindow.height; }


void WindowHandler::clearGameWindow() 
{
    for (int i = 0; i < gameWindow.height; ++i) {
        for (int j = 0; j < gameWindow.width; ++j) {
            gameWindow.characters[i][j] = ' ';
            gameWindow.colors[i][j] = C_WHITE;
        }
    }
}


void WindowHandler::drawGameWindow() const
{
    // Starting from (0, 0)
    //SetConsoleCursorPosition(console, { 0, 0 });
    COORD cursor;
    cursor.X = 0;
    cursor.Y = 0;
    
    for (short i = 0; i < gameWindow.height; ++i) {
        for (short j = 0; j < gameWindow.width; ++j) {
            // Only print the character if it's different than the previous frame's character
            // (Greatly increases performance)
            if (gameWindow.characters[i][j] != gameWindow.oldCharacters[i][j] ||
                gameWindow.colors[i][j] != gameWindow.oldColors[i][j])
            {
                // Copy the character to the previous screen
                gameWindow.oldColors[i][j] = gameWindow.colors[i][j];
                gameWindow.oldCharacters[i][j] = gameWindow.characters[i][j];

                // Calculating the console cursor position
                cursor.X = j + gameWindow.x;
                cursor.Y = i + gameWindow.y;
                
                // Setting the color
                setColor(gameWindow.colors[i][j]);

                // Setting the console cursor position
                SetConsoleCursorPosition(console, cursor);

                // Printing the character
                std::wcout << gameWindow.characters[i][j];
            }
        }
    }
    //setColor(C_WHITE);

    // Setting the cursor out of the gameWindow just incase something happens
    //SetConsoleCursorPosition(console, { gameWindow.x - 1, (short)(gameWindow.y + gameWindow.height + 1) });
}


void WindowHandler::drawStatus() const
{
    SetConsoleCursorPosition(console, { gameWindow.x + 6, gameWindow.y - TOP_GUI_OFFSET });
    setColor(C_RED);
    unsigned char lives = GameEngine::i()->getPlayer()->getLives();
    for (unsigned char i = 0; i < lives; ++i)
        std::wcout << HEART;
    std::wcout << L"  ";

    unsigned char health = GameEngine::i()->getPlayer()->getHealth();
    SetConsoleCursorPosition(console, { gameWindow.x + gameWindow.width - 3, gameWindow.y - TOP_GUI_OFFSET });
    for (unsigned char i = 0; i < health / 4; ++i)
        std::wcout << HEALTH[0];
    if (health % 4 != 0)
        std::wcout << HEALTH[health % 4];
    std::wcout << L"     ";
    setColor(C_WHITE);

    unsigned int score = GameEngine::i()->getPlayer()->getScore();
    SetConsoleCursorPosition(console, { gameWindow.x + gameWindow.width / 2, gameWindow.y - TOP_GUI_OFFSET });
    std::wcout << score << L"    ";
}


void WindowHandler::drawInfo(short speed, short calcTime) const
{
    setColor(C_DK_GRAY);
    SetConsoleCursorPosition(console, { gameWindow.x + 7, gameWindow.y + gameWindow.height + BOTTOM_GUI_OFFSET });
    std::wcout << speed << L"  ";
    SetConsoleCursorPosition(console, { gameWindow.x + 22, gameWindow.y + gameWindow.height + BOTTOM_GUI_OFFSET });
    std::wcout << calcTime << L"    ";
    setColor(C_WHITE);
}


void WindowHandler::drawGameOver() const 
{
    setColor(C_RED);
    for (unsigned char i = 0; i < 6; ++i) {
        SetConsoleCursorPosition(console, { gameWindow.x + gameWindow.width / 2 - (GAME_OVER_TEXT_LEN / 2), 
                                            gameWindow.y + gameWindow.height / 2 - GAME_OVER_TEXT_HEIGHT / 2 + i });
        std::wcout << GAME_OVER_TEXT[i];
    }
}


void WindowHandler::drawPauseMenu() 
{
    char text[] = "Pause Menu";
    unsigned short textLen = strlen(text);
    addTextToWindow(text, pauseMenu.x + PAUSE_MENU_WIDTH / 2 - textLen / 2, pauseMenu.y + 1);
    addTextToWindow("Resume", pauseMenu.x + 3, pauseMenu.y + 3);
    addTextToWindow("Save", pauseMenu.x + 3, pauseMenu.y + 5);
    addTextToWindow("Quit", pauseMenu.x + 3, pauseMenu.y + 7);
    drawGameWindow();
    drawMenuBorder();
}


void WindowHandler::clearPauseMenu() 
{
    for (unsigned char i = pauseMenu.y - 1; i < pauseMenu.y + PAUSE_MENU_HEIGHT + 1; ++i) {
        for (unsigned char j = pauseMenu.x - 1; j <= pauseMenu.x + PAUSE_MENU_WIDTH + 1; ++j) {
            gameWindow.characters[i][j] = ' ';
            gameWindow.oldCharacters[i][j] = ' ';
            setColorAt(j, i, C_WHITE);
            SetConsoleCursorPosition(console, { gameWindow.x + j, gameWindow.y + i });
            std::wcout << L' ';
        }
    }
}


void WindowHandler::drawPauseMenuSelector(unsigned short sel) const {
    for (unsigned char i = pauseMenu.y + 3; i < pauseMenu.y + PAUSE_MENU_HEIGHT; i += 2) {
        SetConsoleCursorPosition(console, { gameWindow.x + pauseMenu.x + 1, gameWindow.y + i });
        std::wcout << L' ';
    }
    SetConsoleCursorPosition(console, { gameWindow.x + pauseMenu.x + 1, gameWindow.y + pauseMenu.y + 3 + 2 * sel });
    std::wcout << PAUSE_MENU_SELECTOR;
}


void WindowHandler::showPauseMenuMessage(const char* text, unsigned time)
{
    // Clear out the place where the pause menu is
    clearPauseMenu();

    unsigned textLen = strlen(text);
    addTextToWindow(text, pauseMenu.x + PAUSE_MENU_WIDTH / 2 - textLen / 2, pauseMenu.y + 1);

    drawGameWindow();
    drawMenuBorder();
    
    // Dispay it for a certain amount of time
    Sleep(time);

    // Clear the place again
    clearPauseMenu();

    // And then redraw the pause menu
    drawPauseMenu();
    drawPauseMenuSelector(1);
    drawGameWindow();
    drawMenuBorder();
}


char WindowHandler::lookAt(short x, short y) const 
{ 
    if (x >= 0 && y >= 0 && x < gameWindow.width && y < gameWindow.height)
        return gameWindow.characters[y][x];
    else
        return ' ';
}


void WindowHandler::writeAt(short x, short y, char ch) const 
{
    if (x >= 0 && y >= 0 && x < gameWindow.width && y < gameWindow.height)
        gameWindow.characters[y][x] = ch; 
}

void WindowHandler::setColorAt(short x, short y, Color c) 
{
    if (x >= 0 && y >= 0 && x < gameWindow.width && y < gameWindow.height)
        gameWindow.colors[y][x] = c;
}


void WindowHandler::destroySelf() 
{ 
    delete instance;
    instance = nullptr;
}


void WindowHandler::drawGUI() const 
{
    SetConsoleCursorPosition(console, { gameWindow.x - 1, gameWindow.y - TOP_GUI_OFFSET });
    std::wcout << L"Lives:";
    const wchar_t txt[] = L"Health: ";
    short lastPos = gameWindow.x + gameWindow.width - 3 - (short)wcslen(txt);
    SetConsoleCursorPosition(console, { lastPos, gameWindow.y - TOP_GUI_OFFSET });
    std::wcout << txt;
    SetConsoleCursorPosition(console, { gameWindow.x, gameWindow.y + gameWindow.height + BOTTOM_GUI_OFFSET });
    setColor(C_DK_GRAY);
    std::wcout << L"Speed: ";
    SetConsoleCursorPosition(console, { gameWindow.x + 12, gameWindow.y + gameWindow.height + BOTTOM_GUI_OFFSET });
    std::wcout << L"calcTime: ";
    setColor(C_WHITE);
}


void WindowHandler::setColor(int color) const
{
        SetConsoleTextAttribute(console, color);
}


void WindowHandler::drawMenuBorder() const 
{
    for (unsigned char i = pauseMenu.x - 1; i <= pauseMenu.x + PAUSE_MENU_WIDTH + 1; ++i) {
        SetConsoleCursorPosition(console, { gameWindow.x + i, gameWindow.y + pauseMenu.y - 1 });
        std::wcout << BORDER_CHAR;
        SetConsoleCursorPosition(console, { gameWindow.x + i, gameWindow.y + pauseMenu.y + PAUSE_MENU_HEIGHT });
        std::wcout << BORDER_CHAR;
    }

    for (unsigned char i = pauseMenu.y; i < pauseMenu.y + PAUSE_MENU_HEIGHT; ++i) {
        SetConsoleCursorPosition(console, { gameWindow.x + pauseMenu.x - 1, gameWindow.y + i });
        std::wcout << BORDER_CHAR;
        SetConsoleCursorPosition(console, { gameWindow.x + pauseMenu.x + PAUSE_MENU_WIDTH + 1, gameWindow.y + i });
        std::wcout << BORDER_CHAR;
    }
}


void WindowHandler::addTextToWindow(const char* str, unsigned char xPosInWindow, unsigned char yPosInWindow) 
{
    unsigned char lenOfStr = strlen(str);
    for (unsigned char i = 0; i < lenOfStr; ++i)
        gameWindow.characters[yPosInWindow][xPosInWindow + i] = str[i];
}


void WindowHandler::setupGameWindow() 
{
    // Calculating the game gameWindow coords
    short temp_window_x = std::ceil(std::max(MIN_GAME_WINDOW_X, (short)((screenSize.X - bufferSize.X - 2) / 2))) + 1;
    short temp_window_y = std::ceil(std::max(MIN_GAME_WINDOW_Y, (short)((screenSize.Y - bufferSize.Y - 2) / 2))) + 1;

    // Setting up the game gameWindow struct
    gameWindow = { nullptr, nullptr, nullptr, nullptr, temp_window_x, temp_window_y, bufferSize.X, bufferSize.Y };

    // Pause gameWindow setting up
    pauseMenu.x = gameWindow.width / 2 - PAUSE_MENU_WIDTH / 2;
    pauseMenu.y = gameWindow.height / 2 - PAUSE_MENU_HEIGHT / 2;

    // Drawing the game gameWindow border
    drawGameWindowBorder();

    // Drawing the GUI
    drawGUI();
    SetConsoleCursorPosition(console, { (short)(screenSize.X / 2 - wcslen(GameEngine::gameName) / 2), 0 });
    std::wcout << GameEngine::gameName;
    setColor(C_LT_GRAY);
    SetConsoleCursorPosition(console, { 0, screenSize.Y - 1 });
    std::wcout << L"Game Window Size: " << gameWindow.width << L"x" << gameWindow.height << L" UTF8";
    setColor(C_WHITE);

    // Creating and filling up the matrices with spaces
    gameWindow.characters = new (std::nothrow) char*[gameWindow.height];
    gameWindow.oldCharacters = new (std::nothrow) char*[gameWindow.height];
    if (!gameWindow.characters || !gameWindow.oldCharacters) {
        std::wcerr << "Not enough memory to create game gameWindow!\n";
        GameEngine::i()->triggerGameStop();
        return;
    }
    
    for (short i = 0; i < gameWindow.height; ++i) {
        gameWindow.characters[i] = new char[gameWindow.width + 1];
        gameWindow.oldCharacters[i] = new char[gameWindow.width + 1];
        if (!gameWindow.characters[i] || !gameWindow.oldCharacters[i]) {
            std::wcerr << "Not enough memory to create game gameWindow!\n";
            GameEngine::i()->triggerGameStop();
            return;
        }
        gameWindow.characters[i][gameWindow.width] = '\0';
        gameWindow.oldCharacters[i][gameWindow.width] = '\0';
        for (short j = 0; j < gameWindow.width; ++j) {
            gameWindow.characters[i][j] = ' ';
            gameWindow.oldCharacters[i][j] = ' ';
        }
    }

    // Creating and filling up the color matrices with white
    gameWindow.colors = new (std::nothrow) char*[gameWindow.height];
    gameWindow.oldColors = new (std::nothrow) char*[gameWindow.height];
    if (!gameWindow.colors || !gameWindow.oldColors) {
        std::wcerr << "Not enough memory to create game gameWindow!\n";
        GameEngine::i()->triggerGameStop();
        return;
    }

    for (short i = 0; i < gameWindow.height; ++i) {
        gameWindow.colors[i] = new char[gameWindow.width + 1];
        gameWindow.oldColors[i] = new char[gameWindow.width + 1];
        if (!gameWindow.colors[i] || !gameWindow.oldColors[i]) {
            std::wcerr << "Not enough memory to create game gameWindow!\n";
            GameEngine::i()->triggerGameStop();
            return;
        }
        gameWindow.colors[i][gameWindow.width] = '\0';
        gameWindow.oldColors[i][gameWindow.width] = '\0';
        for (short j = 0; j < gameWindow.width; ++j) {
            gameWindow.colors[i][j] = C_WHITE;
            gameWindow.oldColors[i][j] = C_WHITE;
        }
    }
}


void WindowHandler::drawGameWindowBorder() const
{
    setColor(C_LT_GRAY);
    SetConsoleCursorPosition(console, { gameWindow.x, gameWindow.y - 1 });
    for (short i = 0; i <= gameWindow.width; ++i)
        std::wcout << BORDER_CHAR;
    for (short i = std::max(0, gameWindow.y - 1); i < gameWindow.y + gameWindow.height + 1; i++) {
        SetConsoleCursorPosition(console, { gameWindow.x - 1, i });
        std::wcout << BORDER_CHAR;
        SetConsoleCursorPosition(console, { (short)(gameWindow.x + gameWindow.width), i });
        std::wcout << BORDER_CHAR;
    }
    SetConsoleCursorPosition(console, { gameWindow.x, (short)(gameWindow.y + gameWindow.height) });
    for (short i = 0; i <= gameWindow.width; ++i)
        std::wcout << BORDER_CHAR;
    setColor(C_WHITE);
}


void WindowHandler::setFont(const wchar_t * fontName, short width, short height, unsigned fontWeight) 
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 16;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = fontWeight;
    wcscpy(cfi.FaceName, fontName);
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}


void WindowHandler::showCursor(bool visible) 
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(console, &cursorInfo);
}
