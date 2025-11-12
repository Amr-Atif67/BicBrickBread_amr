#include "Desktop.h"
#include <thread>
#include <chrono>
#include <algorithm>

namespace TUI {

    void Desktop::run() {
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD count;
        INPUT_RECORD record;

        // Hide cursor
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);

        // Enable mouse input
        DWORD prevMode;
        GetConsoleMode(hInput, &prevMode);
        SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT);

        while (true) {
            // Process input if available
            if (PeekConsoleInput(hInput, &record, 1, &count) && count > 0) {
                ReadConsoleInput(hInput, &record, 1, &count);

                if (record.EventType == MOUSE_EVENT) {
                    auto& me = record.Event.MouseEvent;
                    int mx = me.dwMousePosition.X;
                    int my = me.dwMousePosition.Y;
                    bool leftDown = (me.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;

                    // Find the topmost component under the mouse
                    Component* topComp = nullptr;
                    for (auto it = components.rbegin(); it != components.rend(); ++it) {
                        if ((*it)->isVisible() && (*it)->isMouseOver(mx, my)) {
                            topComp = *it;
                            break;
                        }
                    }

                    // Update each component based on mouse events
                    for (auto* comp : components) {
                        if (!comp->isVisible()) continue;
                        bool over = comp == topComp;

                        // Handle mouse enter/exit events
                        if (over && !comp->mouseOverPrev) {
                            comp->mouseEnter({ mx, my, leftDown, false });
                            comp->markForRepaint();
                        }
                        if (!over && comp->mouseOverPrev) {
                            comp->mouseExit({ mx, my, leftDown, false });
                            comp->markForRepaint();
                        }

                        // Handle mouse press
                        if (over && leftDown && !comp->mousePressed) {
                            comp->mousePressed = true;
                            comp->mouseDown({ mx, my, leftDown, false });
                            comp->markForRepaint();
                        }
                        // Handle mouse release
                        else if (!leftDown && comp->mousePressed) {
                            comp->mousePressed = false;
                            comp->mouseUp({ mx, my, leftDown, false });
                            comp->markForRepaint();
                        }

                        comp->mouseOverPrev = over;

                        // Repaint component if it is dirty
                        if (comp->isDirty()) comp->repaint();
                    }
                }
                // Handle keyboard events
                else if (record.EventType == KEY_EVENT) {
                    auto& ke = record.Event.KeyEvent;
                    if (ke.bKeyDown) {
                        for (auto* comp : components) {
                            if (!comp->isVisible()) continue;
                            comp->keyPressed(KeyPress{ ke.wVirtualKeyCode });
                            if (comp->isDirty()) comp->repaint();
                        }
                    }
                }
            }

            // Force repaint all components (animations, etc.)
            for (auto* comp : components) {
                if (comp->isVisible() && comp->isDirty()) {
                    comp->repaint();
                }
            }

            // Small delay to avoid 100% CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

} // namespace TUI