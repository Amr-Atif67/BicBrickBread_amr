#include "Button.h"
#include "Graphics.h"
#include <algorithm>

namespace TUI {

    Button::Button(int nx, int ny, int w, int h,
                   const std::string& lbl,
                   std::string btnColor, std::string lblColor, bool visible)
        : Component(nx, ny, w, h, visible),
          labelColor(lblColor),
          buttonColor(btnColor),
          hoverButtonColor(btnColor),
          hoverLabelColor(lblColor),
          clickButtonColor(btnColor),
          clickLabelColor(lblColor),
          mouseOverPrev(false),
          mousePressed(false)
    {
        validateLabel(lbl);
    }

    void Button::validateLabel(const std::string& s) {
        label = s.empty() ? "Button" : s;
    }

    void Button::setButtonText(const std::string& s) {
        validateLabel(s);
        markForRepaint(); 
    }

    void Button::setColor(ID id, std::string color) {
        switch (id) {
        case LABEL: labelColor = color; break;
        case BUTTON: buttonColor = color; break;
        case HOVER_BUTTON: hoverButtonColor = color; break;
        case HOVER_LABEL: hoverLabelColor = color; break;
        case CLICK_BUTTON: clickButtonColor = color; break;
        case CLICK_LABEL: clickLabelColor = color; break;
        default: break;
        }
        markForRepaint(); 
    }

    void Button::paint(Graphics& g) {
        if (!visible) return;

        std::string bg = buttonColor;
        std::string fg = labelColor;

        if (mousePressed) {
            bg = clickButtonColor;
            fg = clickLabelColor;
        } else if (mouseOverPrev) {
            bg = hoverButtonColor;
            fg = hoverLabelColor;
        }

        g.fillRect(x, y, width, height, bg);

        // Center the text
        int textX = x + (width - static_cast<int>(label.size())) / 2;
        int textY = y + height / 2;
        g.setColour(fg);
        g.drawText(label, textX, textY);
    }

    void Button::addListener(Listener* listener) {
        if (listener)
            listeners.push_back(listener);
    }

    void Button::removeListener(Listener* listener) {
        listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    }

    void Button::onClicked() {
        for (auto* listener : listeners)
            listener->buttonClicked(this);
    }

    void Button::mouseEnter(const MouseEvent& e) {
        mouseOverPrev = true;
        markForRepaint();
    }

    void Button::mouseExit(const MouseEvent& e) {
        mouseOverPrev = false;
        markForRepaint();
    }

    void Button::mouseDown(const MouseEvent& e) {
        if (e.leftButtonDown) {
            mousePressed = true;
            clickHandled = false;  // start a new click
            markForRepaint(); 
        }
    }

    void Button::mouseUp(const MouseEvent& e) {
        if (mousePressed) {
            mousePressed = false;
            if (!clickHandled && isMouseOver(e.x, e.y)) {
                onClicked();
                clickHandled = true;  // mark this click as handled
            }
            markForRepaint(); 
        }
    }

} // namespace TUI