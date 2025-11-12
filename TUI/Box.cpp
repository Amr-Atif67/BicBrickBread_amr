#include "Box.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <thread>
using namespace TUI;

// -------------------- Constructor --------------------

Box::Box(int nx, int ny, int w, int h)
    : Component(nx, ny, w, h, true),
      distanceFromCam(300),
      scale(40.0),
      cubeWidthX(100),
      cubeWidthY(100),
      cubeWidthZ(100),
      A(0.0),
      B(0.0),
      C(0.0),
      color("blue"),
      intensity(1.0),
      lightX(0.0),
      lightY(0.0),
      lightZ(-1.0),
      spinning(false) // default not spinning
{
    output.resize(height, vector<char>(width, ' '));
    bufferZ.resize(height, vector<double>(width, 0.0));
    normalizeVector(lightX, lightY, lightZ);
    lastFrameBuffer.resize(height, std::vector<double>(width, 0));

    double minDim = min(width, height);
    cubeWidthX = cubeWidthY = cubeWidthZ = max(20.0, minDim * 0.35);
}

// -------------------- Destructor --------------------
Box::~Box() {
    running = false;
    if (spinThread.joinable())
        spinThread.join();
}

// -------------------- Private Helpers --------------------

void Box::normalizeVector(double& x, double& y, double& z) {
    double len = sqrt(x * x + y * y + z * z);
    if (len == 0.0) return;
    x /= len; y /= len; z /= len;
}

double Box::calculateX(int i, int j, int k) {
    double di = static_cast<double>(i);
    double dj = static_cast<double>(j);
    double dk = static_cast<double>(k);
    return dj * sin(A) * sin(B) * cos(C)
         - dk * cos(A) * sin(B) * cos(C)
         + dj * cos(A) * sin(C)
         + dk * sin(A) * sin(C)
         + di * cos(B) * cos(C);
}

double Box::calculateY(int i, int j, int k) {
    double di = static_cast<double>(i);
    double dj = static_cast<double>(j);
    double dk = static_cast<double>(k);
    return dj * cos(A) * cos(C)
         + dk * sin(A) * cos(C)
         - dj * sin(A) * sin(B) * sin(C)
         + dk * cos(A) * sin(B) * sin(C)
         - di * cos(B) * sin(C);
}

double Box::calculateZ(int i, int j, int k) {
    double di = static_cast<double>(i);
    double dj = static_cast<double>(j);
    double dk = static_cast<double>(k);
    return dk * cos(A) * cos(B) - dj * sin(A) * cos(B) + di * sin(B);
}

// -------------------- Setters --------------------

