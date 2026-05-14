#include "KeyProcessor.hpp"

KeyProcessor::KeyProcessor()
    : currentMode(Mode::NORMAL), brightness(1.0),
      crossX(320), crossY(240), zoom(1.0), rotation(0) {}

bool KeyProcessor::processKey(int key) {
    switch (key) {
        case 27:  return false;        // ESC — вихід
        case '1': currentMode = Mode::NORMAL;  break;
        case '2': currentMode = Mode::INVERT;  break;
        case '3': currentMode = Mode::BLUR;    break;
        case '4': currentMode = Mode::CANNY;   break;
        case '5': currentMode = Mode::SOBEL;   break;
        case '6': currentMode = Mode::BINARY;  break;
        case '7': currentMode = Mode::GLITCH;  break;
        case '8': currentMode = Mode::PICTURE_IN_PICTURE; break;
        // стрілки
        case 82: crossY -= 5; rotation += 5; break; // вгору
        case 84: crossY += 5; rotation -= 5; break; // вниз
        case 81: crossX -= 5; break;                // вліво
        case 83: crossX += 5; break;                // вправо
        // яскравість
        case '+': brightness = std::min(brightness + 0.1, 2.0); break;
        case '-': brightness = std::max(brightness - 0.1, 0.1); break;
        case 'f': currentMode = Mode::FACE; break;  // клавіша F
    }
    return true;
}

KeyProcessor::Mode KeyProcessor::getMode()       const { return currentMode; }
double             KeyProcessor::getBrightness()  const { return brightness; }
int                KeyProcessor::getCrosshairX()  const { return crossX; }
int                KeyProcessor::getCrosshairY()  const { return crossY; }
double             KeyProcessor::getZoom()        const { return zoom; }
int                KeyProcessor::getRotation()    const { return rotation; }