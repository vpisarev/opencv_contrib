// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Tencent is pleased to support the open source community by making WeChat QRCode available.
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

#include <opencv2/wechat_qrcode.hpp>
int main(int argc, char* argv[]) {
    cout << endl << argv[0] << endl << endl;
    cout << "A demo program of WeChat QRCode Detector: " << endl;

    Mat img;
    if (argc > 1) {
        img = imread(argv[1]);
    } else {
        cout << "    Usage: " << argv[0] << " <input_image>" << endl;
        return 0;
    }
    // The model is downloaded to ${CMAKE_BINARY_DIR}/modules/wechat_qrcode if cmake runs without warnings,
    // otherwise you can download them from https://github.com/WeChatCV/opencv_3rdparty/tree/wechat_qrcode.
    auto detector = wechat_qrcode::WeChatQRCode("detect.prototxt", "detect.caffemodel",
                                                "sr.prototxt", "sr.caffemodel");
    vector<Mat> points;
    auto res = detector.detectAndDecode(img, points);
    for (const auto& t : res) cout << t << endl;
    return 0;
}
