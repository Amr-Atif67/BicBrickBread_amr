#ifdef _WIN32
#pragma once
#include "Component.h"
#include <string>

namespace TUI {

    /**
     * @brief Label component for displaying text in the console UI.
     * @details Inherits from Component and uses Graphics for drawing.
     * Supports text color and repainting when text or color changes.
     */
    class Label : public Component {
    private:
        std::string text;       ///< Text displayed by the label
        std::string textColor;  ///< Text color (console color name, e.g., "red", "white")

    public:
        /**
         * @brief Construct a new Label.
         * @param nx X-coordinate of the top-left corner
         * @param ny Y-coordinate of the top-left corner
         * @param ntext Initial text
         * @param ncolor Text color (default "white")
         * @param visible Whether the label is initially visible (default true)
         */
        Label(int nx, int ny, const std::string& ntext, const std::string& ncolor = "white", bool visible = true);

        /**
         * @brief Set the label's text.
         * @param ntext New text
         * @details Automatically marks the component for repainting.
         */
        void setText(const std::string& ntext);

        /**
         * @brief Set the label's text color.
         * @param ncolor Color name (e.g., "red", "white")
         * @details Automatically marks the component for repainting.
         */
        void setColor(const std::string& ncolor);

        /**
         * @brief Paint the label on the console using Graphics.
         * @param g Graphics object
         * @details This overrides Component::paint and draws the text at the label's position.
         */
        void paint(Graphics& g) override;
    };

} // namespace TUI

#endif // Windows