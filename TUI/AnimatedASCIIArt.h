#ifdef _WIN32
#pragma once
#include "ASCIIArt.h"
#include <vector>
#include <string>
#include <chrono>


namespace TUI {

    /**
     * @brief Animated ASCII Art component.
     * 
     * Displays multiple frames of ASCII art in sequence with optional looping.
     * Color is specified as a string to match the Graphics class (e.g., "red", "white").
     */
    class AnimatedASCIIArt : public ASCIIArt {
    private:
        std::vector<std::vector<std::string>> frames;   ///< All frames of animation
        size_t currentFrame;                            ///< Index of the current frame
        int frameDelayMs;                               ///< Delay between frames in milliseconds
        bool loop;                                      ///< Whether the animation loops
    
        std::chrono::steady_clock::time_point lastFrameTime;  ///< Last time the frame was updated
    
    public:
        /**
         * @brief Construct an AnimatedASCIIArt component.
         * @param nx X position in console
         * @param ny Y position in console
         * @param nframes Vector of frames (each frame is a vector of strings)
         * @param delay Delay between frames in milliseconds (default 100)
         * @param loopAnimation Whether to loop animation (default true)
         * @param ncolor Text color (string, e.g., "red")
         */
        AnimatedASCIIArt(int nx, int ny,
                         const std::vector<std::vector<std::string>>& nframes,
                         int delay = 100, bool loopAnimation = true, const std::string& ncolor = "white");
    
        /**
         * @brief Paint the current frame.
         * @param g Graphics context
         */
        void paint(Graphics& g) override;
    
        /**
         * @brief Advance to the next frame based on elapsed time.
         */
        void nextFrame();
    };

}
#endif // Windows