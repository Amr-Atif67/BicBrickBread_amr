#include "Label.h"
#include "Graphics.h"

namespace TUI {

    Label::Label(int nx, int ny, const std::string& ntext, const std::string& ncolor, bool visible)
        : Component(nx, ny, max(1, static_cast<int>(ntext.size())), 1, visible), text(ntext), textColor(ncolor)
    {
        markForRepaint();
    }

    void Label::setText(const std::string& ntext) {
        text = ntext;
        setWidth(static_cast<int>(text.size()));
        markForRepaint();
    }

    void Label::setColor(const std::string& ncolor) {
        textColor = ncolor;
        markForRepaint();
    }

    void Label::paint(Graphics& g) {
        if (!isVisible()) return;
        g.setColour(textColor);
        g.drawText(text, getX(), getY());
    }

} // namespace TUI