#pragma once

class KeyProcessor {
public:
    enum class Mode {
        NORMAL,
        INVERT,
        BLUR,
        CANNY,
        SOBEL,
        BINARY,
        GLITCH,
        PICTURE_IN_PICTURE
        FACE
    };

    KeyProcessor();
    // повертає false якщо треба вийти
    bool processKey(int key);

    Mode getMode() const;
    double getBrightness() const;
    int getCrosshairX() const;
    int getCrosshairY() const;
    double getZoom() const;
    int getRotation() const;

private:
    Mode currentMode;
    double brightness;  // 0.5 .. 2.0
    int crossX, crossY;
    double zoom;
    int rotation;
};