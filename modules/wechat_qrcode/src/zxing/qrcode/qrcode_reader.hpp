// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Tencent is pleased to support the open source community by making WeChat QRCode available.
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.

/*
 *  qrcode_reader.hpp
 *  zxing
 *
 *  Copyright 2010 ZXing authors All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ZXING_QRCODE_QRCODE_READER_HPP__
#define __ZXING_QRCODE_QRCODE_READER_HPP__

#include "zxing/decodehints.hpp"
#include "zxing/errorhandler.hpp"
#include "zxing/qrcode/decoder/decoder.hpp"
#include "zxing/qrcode/decoder/qrcode_decoder_metadata.hpp"
#include "zxing/qrcode/detector/detector.hpp"
#include "zxing/reader.hpp"

namespace zxing {
namespace qrcode {

struct QBAR_QRCODE_DETECT_INFO {
    int possibleFixIndex;
    unsigned int possibleAPType;

    // QRCodeReader Info
    float possibleFix;
    float patternPossibleFix;
    int pyramidLev;
    float possibleModuleSize;
    std::vector<Ref<ResultPoint> > qrcodeBorder;

    QBAR_QRCODE_DETECT_INFO() { clear(); }

    void clear() {
        possibleFixIndex = -1;
        possibleAPType = 0;
        possibleModuleSize = 0;

        possibleFix = 0;
        patternPossibleFix = 0;
        pyramidLev = 0;
        qrcodeBorder.clear();
    }
};

class QRCodeReader : public Reader {
public:
    enum ReaderState {
        READER_START = -1,
        DETECT_START = 0,
        DETECT_FINDFINDERPATTERN = 1,
        DETECT_FINDALIGNPATTERN = 2,
        DETECT_FAILD = 3,
        DECODE_START = 4,
        DECODE_READVERSION = 5,
        DECODE_READERRORCORRECTIONLEVEL = 6,
        DECODE_READCODEWORDSORRECTIONLEVEL = 7,
        DECODE_FINISH = 8
    };

private:
    Decoder decoder_;
    int detectedDimension_;
    ReaderState readerState_;
    DecodeHints nowHints_;

protected:
    Decoder& getDecoder();

public:
    QRCodeReader();
    virtual ~QRCodeReader();
    string name() override { return "qrcode"; }

    Ref<Result> decode(Ref<BinaryBitmap> image) override;
    Ref<Result> decode(Ref<BinaryBitmap> image, DecodeHints hints) override;

    Ref<Result> decodeMore(Ref<BinaryBitmap> image, Ref<BitMatrix> imageBitMatrix,
                           DecodeHints hints, ErrorHandler& err_handler);

private:
    QBAR_QRCODE_DETECT_INFO possibleQrcodeInfo_;

protected:
    void setPossibleAPCountByVersion(unsigned int version);
    int getRecommendedImageSizeTypeInteral();
    static void initIntegralOld(unsigned int* integral, Ref<BitMatrix> input);
    static void initIntegral(unsigned int* integral, Ref<BitMatrix> input);
    static int smooth(unsigned int* integral, Ref<BitMatrix> input, Ref<BitMatrix> output,
                      int window);
    unsigned int lastDecodeTime_;
    unsigned int lastDecodeID_;
    unsigned int decodeID_;
    int lastPossibleAPCount_;
    int possibleAPCount_;
    int possibleModuleSize_;
    unsigned int lastSamePossibleAPCountTimes_;
    unsigned int samePossibleAPCountTimes_;
    unsigned int lastRecommendedImageSizeType_;
    unsigned int recommendedImageSizeType_;
    unsigned int smoothMaxMultiple_;

public:
    virtual unsigned int getDecodeID() override;
    virtual void setDecodeID(unsigned int id) override;
    virtual float getPossibleFix() override;
    virtual unsigned int getPossibleAPType();
    virtual int getPossibleFixType();

    void setReaderState(Detector::DetectorState state);
    void setReaderState(Decoder::DecoderState state);

    void setPatternFix(float possibleFix);
    void setDecoderFix(float possibleFix, ArrayRef<Ref<ResultPoint> > border);
    void setSuccFix(ArrayRef<Ref<ResultPoint> > border);

    ReaderState getReaderState() { return this->readerState_; }
    float calQrcodeArea(Ref<DetectorResult> detectorResult);
    float calTriangleArea(Ref<ResultPoint> centerA, Ref<ResultPoint> centerB,
                          Ref<ResultPoint> centerC);
    // Ref<DetectorResult> getDetectorResult( Ref<BitMatrix> imageBitMatrix,
    // Ref<Detector> detector, int dimension,bool whiteAlignmen=false);

    int getQrcodeInfo(const void*& pQBarQrcodeInfo) override;

    vector<int> getPossibleDimentions(int detectDimension);
};

}  // namespace qrcode
}  // namespace zxing

#endif  // __ZXING_QRCODE_QRCODE_READER_HPP__
