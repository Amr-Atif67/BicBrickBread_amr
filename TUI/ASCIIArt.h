#ifdef _WIN32
#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <windows.h>

namespace TUI {
    
    /**
     * @brief Component for displaying ASCII or UTF-8 art (supports emojis) in the console.
     *
     * Inherits from Component, so it can be added to Desktop, painted, and participate
     * in the GUI event loop. Each line of the art is stored as a string, and color can
     * be set using a WORD attribute (Windows console color code).
     */
    class ASCIIArt : public Component {
    protected:
        std::string color;                       ///< Foreground color (Windows console attribute)
        std::vector<std::string> lines;   ///< ASCII or UTF-8 text lines
    
        /**
         * @brief Converts UTF-8 string to UTF-16 (wstring) for Windows console output.
         * @param utf8 Input UTF-8 string
         * @return Converted UTF-16 string
         */
        static std::wstring utf8ToUtf16(const std::string& utf8);
    
    public:
        /**
         * @brief Construct a new ASCIIArt component.
         * @param nx X-coordinate on the console
         * @param ny Y-coordinate on the console
         * @param nlines Vector of text lines (ASCII or UTF-8)
         * @param ncolor Foreground color (default 7 = white)
         */
        ASCIIArt(int nx, int ny, const std::vector<std::string>& nlines, string ncolor = "");
    
        /**
         * @brief Set the foreground color for the art.
         * @param ncolor Windows console color code (WORD)
         */
        void setColor(string ncolor);
    
        /**
         * @brief Update the ASCII/UTF-8 lines of the art.
         * Updates width and height automatically.
         * @param nlines New lines
         */
        void setLines(const std::vector<std::string>& nlines);
    
        /**
         * @brief Paint the ASCII art at its current position.
         * Uses the Graphics object to draw each line.
         * Overrides Component::paint.
         * @param g Graphics object for drawing
         */
        void paint(Graphics& g) override;
    };

}
#endif // Windows