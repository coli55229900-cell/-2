#!/bin/bash
set -e

echo "=== Встановлення залежностей ==="
sudo apt update
sudo apt install -y libopencv-dev cmake g++ make wget

echo "=== Завантаження моделей нейромережі ==="
mkdir -p models

wget -q --show-progress -O models/deploy.prototxt \
  "https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt"

wget -q --show-progress -O models/res10_300x300_ssd_iter_140000.caffemodel \
  "https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel"

echo "=== Готово! ==="