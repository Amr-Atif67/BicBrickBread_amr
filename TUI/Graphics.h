#ifdef _WIN32
#pragma once
#include <windows.h>
#include <string>

using namespace std;


namespace TUI {
    /**
     * @brief Graphics class for TUI drawing in Windows console.
     */
    class Graphics {
    private:
        HANDLE hConsole;
    
        /**
         * @brief Map a string color name to a Windows console attribute.
         * @param color Color name (e.g., "red", "green", "blue", "white")
         * @return WORD console color attribute
         */
        WORD getColorAttribute(const string& _color, bool isBackground = false) {
            string color = _color;
            for (auto& c : color) c = tolower(c);
            WORD attr = 0;

            if (color == "black") attr = 0;
            else if (color == "blue") attr = FOREGROUND_BLUE;
            else if (color == "green") attr = FOREGROUND_GREEN;
            else if (color == "red") attr = FOREGROUND_RED;
            else if (color == "cyan") attr = FOREGROUND_GREEN | FOREGROUND_BLUE;
            else if (color == "magenta") attr = FOREGROUND_RED | FOREGROUND_BLUE;
            else if (color == "yellow") attr = FOREGROUND_RED | FOREGROUND_GREEN;
            else if (color == "white") attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

            if (isBackground) attr <<= 4;
            return attr;
        }
    
        /**
         * @brief Get current cursor position
         * @return COORD Current cursor position
         */
        COORD getCursorPosition() {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(hConsole, &csbi))
                return csbi.dwCursorPosition;
            return {0, 0};
        }
    
        /**
         * @brief Safely set cursor position
         * @param pos Position to set
         * @return true if success, false otherwise
         */
        bool safeSetCursor(const COORD& pos) {
            return SetConsoleCursorPosition(hConsole, pos) != 0;
        }
    
        /**
         * @brief Safely write text to console
         * @param text Text to write
         * @return true if success, false otherwise
         */
        bool safeWrite(const string& text) {
            DWORD written;
            return WriteConsoleA(hConsole, text.c_str(), (DWORD)text.size(), &written, nullptr) != 0;
        }
    
    public:
        Graphics() {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    
        /**
         * @brief Fill the whole console with a color
         * @param color Color name
         */
        void fillAll(const string& color) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    
            DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
            COORD topLeft = {0, 0};
            DWORD written;
    
            WORD attr = getColorAttribute(color, true);
            SetConsoleTextAttribute(hConsole, attr);
            FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &written);
            FillConsoleOutputAttribute(hConsole, attr, consoleSize, topLeft, &written);
            safeSetCursor(topLeft);
        }
    
        /**
         * @brief Fill a rectangular area with a color
         * @param x X coordinates
         * @param y Y coordinates
         * @param w Width of the rectangle
         * @param h Height of the rectangle
         * @param color Color name
         */
        void fillRect(int x, int y, int w, int h, const string& bgColor, const string& fgColor = "white") {
            COORD oldPos = getCursorPosition();
            DWORD written;
            WORD attr = getColorAttribute(fgColor) | getColorAttribute(bgColor, true);
            SetConsoleTextAttribute(hConsole, attr);
            COORD pos;
            for (int row = 0; row < h; ++row) {
                pos = { (SHORT)x, (SHORT)(y + row) };
                safeSetCursor(pos);
                FillConsoleOutputCharacter(hConsole, ' ', w, pos, &written);
                FillConsoleOutputAttribute(hConsole, attr, w, pos, &written);
            }
            safeSetCursor(oldPos);
        }
    
        /**
         * @brief Set text color for subsequent drawing
         * @param color Color name
         */
        void setColour(const string& color) {
            SetConsoleTextAttribute(hConsole, getColorAttribute(color));
        }
    
        /**
         * @brief Draw a rectangle (border only) in console
         * @param x X-coordinate (column)
         * @param y Y-coordinate (row)
         * @param w Width
         * @param h Height
         */
        void drawRect(int x, int y, int w, int h) {
            COORD oldPos = getCursorPosition();
            DWORD written;
    
            // Top and bottom borders
            for (int i = 0; i < w; ++i) {
                safeSetCursor({ (SHORT)(x + i), (SHORT)y });
                safeWrite("-");
                safeSetCursor({ (SHORT)(x + i), (SHORT)(y + h - 1) });
                safeWrite("-");
            }
    
            // Left and right borders
            for (int i = 0; i < h; ++i) {
                safeSetCursor({ (SHORT)x, (SHORT)(y + i) });
                safeWrite("|");
                safeSetCursor({ (SHORT)(x + w - 1), (SHORT)(y + i) });
                safeWrite("|");
            }
    
            // Corners
            safeSetCursor({ (SHORT)x, (SHORT)y }); safeWrite("+");
            safeSetCursor({ (SHORT)(x + w - 1), (SHORT)y }); safeWrite("+");
            safeSetCursor({ (SHORT)x, (SHORT)(y + h - 1) }); safeWrite("+");
            safeSetCursor({ (SHORT)(x + w - 1), (SHORT)(y + h - 1) }); safeWrite("+");
    
            safeSetCursor(oldPos);
        }
    
        /**
         * @brief Draw text at a position in console
         * @param text Text to draw
         * @param x Column position
         * @param y Row position
         */
        void drawText(const string& text, int x, int y) {
            COORD oldPos = getCursorPosition();
            safeSetCursor({ (SHORT)x, (SHORT)y });
            safeWrite(text);
            safeSetCursor(oldPos);
        }
    };
}

#endif