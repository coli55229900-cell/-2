#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>

class FaceDetector {
public:
    FaceDetector(const std::string& prototxt, const std::string& caffemodel);
    ~FaceDetector();

    // Головний потік кладе сюди новий кадр
    void setFrame(const cv::Mat& frame);

    // Головний потік забирає звідси останні прямокутники облич
    std::vector<cv::Rect> getFaces() const;

    // Чи ввімкнена детекція
    bool isEnabled() const;
    void setEnabled(bool val);

private:
    void workerLoop();  // крутиться у фоновому потоці

    cv::dnn::Net net;

    mutable std::mutex mutex;       // захищає inputFrame і faces
    cv::Mat inputFrame;             // спільний кадр (main → worker)
    std::vector<cv::Rect> faces;    // спільний результат (worker → main)

    std::thread workerThread;
    std::atomic<bool> running;
    std::atomic<bool> enabled;
    std::atomic<bool> hasNewFrame;  // прапорець: є новий кадр для обробки
};