#ifdef _WIN32
#pragma once
#include "Component.h"
#include <vector>
#include <windows.h>

namespace TUI {

    /**
     * @brief Desktop class that manages GUI components and handles input events.
     * 
     * It handles mouse and keyboard input, determines which component is on top
     * under the cursor, and manages repainting of components. It is the main
     * "container" of all components.
     */
    class Desktop {
    private:
        std::vector<Component*> components; ///< All components added to the desktop

    public:

        ~Desktop() {
            for(auto& i : components) {
                delete i;
            }
        }
        /**
         * @brief Add a component to the desktop
         * @param c Pointer to a Component
         */
        void addComponent(Component* c) { components.push_back(c); }

        /**
         * @brief Remove a component from the desktop
         * @param c Pointer to a Component
         */
        void removeComponent(Component* c) {
            components.erase(std::remove(components.begin(), components.end(), c), components.end());
        }

        /**
         * @brief Start the main event loop of the desktop
         * 
         * This function:
         * - Processes mouse and keyboard input
         * - Detects hover, press, and release events for components
         * - Forces all components to repaint every cycle (good for animations)
         */
        void run(); // Declaration only; implemented in Desktop.cpp
    };

} // namespace TUI

#endif // Windows