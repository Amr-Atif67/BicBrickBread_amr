#ifdef _WIN32
#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "Graphics.h"

using namespace std;

namespace TUI {
    /**
     * @brief Rectangle structure representing position and size.
     */
    struct Rectangle {
        int x, y, width, height;
        Rectangle(int nx = 0, int ny = 0, int w = 1, int h = 1)
            : x(nx), y(ny), width(w), height(h) {}
    };

    /**
     * @brief Structure representing a mouse event.
     */
    struct MouseEvent {
        int x, y;            ///< Mouse coordinates
        bool leftButtonDown; ///< Is left button pressed
        bool rightButtonDown;///< Is right button pressed
    };

    /**
     * @brief Structure representing a key press event.
     */
    struct KeyPress {
        int keyCode; ///< Key code of pressed key
    };

    /**
     * @brief Enum for focus change type.
     */
    enum FocusChangeType { focusChangedByMouse, focusChangedByKeyboard };

    // --------------------
    // Base Component Class
    // --------------------

    /**
     * @brief Base class for console GUI components (JUCE-style).
     * @details Represents a component with position, size, visibility, and child management.
     */
    class Component {
    protected:
        int x, y;           ///< Top-left coordinates
        int width, height;  ///< Size of component
        bool visible;       ///< Visibility flag
        bool needsRepaint = true;

        vector<Component*> children; ///< Child components
        Component* parent = nullptr; ///< Parent component

        void validateSize(int w, int h) {
            if (w <= 0 || h <= 0) throw invalid_argument("Width and height must be positive.");
        }

        void validateCoordinates(int nx, int ny) {
            if (nx < 0 || ny < 0) throw invalid_argument("Coordinates must be non-negative.");
        }

    public:
        bool mouseOverPrev = false;  ///< Tracks whether the mouse was previously over this component
        bool mousePressed = false;   ///< Tracks whether the mouse was previously pressed over this component

        void markForRepaint() { needsRepaint = true; }
        void removeFromRepaint() { needsRepaint = false; }
        bool isDirty() const { return needsRepaint; }

        Component(int nx = 0, int ny = 0, int w = 1, int h = 1, bool v = true)
            : x(nx), y(ny), width(w), height(h), visible(v) {
            validateSize(w, h);
            validateCoordinates(nx, ny);
        }

        virtual ~Component() = default;

        void setBounds(int nx, int ny, int w, int h) {
            validateCoordinates(nx, ny);
            validateSize(w, h);
            x = nx; y = ny; width = w; height = h;
        }

        void setBounds(const Rectangle& r) { setBounds(r.x, r.y, r.width, r.height); }
        void setBoundsRelative(int dw, int dh) { resize(width + dw, height + dh); }
        void move(int nx, int ny) { setX(nx); setY(ny); }
        void resize(int w, int h) { setWidth(w); setHeight(h); }

        void setX(int nx) { validateCoordinates(nx, y); x = nx; }
        void setY(int ny) { validateCoordinates(x, ny); y = ny; }
        void setWidth(int w) { validateSize(w, height); width = w; }
        void setHeight(int h) { validateSize(width, h); height = h; }

        int getX() const { return x; }
        int getY() const { return y; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getRight() const { return x + width; }
        int getBottom() const { return y + height; }
        bool contains(int px, int py) const { return isMouseOver(px, py); }

        void setVisible(bool v) { visible = v; }
        bool isVisible() const { return visible; }

        void addAndMakeVisible(Component* child) {
            if (child && child != this) {
                child->parent = this;
                children.push_back(child);
                child->setVisible(true);
            }
        }

        void removeChild(Component* child) {
            if (!child) return;
            children.erase(remove(children.begin(), children.end(), child), children.end());
            child->parent = nullptr;
        }

        Component* getParent() const { return parent; }
        const vector<Component*>& getChildren() const { return children; }

        virtual void paint(Graphics& g) = 0;  ///< Must be implemented by derived classes
        virtual void onClicked() {}          ///< Implemented by derived classes

        virtual void repaint() {
            if (!visible) return;
            removeFromRepaint();
            Graphics g;
            paint(g);
            for (auto* child : children)
                if (child->isVisible()) child->repaint();
        }

        virtual void repaint(Component* c) {
            if (!visible) return;
            Graphics g;
            if (c && c->isVisible()) c->repaint();
        }

        virtual bool isMouseOver(int mx, int my) const {
            return visible && mx >= x && mx < x + width && my >= y && my < y + height;
        }

        virtual void mouseEnter(const MouseEvent& e) {}
        virtual void mouseExit(const MouseEvent& e) {}
        virtual void mouseDown(const MouseEvent& e) {}
        virtual void mouseUp(const MouseEvent& e) {}
        virtual void keyPressed(const KeyPress& key) {}
        virtual void focusGained(FocusChangeType cause) {}
        virtual void focusLost(FocusChangeType cause) {}
    };
}
#endif // end of class definition