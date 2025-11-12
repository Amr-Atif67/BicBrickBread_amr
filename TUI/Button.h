#ifdef _WIN32
#pragma once
#include <vector>
#include <windows.h>
#include <string>
#include <algorithm>
#include "Component.h"

namespace TUI {

    /// @brief Console button component with listener support
    class Button : public Component {
    public:
        /// @brief Interface for objects that listen to button clicks.
        class Listener {
        public:
            virtual ~Listener() = default;
            virtual void buttonClicked(Button* button) = 0;
        };

        /// @brief Identifiers for setting colors.
        enum ID : unsigned char { LABEL, BUTTON, HOVER_BUTTON, HOVER_LABEL, CLICK_BUTTON, CLICK_LABEL };

        /// @brief Constructor
        Button(int nx = 0, int ny = 0, int w = 1, int h = 1,
               const std::string& lbl = "",
               std::string buttonColor = "", std::string labelColor = "", bool visible = true);

        // -------------------------
        // Label & Color
        // -------------------------
        void setButtonText(const std::string& s);
        const std::string& getButtonText() const { return label; }
        void setColor(ID id, std::string color);

        // -------------------------
        // Painting
        // -------------------------
        void paint(Graphics& g) override;

        // -------------------------
        // Listener Management
        // -------------------------
        void addListener(Listener* listener);
        void removeListener(Listener* listener);

    protected:
        // -------------------------
        // Component Event Handlers
        // -------------------------
        void onClicked() override;
        void mouseEnter(const MouseEvent& e) override;
        void mouseExit(const MouseEvent& e) override;
        void mouseDown(const MouseEvent& e) override;
        void mouseUp(const MouseEvent& e) override;

    private:
        std::string label;               ///< Text displayed on the button
        std::string labelColor;                 ///< Text color
        std::string buttonColor;                ///< Background color
        std::string hoverButtonColor;           ///< Background color when hovered
        std::string hoverLabelColor;            ///< Text color when hovered
        std::string clickButtonColor;           ///< Background color when clicked
        std::string clickLabelColor;            ///< Text color when clicked
        std::vector<Listener*> listeners;///< Registered click listeners
        bool mouseOverPrev;              ///< Tracks hover state
        bool mousePressed;               ///< Tracks click state
        bool clickHandled = false;

        void validateLabel(const std::string& s);
    };

} // namespace TUI
#endif // Windows