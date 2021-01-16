// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Tencent is pleased to support the open source community by making WeChat QRCode available.
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Modified from ZXing. Copyright ZXing authors.
// Licensed under the Apache License, Version 2.0 (the "License").

#ifndef __ZXING_COMMON_GLOBAL_HISTOGRAM_BINARIZER_HPP__
#define __ZXING_COMMON_GLOBAL_HISTOGRAM_BINARIZER_HPP__

#include "../binarizer.hpp"
#include "../errorhandler.hpp"
#include "array.hpp"
#include "bitarray.hpp"
#include "bitmatrix.hpp"

#ifndef USE_GOOGLE_CODE
#define USE_GOOGLE_CODE 0
#endif
//#define INPUT_BINARIZED
namespace zxing {

class GlobalHistogramBinarizer : public Binarizer {
protected:
    ArrayRef<char> luminances;
    ArrayRef<int> buckets;

public:
    explicit GlobalHistogramBinarizer(Ref<LuminanceSource> source);
    virtual ~GlobalHistogramBinarizer();

    virtual Ref<BitArray> getBlackRow(int y, Ref<BitArray> row, ErrorHandler &err_handler) override;
    virtual Ref<BitMatrix> getBlackMatrix(ErrorHandler &err_handler) override;
    int estimateBlackPoint(ArrayRef<int> const &buckets, ErrorHandler &err_handler);
    int estimateBlackPoint2(ArrayRef<int> const &buckets);
    Ref<Binarizer> createBinarizer(Ref<LuminanceSource> source) override;

private:
    int binarizeImage0(ErrorHandler &err_handler);
    void initArrays(int luminanceSize);
    bool filtered;
};

}  // namespace zxing

#endif  // __ZXING_COMMON_GLOBAL_HISTOGRAM_BINARIZER_HPP__
