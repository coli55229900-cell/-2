#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "MouseHandler.hpp"
#include "Display.hpp"

int main() {
    CameraProvider camera(0);
    KeyProcessor   keys;
    FrameProcessor processor("../assets/overlay.png");
    MouseHandler   mouse;
    Display        display("Camera App");

    // трекбари
    cv::createTrackbar("Blur",       display.getName(),
                       &FrameProcessor::blurValue,      30);
    cv::createTrackbar("Brightness", display.getName(),
                       &FrameProcessor::brightnessTrack, 200);

    cv::setMouseCallback(display.getName(),
                         MouseHandler::callback, &mouse);

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        cv::Mat result = processor.process(
            frame,
            keys.getMode(),
            keys.getBrightness(),
            keys.getCrosshairX(),
            keys.getCrosshairY(),
            keys.getRotation(),
            mouse.getZoom()   // зум мишкою
        );

        mouse.drawOnFrame(result);
        display.show(result);

        int key = cv::waitKey(1);
        if (key != -1 && !keys.processKey(key)) break;
    }

    cv::destroyAllWindows();
    return 0;
}