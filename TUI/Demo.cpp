/**
 * TUI Demo Application
 *
 * This program demonstrates the various components available in the TUI library,
 * including labels, buttons, event listeners, static ASCII art,
 * animated ASCII art, and advanced 3D 'Box' components.
 */

#include "TUI.h"
#include <vector>
#include <string>

using namespace TUI;

// --- 1. Button Event Listener ---
/**
 * @class MyListener
 * @brief A simple listener to handle button click events.
 *
 * Inherits from Button::Listener and overrides the buttonClicked method
 * to define custom actions for different buttons.
 */
class MyListener : public Button::Listener {
public:
    /**
     * @brief Called when any button this listener is attached to is clicked.
     * @param btn A pointer to the button that was clicked.
     */
    void buttonClicked(Button* btn) override {
        if (btn->getButtonText() == "Exit") {
            // If the 'Exit' button is clicked, end the program.
            std::cout << u8"Exiting...\n";
            exit(0);
        } else {
            // For any other button, just print its text.
            std::cout << u8"Button clicked: " << btn->getButtonText() << "\n";
        }
    }
};

// --- 2. Main Application ---
int main() {
    // Enable UTF-8 support for the Windows console to show emoji.
    SetConsoleOutputCP(CP_UTF8);

    // The Desktop is the root container that manages and draws all components.
    Desktop desktop;

    // --- 3. Component Setup ---

    // ----- Add a Label -----
    // A simple, non-interactive text component.
    // Constructor: Label(x, y, text, color)
    Label* title = new Label(10, 1, "Welcome to TUI Demo!", "yellow");
    desktop.addComponent(title);

    // ----- Add Buttons -----
    // Interactive components that respond to clicks.
    // Constructor: Button(x, y, w, h, text, bgColor, fgColor)
    Button* btn1 = new Button(5, 5, 14, 3, "Click Me", "blue", "white");
    Button* btn2 = new Button(25, 5, 14, 3, "Exit", "red", "white");

    // Customize button colors for different states
    btn1->setColor(Button::HOVER_BUTTON, "yellow");
    btn1->setColor(Button::CLICK_BUTTON, "green");

    // Attach our listener to the buttons
    MyListener listener;
    btn1->addListener(&listener);
    btn2->addListener(&listener);

    desktop.addComponent(btn1);
    desktop.addComponent(btn2);

    // ----- Add Static ASCII Art -----
    // A component to display multi-line ASCII art.
    // Constructor: ASCIIArt(x, y, art_lines, color)
    std::vector<std::string> catArt = {
        " /\\_/\\ ",
        "( o.o )",
        " > ^ < "
    };
    ASCIIArt* cat = new ASCIIArt(50, 3, catArt, "magenta");
    desktop.addComponent(cat);

    // ----- Add Animated ASCII Art -----
    // A component that cycles through multiple "frames" of ASCII art.
    // Constructor: AnimatedASCIIArt(x, y, frames, frameTimeMs, loop, color)
    vector<std::vector<std::string>> frames = {
        {u8"🙂","<) 	)╯",u8" / 	\\"},
        {u8"😀","<) 	)╯",u8" / 	\\"},
        {u8"😃","<) 	)╯",u8" / 	\\"},
        {u8"😄","<) 	)╯",u8" / 	\\"},
        {u8"😁","<) 	)╯",u8" / 	\\"},
        {u8"😆","<) 	)╯",u8" / 	\\"},
        {u8"😁","<) 	)╯",u8" / 	\\"},
        {u8"😄","<) 	)╯",u8" / 	\\"},
        {u8"😃","<) 	)╯",u8" / 	\\"},
        {u8"😀","<) 	)╯",u8" / 	\\"},
    };
    AnimatedASCIIArt* anim = new AnimatedASCIIArt(50, 10, frames, 150, true, "cyan");
    desktop.addComponent(anim);

    // ----- Add 3D Spinning 'X' -----
    // This demonstrates layering two 'Box' components.
    // NOTE: This requires the Box::paint() method to have a "transparent"
    // background (i.e., not drawing a solid color for empty pixels).

    // --- 1. Define Model & Component properties ---
    const int plankLength = 50;     // The 3D length of the plank
    const int plankWidth = 7;       // The 3D width/thickness of the plank
    
    const int renderAreaWidth = 70; // The 2D component width on screen
    const int renderAreaHeight = 70;// The 2D component height on screen

    const double rotationAngle = 0.785398; // ~45 degrees in radians
    const double scale = 50.0;             // Zoom level for the 3D projection

    // --- 2. Define the CENTER position for the 'X' ---
    const int desired_center_x = 120;
    const int desired_center_y = 0;

    
    // --- 3. Create Plank 1 (\) ---
    // This component is created at (105, 0) and is 70x70.
    Box* diag1 = new Box(desired_center_x, desired_center_y, renderAreaWidth, renderAreaHeight);
    diag1->setCubeBounds(plankLength, plankWidth, plankWidth); // Make it a long, thin plank
    diag1->setRotation(0.35, 0.35, rotationAngle);             // Rotate +45 degrees
    diag1->setColor("cyan");
    diag1->setScale(scale);
    diag1->setSpinning(true);
    desktop.addComponent(diag1);

    // --- 4. Create Plank 2 (/) ---
    // We create the second plank at the *exact same position* to layer it.
    Box* diag2 = new Box(desired_center_x, desired_center_y, renderAreaWidth, renderAreaHeight);
    diag2->setCubeBounds(plankLength, plankWidth, plankWidth);
    diag2->setRotation(0.35, 0.35, -rotationAngle);            // Rotate -45 degrees
    diag2->setColor("magenta");
    diag2->setScale(scale);
    diag2->setSpinning(true);
    desktop.addComponent(diag2);

    // --- 4. Run the Application ---
    // This starts the main TUI loop, which handles input, updates, and rendering.
    desktop.run();

    return 0;
}