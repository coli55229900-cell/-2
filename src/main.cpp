#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "MouseHandler.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"   // ← новий include

int main() {
    CameraProvider camera(0);
    KeyProcessor   keys;
    FrameProcessor processor("../assets/overlay.png");
    MouseHandler   mouse;
    Display        display("Camera App");

    // Завантажуємо детектор облич
    FaceDetector faceDetector(
        "../models/deploy.prototxt",
        "../models/res10_300x300_ssd_iter_140000.caffemodel"
    );

    // Трекбари
    cv::createTrackbar("Blur",       display.getName(),
                       &FrameProcessor::blurValue,       30);
    cv::createTrackbar("Brightness", display.getName(),
                       &FrameProcessor::brightnessTrack, 200);

    cv::setMouseCallback(display.getName(),
                         MouseHandler::callback, &mouse);

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        // Вмикаємо/вимикаємо детектор залежно від режиму
        bool faceMode = (keys.getMode() == KeyProcessor::Mode::FACE);
        faceDetector.setEnabled(faceMode);

        // Відправляємо кадр у фоновий потік
        faceDetector.setFrame(frame);

        // Обробляємо кадр як раніше
        cv::Mat result = processor.process(
            frame,
            keys.getMode(),
            keys.getBrightness(),
            keys.getCrosshairX(),
            keys.getCrosshairY(),
            keys.getRotation(),
            mouse.getZoom()
        );

        // Малюємо рамки облич поверх кадру (беремо з фонового потоку)
        if (faceMode) {
            auto faces = faceDetector.getFaces();
            for (const auto& rect : faces) {
                cv::rectangle(result, rect, {0, 255, 0}, 2);
                cv::putText(result, "Face",
                            {rect.x, rect.y - 5},
                            cv::FONT_HERSHEY_SIMPLEX,
                            0.6, {0, 255, 0}, 2);
            }
            // Підпис що режим активний
            cv::putText(result, "FACE DETECTION ON",
                        {10, 55},
                        cv::FONT_HERSHEY_SIMPLEX,
                        0.7, {0, 255, 0}, 2);
        }

        mouse.drawOnFrame(result);
        display.show(result);

        int key = cv::waitKey(1);
        if (key != -1 && !keys.processKey(key)) break;
    }

    cv::destroyAllWindows();
    return 0;
}