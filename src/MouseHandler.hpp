#pragma once
#include <opencv2/opencv.hpp>

class MouseHandler {
public:
    MouseHandler();
    static void callback(int event, int x, int y, int flags, void* userdata);
    void drawOnFrame(cv::Mat& frame);

    double getZoom() const;

private:
    void handle(int event, int x, int y, int flags);

    bool drawing;
    cv::Point startPt, endPt;
    std::vector<std::pair<cv::Point,cv::Point>> lines;
    double zoom;
};