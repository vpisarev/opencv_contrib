// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Tencent is pleased to support the open source community by making WeChat QRCode available.
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Modified from ZXing. Copyright ZXing authors.
// Licensed under the Apache License, Version 2.0 (the "License").

#ifndef __ZXING_QRCODE_DETECTOR_ALIGNMENT_PATTERN_FINDER_HPP_
#define __ZXING_QRCODE_DETECTOR_ALIGNMENT_PATTERN_FINDER_HPP_

#include "../../common/bitmatrix.hpp"
#include "../../common/counted.hpp"
#include "../../errorhandler.hpp"
#include "alignment_pattern.hpp"
#include "finder_pattern.hpp"

#include "alignment_pattern.hpp"

#include <vector>

namespace zxing {
namespace qrcode {

class AlignmentPatternFinder : public Counted {
private:
    static int CENTER_QUORUM;
    static int MIN_SKIP;
    static int MAX_MODULES;

    Ref<BitMatrix> image_;
    std::vector<AlignmentPattern *> *possibleCenters_;

    int startX_;
    int startY_;
    int width_;
    int height_;
    float moduleSize_;

#ifdef FIND_WHITE_ALIGNMENTPATTERN
    std::vector<AlignmentPattern *> *possibleWhiteCenters_;
    Ref<AlignmentPattern> confirmedWhiteAlignmentPattern;
    Ref<AlignmentPattern> confirmedAlignmentPattern;
    bool foundAlignmentPattern;
    bool foundWhiteAlignmentPattern;
    bool lastTry;
#endif

    static float centerFromEnd(std::vector<int> &stateCount, int end);
    float crossCheckVertical(int startI, int centerJ, int maxCount, int originalStateCountTotal);
#ifdef FIND_WHITE_ALIGNMENTPATTERN
    static float centerFromEndWhite(std::vector<int> &stateCount, int end);
    float crossCheckVerticalWhite(int startI, int centerJ, int maxCount,
                                  int originalStateCountTotal);
#endif

public:
    AlignmentPatternFinder(Ref<BitMatrix> image, int startX, int startY, int width, int height,
                           float moduleSize);
    AlignmentPatternFinder(Ref<BitMatrix> image, float moduleSize);
    ~AlignmentPatternFinder();
#ifdef FIND_WHITE_ALIGNMENTPATTERN
    std::vector<Ref<AlignmentPattern> > find(ErrorHandler &err_handler);
    void setLastTry(bool toSet) { lastTry = toSet; }
    void foundPatternCross(std::vector<int> &stateCount);
    void handlePossibleCenter(std::vector<int> &stateCount, int i, int j);
#else
    Ref<AlignmentPattern> find(ErrorHandler &err_handler);
    bool foundPatternCross(std::vector<int> &stateCount);
    Ref<AlignmentPattern> handlePossibleCenter(std::vector<int> &stateCount, int i, int j);
#endif

private:
    AlignmentPatternFinder(const AlignmentPatternFinder &);
    AlignmentPatternFinder &operator=(const AlignmentPatternFinder &);

#ifdef USING_WX
private:
    std::vector<float> sinTable_;
    std::vector<float> cosTable_;
    float crossCheckVertical(size_t startI, size_t centerJ, int maxCount,
                             int originalStateCountTotal, int *state);
    float crossCheckHorizontal(size_t startI, size_t centerJ, int maxCount,
                               int originalStateCountTotal, int *state);
    static float centerFromEnd(int *stateCount, int end);
    bool foundPatternCross(int *stateCount);

public:
    AlignmentPatternFinder(Ref<BitMatrix> image, float moduleSize);
    void initAngleTable();
    bool findPatternLine(Ref<FinderPattern> const &from, Ref<FinderPattern> const &to,
                         double &aValue, double &pValue);
    bool findPatternLineSample(Ref<FinderPattern> const &from, Ref<FinderPattern> const &to,
                               std::vector<std::pair<int, int> > &sample);
    int findPatternLineCenterOneSide(int startX, int startY, int deltaX, bool steep, int side);
    int findPatternLineCenter(int startX, int startY, int deltaX, bool steep);
    bool findPatternLineStartPoint(Ref<FinderPattern> const &from, Ref<FinderPattern> const &to,
                                   int &startX, int &startY, bool &steep, int &xstep);
    void findPatternLineSampleOneSide(int startX, int startY, int endY, int deltaX, int steep,
                                      int side, std::vector<std::pair<int, int> > &sample);
    Ref<AlignmentPattern> findInRange(Ref<AlignmentPattern> const &estimateCenter, size_t startX,
                                      size_t startY, size_t width, size_t height,
                                      ErrorHandler &err_handler);
    void handlePossibleCenter(int *stateCount, size_t i, size_t j);
#endif
};
}  // namespace qrcode
}  // namespace zxing

#endif  // __ZXING_QRCODE_DETECTOR_ALIGNMENT_PATTERN_FINDER_HPP_
