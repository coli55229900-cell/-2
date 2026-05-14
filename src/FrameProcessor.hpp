#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    FrameProcessor(const std::string& overlayPath);
    cv::Mat process(const cv::Mat& frame,
                    KeyProcessor::Mode mode,
                    double brightness,
                    int crossX, int crossY,
                    int rotation,
                    double zoom);

    // трекбар
    static int blurValue;
    static int brightnessTrack;

private:
    cv::Mat applyZoomRotation(const cv::Mat& f, double zoom, int rot);
    void drawCrosshair(cv::Mat& f, int x, int y);
    void drawOverlay(cv::Mat& f);
    void drawStats(cv::Mat& f, int frameCount);

    cv::Mat overlayImg;
    int frameCount;
};