# This file is part of OpenCV project.
# It is subject to the license terms in the LICENSE file found in the top-level directory
# of this distribution and at http://opencv.org/license.html.
#
# Tencent is pleased to support the open source community by making WeChat QRCode available.
# Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.

import cv2
import sys
print(sys.argv[0])
print('A demo program of WeChat QRCode Detector:')
if len(sys.argv) > 1:
    img = cv2.imread(sys.argv[1])
else:
    print("    Usage: " + sys.argv[0] + "  <input_image>")
    exit(0)

# For python API generator, it follows the template: {module_name}_{class_name},
# so it is a little weird.
# The model is downloaded to ${CMAKE_BINARY_DIR}/downloads/wechat_qrcode if cmake runs without warnings,
# otherwise you can download them from https://github.com/WeChatCV/opencv_3rdparty/tree/wechat_qrcode.
detector = cv2.wechat_qrcode_WeChatQRCode(
    "detect.prototxt", "detect.caffemodel", "sr.prototxt", "sr.caffemodel")

ret_str, points = detector.detectAndDecode(img)
print(ret_str)
