#include "FaceDetector.hpp"
#include <chrono>

FaceDetector::FaceDetector(const std::string& prototxt,
                           const std::string& caffemodel)
    : running(true), enabled(false), hasNewFrame(false)
{
    // Завантажуємо нейромережу
    net = cv::dnn::readNetFromCaffe(prototxt, caffemodel);

    // Запускаємо фоновий потік
    workerThread = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    if (workerThread.joinable())
        workerThread.join();
}

void FaceDetector::setFrame(const cv::Mat& frame) {
    if (!enabled) return;
    std::lock_guard<std::mutex> lock(mutex);
    inputFrame = frame.clone();
    hasNewFrame = true;
}

std::vector<cv::Rect> FaceDetector::getFaces() const {
    std::lock_guard<std::mutex> lock(mutex);
    return faces;
}

bool FaceDetector::isEnabled() const { return enabled; }
void FaceDetector::setEnabled(bool val) {
    enabled = val;
    if (!val) {
        // очищаємо рамки коли режим вимкнено
        std::lock_guard<std::mutex> lock(mutex);
        faces.clear();
    }
}

void FaceDetector::workerLoop() {
    while (running) {
        cv::Mat frame;

        // Перевіряємо чи є новий кадр
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (hasNewFrame && !inputFrame.empty()) {
                frame = inputFrame.clone();
                hasNewFrame = false;
            }
        }

        // Якщо нема нового кадру або детекція вимкнена — чекаємо
        if (frame.empty() || !enabled) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // ======= ІНФЕРЕНС НЕЙРОМЕРЕЖІ =======

        // Щоб показати різницю між потоками — можна увімкнути затримку:
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        cv::Mat blob = cv::dnn::blobFromImage(
            frame, 1.0, cv::Size(300, 300),
            cv::Scalar(104.0, 177.0, 123.0)
        );
        net.setInput(blob);
        cv::Mat detections = net.forward();

        // detections має форму [1, 1, N, 7]
        // кожен рядок: [_, _, confidence, x1, y1, x2, y2]
        cv::Mat det = detections.reshape(1, detections.total() / 7);

        std::vector<cv::Rect> found;
        int W = frame.cols, H = frame.rows;

        for (int i = 0; i < det.rows; i++) {
            float confidence = det.at<float>(i, 2);
            if (confidence < 0.5f) continue;

            int x1 = (int)(det.at<float>(i, 3) * W);
            int y1 = (int)(det.at<float>(i, 4) * H);
            int x2 = (int)(det.at<float>(i, 5) * W);
            int y2 = (int)(det.at<float>(i, 6) * H);

            // Обрізаємо щоб не виходило за межі кадру
            x1 = std::max(0, x1); y1 = std::max(0, y1);
            x2 = std::min(W, x2); y2 = std::min(H, y2);

            if (x2 > x1 && y2 > y1)
                found.push_back(cv::Rect(x1, y1, x2 - x1, y2 - y1));
        }

        // Зберігаємо результат під м'ютексом
        {
            std::lock_guard<std::mutex> lock(mutex);
            faces = found;
        }
    }
}