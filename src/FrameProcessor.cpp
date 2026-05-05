#include "FrameProcessor.hpp"
#include <chrono>

int FrameProcessor::blurValue = 5;
int FrameProcessor::brightnessTrack = 100; // 0..200, 100 = нейтрально

FrameProcessor::FrameProcessor(const std::string& overlayPath)
    : frameCount(0) {
    overlayImg = cv::imread(overlayPath, cv::IMREAD_UNCHANGED);
}

cv::Mat FrameProcessor::process(const cv::Mat& src,
    KeyProcessor::Mode mode, double brightness,
    int crossX, int crossY, int rotation, double zoom)
{
    frameCount++;
    cv::Mat frame = src.clone();

    // застосовуємо яскравість з трекбару
    double bTrack = brightnessTrack / 100.0;
    frame.convertTo(frame, -1, bTrack, 0);

    // зум і поворот
    frame = applyZoomRotation(frame, zoom, rotation);

    // режим
    switch (mode) {
        case KeyProcessor::Mode::INVERT:
            cv::bitwise_not(frame, frame);
            break;
        case KeyProcessor::Mode::BLUR: {
            int k = blurValue % 2 == 0 ? blurValue + 1 : blurValue;
            if (k < 1) k = 1;
            cv::GaussianBlur(frame, frame, {k, k}, 0);
            break;
        }
        case KeyProcessor::Mode::CANNY: {
            cv::Mat gray, edges;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, edges, 50, 150);
            cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
            break;
        }
        case KeyProcessor::Mode::SOBEL: {
            cv::Mat gray, gx, gy, abs_gx, abs_gy;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, gx, CV_16S, 1, 0);
            cv::Sobel(gray, gy, CV_16S, 0, 1);
            cv::convertScaleAbs(gx, abs_gx);
            cv::convertScaleAbs(gy, abs_gy);
            cv::addWeighted(abs_gx, 0.5, abs_gy, 0.5, 0, gray);
            cv::cvtColor(gray, frame, cv::COLOR_GRAY2BGR);
            break;
        }
        case KeyProcessor::Mode::BINARY: {
            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::threshold(gray, gray, 127, 255, cv::THRESH_BINARY);
            cv::cvtColor(gray, frame, cv::COLOR_GRAY2BGR);
            break;
        }
        case KeyProcessor::Mode::GLITCH: {
            std::vector<cv::Mat> ch;
            cv::split(frame, ch);
            int shift = 15;
            // зсуваємо канали
            cv::Mat tx = (cv::Mat_<double>(2,3) << 1,0,shift, 0,1,0);
            cv::warpAffine(ch[2], ch[2], tx, frame.size()); // R вправо
            tx.at<double>(0,2) = -shift;
            cv::warpAffine(ch[0], ch[0], tx, frame.size()); // B вліво
            cv::merge(ch, frame);
            break;
        }
        case KeyProcessor::Mode::PICTURE_IN_PICTURE:
            drawOverlay(frame);
            break;
        default: break;
    }

    drawCrosshair(frame, crossX, crossY);
    drawStats(frame, frameCount);
    return frame;
}

cv::Mat FrameProcessor::applyZoomRotation(const cv::Mat& f, double zoom, int rot) {
    cv::Point2f center(f.cols / 2.0f, f.rows / 2.0f);
    cv::Mat M = cv::getRotationMatrix2D(center, rot, zoom);
    cv::Mat out;
    cv::warpAffine(f, out, M, f.size());
    return out;
}

void FrameProcessor::drawCrosshair(cv::Mat& f, int x, int y) {
    x = std::max(0, std::min(x, f.cols));
    y = std::max(0, std::min(y, f.rows));
    cv::drawMarker(f, {x, y}, {0, 255, 255},
                   cv::MARKER_CROSS, 30, 2);
}

void FrameProcessor::drawOverlay(cv::Mat& f) {
    if (overlayImg.empty()) return;
    int w = f.cols / 4, h = f.rows / 4;
    cv::Mat resized;
    cv::resize(overlayImg, resized, {w, h});
    cv::Rect roi(f.cols - w - 10, 10, w, h);
    if (resized.channels() == 4) {
        std::vector<cv::Mat> ch;
        cv::split(resized, ch);
        cv::Mat alpha = ch[3];
        cv::Mat rgb;
        ch.pop_back();
        cv::merge(ch, rgb);
        rgb.copyTo(f(roi), alpha);
    } else {
        resized.copyTo(f(roi));
    }
}

void FrameProcessor::drawStats(cv::Mat& f, int count) {
    static auto t0 = std::chrono::steady_clock::now();
    static int fps = 0, fpsCounter = 0;
    fpsCounter++;
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - t0).count();
    if (elapsed >= 1.0) {
        fps = fpsCounter;
        fpsCounter = 0;
        t0 = now;
    }
    cv::Scalar mean = cv::mean(f);
    std::string info = "FPS: " + std::to_string(fps) +
                       "  Frame: " + std::to_string(count) +
                       "  Avg B/G/R: " +
                       std::to_string((int)mean[0]) + "/" +
                       std::to_string((int)mean[1]) + "/" +
                       std::to_string((int)mean[2]);
    cv::putText(f, info, {10, 25},
                cv::FONT_HERSHEY_SIMPLEX, 0.55, {0,255,0}, 1);
}