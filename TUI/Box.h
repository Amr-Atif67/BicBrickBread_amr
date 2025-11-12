#ifdef _WIN32
#pragma once

#include "Component.h"
#include <vector>
#include <cmath>
#include <string>
#include <thread>
#include <atomic>

namespace TUI {

    class Box : public Component {
        private:
            vector<vector<char>> output;                    ///< Stores the output chars
            vector<vector<double>> bufferZ;                 ///< Fake depth
            vector<std::vector<double>> lastFrameBuffer;    ///< Store last frame
            int distanceFromCam;                            ///< Distance of the camera
            double scale;                                   ///< Scale of the camera
            double cubeWidthX;                              ///< Width along X-axis (left-right faces)
            double cubeWidthY;                              ///< Width along Y-axis (top-bottom faces)
            double cubeWidthZ;                              ///< Width along Z-axis (front-back faces)
            double A;                                       ///< Theta of rotation about X-axis
            double B;                                       ///< Theta of rotation about Y-axis
            double C;                                       ///< Theta of rotation about Z-axis
            string color;                                   ///< Color of Box
            double intensity;                               ///< Intensity of the color
            double lightX;                                  ///< Vector of Light Source X-axis
            double lightY;                                  ///< Vector of Light Source Y-axis
            double lightZ;                                  ///< Vector of Light Source Z-axis
            bool spinning;                                  ///< Flag for spinning
            thread spinThread;                              ///< Thread for spinning
            atomic<bool> running;                           ///< Flag to stop the thread safely

            void normalizeVector(double& x, double& y, double& z);
            double calculateX(int i, int j, int k);
            double calculateY(int i, int j, int k);
            double calculateZ(int i, int j, int k);

        public:
            /**
             * @brief Initializing Box
             * @param nx X value of the top-left of render location
             * @param ny Y value of the top-left of render location
             * @param w Width of render window
             * @param h Height of render window
             */
            Box(int nx, int ny, int w, int h);

            /**
             * @brief Stopping the thread
             */
            ~Box();

            // --------- Setters ---------
            /**
             * @brief Set distance from camera
             * @param _distance Distance from camera
             */
            void setDistanceFromCam(int _distance);

            /**
             * @brief Set projection scale
             * @param _scale Scale factor for projection
             */
            void setScale(double _scale);

            /**
             * @brief Set width of cube along X-axis
             * @param _cubeWidthX Width along X-axis
             */
            void setCubeWidthX(double _cubeWidthX);

            /**
             * @brief Set width of cube along Y-axis
             * @param _cubeWidthY Width along Y-axis
             */
            void setCubeWidthY(double _cubeWidthY);

            /**
             * @brief Set width of cube along Z-axis
             * @param _cubeWidthZ Width along Z-axis
             */
            void setCubeWidthZ(double _cubeWidthZ);
            /**
             * @brief Set bounds of the box
             * @param _cubeWidthX Width along X-axis
             * @param _cubeWidthY Width along Y-axis
             * @param _cubeWidthZ Width along Z-axis
             */
            void setCubeBounds(double _cubeWidthX, double _cubeWidthY, double _cubeWidthZ);

            /**
             * @brief Set rotation angles
             * @param _A Theta of rotation about X-axis
             * @param _B Theta of rotation about Y-axis
             * @param _C Theta of rotation about Z-axis
             */
            void setRotation(double _A, double _B, double _C);

            /**
             * @brief Set color of the box
             * @param _color Color name (e.g., "red", "green", "blue", "white")
             */
            void setColor(std::string _color);

            /**
             * @brief Set color intensity
             * @param _intensity Intensity value (0.0 to 1.0)
             */
            void setIntensity(double _intensity);

            /**
             * @brief Set vector of light source
             * @param _lightX X component of light vector
             * @param _lightY Y component of light vector
             * @param _lightZ Z component of light vector
             */
            void setLight(double _lightX, double _lightY, double _lightZ);

            /**
             * @brief Set spinning flag
             * @param _spinning True if the box should rotate automatically, false otherwise
             */
            void setSpinning(bool _spinning);

            // --------- Getters ---------
            /**
             * @brief Get distance from camera
             * @return Distance from camera
             */
            int getDistanceFromCam() const;

            /**
             * @brief Get projection scale
             * @return Scale factor
             */
            double getScale() const;

            /**
             * @brief Get cube width along X-axis
             * @return Width along X-axis
             */
            double getCubeWidthX() const;

            /**
             * @brief Get cube width along Y-axis
             * @return Width along Y-axis
             */
            double getCubeWidthY() const;

            /**
             * @brief Get cube width along Z-axis
             * @return Width along Z-axis
             */
            double getCubeWidthZ() const;

            /**
             * @brief Get rotation angles
             * @param outA Output theta about X-axis
             * @param outB Output theta about Y-axis
             * @param outC Output theta about Z-axis
             */
            void getRotation(double& outA, double& outB, double& outC) const;

            /**
             * @brief Get color of the box
             * @return Color name
             */
            std::string getColor() const;

            /**
             * @brief Get color intensity
             * @return Intensity value
             */
            double getIntensity() const;

            /**
             * @brief Get light vector
             * @param outX Output X component
             * @param outY Output Y component
             * @param outZ Output Z component
             */
            void getLight(double& outX, double& outY, double& outZ) const;

            /**
             * @brief Get spinning flag
             * @return True if the box is spinning automatically, false otherwise
             */
            bool isSpinning() const;

            // ---------Paint----------
            void paint(Graphics& g) override;
    };

} // TUI

#endif // Windows