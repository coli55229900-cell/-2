#include "MouseHandler.hpp"

MouseHandler::MouseHandler() : drawing(false), zoom(1.0) {}

void MouseHandler::callback(int event, int x, int y, int flags, void* userdata) {
    static_cast<MouseHandler*>(userdata)->handle(event, x, y, flags);
}

void MouseHandler::handle(int event, int x, int y, int flags) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        drawing = true;
        startPt = {x, y};
    } else if (event == cv::EVENT_LBUTTONUP && drawing) {
        drawing = false;
        lines.push_back({startPt, {x, y}});
    } else if (event == cv::EVENT_MOUSEWHEEL) {
        zoom += (flags > 0) ? 0.1 : -0.1;
        zoom = std::max(0.5, std::min(zoom, 3.0));
    }
}

void MouseHandler::drawOnFrame(cv::Mat& frame) {
    for (auto& l : lines)
        cv::line(frame, l.first, l.second, {0, 255, 0}, 2);
    if (drawing)
        cv::line(frame, startPt, startPt, {0, 0, 255}, 4); // точка
}

double MouseHandler::getZoom() const { return zoom; }