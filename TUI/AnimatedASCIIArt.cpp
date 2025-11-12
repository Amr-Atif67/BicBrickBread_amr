#include "AnimatedASCIIArt.h"
#include "Graphics.h"

namespace TUI {

    AnimatedASCIIArt::AnimatedASCIIArt(int nx, int ny,
                                       const std::vector<std::vector<std::string>>& nframes,
                                       int delay, bool loopAnimation, const std::string& ncolor)
        : ASCIIArt(nx, ny, nframes.empty() ? std::vector<std::string>{} : nframes[0], ncolor),
          frames(nframes),
          currentFrame(0),
          frameDelayMs(delay),
          loop(loopAnimation),
          lastFrameTime(std::chrono::steady_clock::now())
    {
        // Calculate width and height from first frame
        if (!frames.empty()) {
            width = 0;
            for (const auto& line : frames[0]) {
                if (static_cast<int>(line.size()) > width)
                    width = static_cast<int>(line.size());
            }
            height = static_cast<int>(frames[0].size());
        }
    }
    
    void AnimatedASCIIArt::nextFrame() {
        if (frames.empty()) return;
    
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime);
    
        if (elapsed.count() >= frameDelayMs) {
            currentFrame++;
            if (currentFrame >= frames.size()) {
                if (loop) currentFrame = 0;
                else currentFrame = frames.size() - 1; // stop at last frame
            }
            lastFrameTime = now;
            lines = frames[currentFrame];  // update the current frame in ASCIIArt
        }
    }
    
    void AnimatedASCIIArt::paint(Graphics& g) {
        nextFrame();        // advance frame if needed
        ASCIIArt::paint(g); // draw the current frame
        markForRepaint();
    }

}