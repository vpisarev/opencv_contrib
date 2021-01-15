// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Tencent is pleased to support the open source community by making WeChat QRCode available.
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Modified from ZXing. Copyright ZXing authors.
// Licensed under the Apache License, Version 2.0 (the "License").

#include "zxing/common/line_binarizer.hpp"
#include "zxing/common/illegal_argument_exception.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace zxing;


LineBinarizer::LineBinarizer(Ref<LuminanceSource> source) : GlobalHistogramBinarizer(source) {}

LineBinarizer::~LineBinarizer() {}

Ref<Binarizer> LineBinarizer::createBinarizer(Ref<LuminanceSource> source) {
    return Ref<Binarizer>(new LineBinarizer(source));
}

Ref<BitArray> LineBinarizer::getBlackRow(int y, Ref<BitArray> row, ErrorHandler &err_handler) {
    // First call binarize image in child class to get matrix0_ and binCache
    if (!matrix0_) {
        binarizeImage(err_handler);
        if (err_handler.ErrCode()) return Ref<BitArray>();
    }

    // Call parent getBlackMatrix to get current matrix
    return Binarizer::getBlackRow(y, row, err_handler);
}

Ref<BitMatrix> LineBinarizer::getBlackMatrix(ErrorHandler &err_handler) {
    // First call binarize image in child class to get matrix0_ and binCache
    // std::cerr<<"use second"<<std::endl;
    if (!matrix0_) {
        binarizeImage(err_handler);
        if (err_handler.ErrCode()) return Ref<BitMatrix>();
    }
    return Binarizer::getBlackMatrix(err_handler);
}

int LineBinarizer::binarizeImage(ErrorHandler &err_handler) {
    if (width >= 7 && height >= 7) {
        LuminanceSource &source = *getLuminanceSource();
        Ref<BitMatrix> matrix(new BitMatrix(width, height, err_handler));
        if (err_handler.ErrCode()) return -1;

        ArrayRef<char> localLuminances = source.getMatrix();

        unsigned char *src = (unsigned char *)localLuminances->data();
        unsigned char *dst = matrix->getPtr();
        binarizeImage(src, dst);
        matrix0_ = matrix;
    } else {
        matrix0_ = GlobalHistogramBinarizer::getBlackMatrix(err_handler);
        if (err_handler.ErrCode()) return 1;
    }
    return 0;
}

void LineBinarizer::binarizeImage(const unsigned char *src, unsigned char *dst) {
    cv::Mat src_img = cv::Mat(height, width, CV_8UC1, (void *)src);

    int x0 = 0;
    int y0 = int(height * 0.33 / 1.66);  // TODO: use DecodeTipInfo
    int y1 = int(height * 1.33 / 1.66);
    cv::Rect roi = cv::Rect(x0, y0, width, y1 - y0 + 1);

    cv::Mat src_roi = src_img(roi);
    cv::Mat norm_roi;
    cv::normalize(src_roi, norm_roi, 0, 255, cv::NORM_MINMAX);

    for (int j = 0; j < norm_roi.rows; j++) {
        uchar *src_line = norm_roi.ptr(j);
        uchar *dst_line = dst + (y0 + j) * width;
        binarizeLine(src_line, dst_line);
    }

    return;
}

bool LineBinarizer::binarizeLine(const unsigned char *line, unsigned char *dst) {
    std::vector<short> maxiam_index;
    std::vector<short> miniam_index;
    scanLine(line, maxiam_index, miniam_index);
    int len1 = maxiam_index.size();
    int len2 = miniam_index.size();
    if ((len1 + len2) < 40 || std::abs(len1 - len2) > 1) return false;
    std::vector<short> index_all;

    int i = 0;
    if (maxiam_index[0] < miniam_index[0]) {
        while (i < min(len1, len2)) {
            index_all.push_back(maxiam_index[i]);
            index_all.push_back(miniam_index[i]);
            i++;
        }
    } else {
        while (i < min(len1, len2)) {
            index_all.push_back(miniam_index[i]);
            index_all.push_back(maxiam_index[i]);
            i++;
        }
    }
    if (i < len1)
        index_all.push_back(maxiam_index[i]);
    else if (i < len2)
        index_all.push_back(miniam_index[i]);

    std::size_t n = 0;

    while (n < index_all.size()) {
        int start_i = 0;
        int end_i = index_all[n];
        int thresh;
        if (n == 0) {
            start_i = 0;
            thresh = int(line[end_i]) / 2;
        } else {
            start_i = index_all[n - 1];
            thresh = (int(line[start_i]) + int(line[end_i])) / 2;
            if (n == index_all.size() - 1) end_i = width;
        }
        for (i = start_i; i < end_i; i++) {
            dst[i] = line[i] < thresh;
        }
        n++;
    }

    return true;
}

void LineBinarizer::scanLine(const unsigned char *line, std::vector<short> &maxiam_index,
                             std::vector<short> &miniam_index) {
    short cur_max_i = 0;
    short cur_min_i = 0;
    short diff_thresh = 25;
    int flag = 0;
    for (int i = 0; i < width; ++i) {
        auto v = line[i];
        if (v > line[cur_max_i]) {
            cur_max_i = i;
            if (flag > 0) {
                maxiam_index.back() = cur_max_i;
                cur_min_i = cur_max_i;
                continue;
            }
        }
        if (v < line[cur_min_i]) {
            cur_min_i = i;
            if (flag < 0) {
                miniam_index.back() = cur_min_i;
                cur_max_i = cur_min_i;
                continue;
            }
        }
        if (line[cur_max_i] - line[cur_min_i] > diff_thresh) {
            if (flag == 0) {
                maxiam_index.push_back(cur_max_i);
                miniam_index.push_back(cur_min_i);
                if (cur_max_i > cur_min_i) {
                    flag = 1;
                    cur_min_i = cur_max_i;
                } else {
                    flag = -1;
                    cur_max_i = cur_min_i;
                }
            } else if (flag > 0 && cur_min_i > cur_max_i) {
                miniam_index.push_back(cur_min_i);
                flag = -1;
                cur_max_i = cur_min_i;
            } else if (flag < 0 && cur_max_i > cur_min_i) {
                maxiam_index.push_back(cur_max_i);
                flag = 1;
                cur_min_i = cur_max_i;
            }
        }
    }
    return;
}
