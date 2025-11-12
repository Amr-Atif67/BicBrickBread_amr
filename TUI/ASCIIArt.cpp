#include "ASCIIArt.h"
#include "Graphics.h"
#include <iostream>
#include <locale>
#include <codecvt>

namespace TUI {

    ASCIIArt::ASCIIArt(int nx, int ny, const std::vector<std::string>& nlines, string ncolor)
        : Component(nx, ny, 1, 1, true), color(ncolor), lines(nlines)
    {
        // Set width to the longest line and height to the number of lines
        width = 0;
        for (const auto& line : lines) {
            if (static_cast<int>(line.size()) > width)
                width = static_cast<int>(line.size());
        }
        height = static_cast<int>(lines.size());
    }
    
    /**
     * Convert UTF-8 string to UTF-16
     */
    std::wstring ASCIIArt::utf8ToUtf16(const std::string& utf8) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.from_bytes(utf8);
    }
    
    /**
     * Set the color
     */
    void ASCIIArt::setColor(string ncolor) {
        color = ncolor;
        markForRepaint();
    }
    
    /**
     * Set the lines
     */
    void ASCIIArt::setLines(const std::vector<std::string>& nlines) {
        lines = nlines;
    
        // Update width and height
        width = 0;
        for (const auto& line : lines) {
            if (static_cast<int>(line.size()) > width)
                width = static_cast<int>(line.size());
        }
        height = static_cast<int>(lines.size());
    
        markForRepaint();
    }
    
    /**
     * Paint the ASCIIArt component
     */
    void ASCIIArt::paint(Graphics& g) {
        if (!visible) return;
    
        for (int row = 0; row < static_cast<int>(lines.size()); ++row) {
            int drawY = y + row;
    
            // Draw each line at (x, drawY)
            g.setColour(color);
            g.drawText(lines[row], x, drawY);
        }
    }

}