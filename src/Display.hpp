// Display.hpp
#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class Display {
public:
    Display(const std::string& windowName);
    void show(const cv::Mat& frame);
    const std::string& getName() const;
private:
    std::string name;
};