void Box::setDistanceFromCam(int _distance) { distanceFromCam = _distance; markForRepaint(); }
void Box::setScale(double _scale) { scale = _scale; }
void Box::setCubeWidthX(double _cubeWidthX) { cubeWidthX = _cubeWidthX; markForRepaint(); }
void Box::setCubeWidthY(double _cubeWidthY) { cubeWidthY = _cubeWidthY; markForRepaint(); }
void Box::setCubeWidthZ(double _cubeWidthZ) { cubeWidthZ = _cubeWidthZ; markForRepaint(); }
void Box::setCubeBounds(double _cubeWidthX, double _cubeWidthY, double _cubeWidthZ) { setCubeWidthX(_cubeWidthX), setCubeWidthY(_cubeWidthY), setCubeWidthZ(_cubeWidthZ); }
void Box::setRotation(double _A, double _B, double _C) { A = _A; B = _B; C = _C; markForRepaint(); }
void Box::setColor(string _color) { color = _color; markForRepaint(); }
void Box::setIntensity(double _intensity) { intensity = _intensity; markForRepaint(); }
void Box::setLight(double _lightX, double _lightY, double _lightZ) {
    lightX = _lightX; lightY = _lightY; lightZ = _lightZ;
    normalizeVector(lightX, lightY, lightZ);
    markForRepaint();
}
void Box::setSpinning(bool _spinning) {
    spinning = _spinning;

    if (spinning && !running.load()) {
        running = true;
        spinThread = std::thread([this]() {
            while (running) {
                A += 0.05;
                B += 0.05;
                C += 0.01;

                markForRepaint();

                this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });
    } else if (!spinning && running.load()) {
        running = false;
        if (spinThread.joinable())
            spinThread.join();
    }
}

// -------------------- Getters --------------------

int Box::getDistanceFromCam() const { return distanceFromCam; }
double Box::getScale() const { return scale; }
double Box::getCubeWidthX() const { return cubeWidthX; }
double Box::getCubeWidthY() const { return cubeWidthY; }
double Box::getCubeWidthZ() const { return cubeWidthZ; }
void Box::getRotation(double& outA, double& outB, double& outC) const { outA = A; outB = B; outC = C; }
string Box::getColor() const { return color; }
double Box::getIntensity() const { return intensity; }
void Box::getLight(double& outX, double& outY, double& outZ) const { outX = lightX; outY = lightY; outZ = lightZ; }
bool Box::isSpinning() const { return spinning; }

// -------------------- Paint --------------------

void Box::paint(Graphics& g) {
    // ANSI color palettes
    static const char* blueShades[] = { "\x1b[48;5;17m ", "\x1b[48;5;18m ", "\x1b[48;5;19m ",
        "\x1b[48;5;20m ", "\x1b[48;5;21m ", "\x1b[48;5;26m ", "\x1b[48;5;27m ",
        "\x1b[48;5;32m ", "\x1b[48;5;33m ", "\x1b[48;5;38m ", "\x1b[48;5;39m ", "\x1b[48;5;8m " };
    static const char* redShades[] = { "\x1b[48;5;52m ", "\x1b[48;5;88m ", "\x1b[48;5;124m ",
        "\x1b[48;5;160m ", "\x1b[48;5;196m ", "\x1b[48;5;202m ", "\x1b[48;5;203m ",
        "\x1b[48;5;209m ", "\x1b[48;5;210m ", "\x1b[48;5;211m ", "\x1b[48;5;215m ", "\x1b[48;5;8m " };
    static const char* greenShades[] = { "\x1b[48;5;22m ", "\x1b[48;5;28m ", "\x1b[48;5;34m ",
        "\x1b[48;5;40m ", "\x1b[48;5;46m ", "\x1b[48;5;82m ", "\x1b[48;5;83m ",
        "\x1b[48;5;119m ", "\x1b[48;5;120m ", "\x1b[48;5;154m ", "\x1b[48;5;155m ", "\x1b[48;5;8m " };
    static const char* whiteShades[] = { "\x1b[48;5;235m ", "\x1b[48;5;240m ", "\x1b[48;5;245m ",
        "\x1b[48;5;250m ", "\x1b[48;5;254m ", "\x1b[48;5;255m ", "\x1b[48;5;15m ",
        "\x1b[48;5;15m ", "\x1b[48;5;15m ", "\x1b[48;5;15m ", "\x1b[48;5;15m ", "\x1b[48;5;8m " };

    const char** palette = blueShades;
    if (color == "red") palette = redShades;
    else if (color == "green") palette = greenShades;
    else if (color == "white") palette = whiteShades;

    // Initialize buffers
    for (auto& row : bufferZ) fill(row.begin(), row.end(), -1e9); // depth buffer
    vector<vector<int>> colorBuffer(height, vector<int>(width, 0));

    auto calculateLightingIndex = [&](double nx, double ny, double nz) -> int {
        double intensityVal = nx * lightX + ny * lightY + nz * lightZ;
        if (intensityVal < 0.0) intensityVal = 0.0;
        if (intensityVal > 1.0) intensityVal = 1.0;
        int idx = static_cast<int>(intensityVal * 10.0 + 0.0001);
        if (idx < 0) idx = 0;
        if (idx > 10) idx = 10;
        return idx;
    };

    auto projectAndShade = [&](double cubeX, double cubeY, double cubeZ, double nx, double ny, double nz) {
        double rx = calculateX(cubeX, cubeY, cubeZ);
        double ry = calculateY(cubeX, cubeY, cubeZ);
        double rz = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;
        if (rz <= 0.0) return;
        double ooz = 1.0 / rz;

        int xp = static_cast<int>(width / 2.0 + scale * ooz * rx * 2.0);
        int yp = static_cast<int>(height / 2.0 + scale * ooz * ry);

        if (xp < 0 || xp >= width || yp < 0 || yp >= height) return;
        if (ooz <= bufferZ[yp][xp]) return;

        double rnx = calculateX(nx, ny, nz);
        double rny = calculateY(nx, ny, nz);
        double rnz = calculateZ(nx, ny, nz);
        normalizeVector(rnx, rny, rnz);

        int idx = calculateLightingIndex(rnx, rny, rnz);
        colorBuffer[yp][xp] = idx + 1;
        bufferZ[yp][xp] = ooz;
    };

    double step = max(0.5, min( cubeWidthX, min(cubeWidthY, cubeWidthZ)) / 40.0);

    for (double cx = -cubeWidthX; cx <= cubeWidthX; cx += step) {
        for (double cy = -cubeWidthY; cy <= cubeWidthY; cy += step) {
            projectAndShade(cx, cy, -cubeWidthZ, 0, 0, -1); // front
            projectAndShade(cx, cy, cubeWidthZ, 0, 0, 1);   // back
        }
    }
    for (double cz = -cubeWidthZ; cz <= cubeWidthZ; cz += step) {
        for (double cy = -cubeWidthY; cy <= cubeWidthY; cy += step) {
            projectAndShade(-cubeWidthX, cy, cz, -1, 0, 0); // left
            projectAndShade(cubeWidthX, cy, cz, 1, 0, 0);   // right
        }
    }
    for (double cx = -cubeWidthX; cx <= cubeWidthX; cx += step) {
        for (double cz = -cubeWidthZ; cz <= cubeWidthZ; cz += step) {
            projectAndShade(cx, -cubeWidthY, cz, 0, -1, 0); // bottom
            projectAndShade(cx, cubeWidthY, cz, 0, 1, 0);   // top
        }
    }

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int idx = colorBuffer[row][col];

            // Clear old pixel if necessary
            if (lastFrameBuffer[row][col] > 0 && idx == 0) {
                g.drawText("\x1b[49m ", x + col, y + row);
            }

            // Draw new pixel
            if (idx > 0 && idx <= 12) {
                g.drawText(palette[idx - 1], x + col, y + row);
            }

            lastFrameBuffer[row][col] = idx;
        }
    }
}