set(the_description "WeChat QR code Detector")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
ocv_define_module(wechat_qrcode opencv_core opencv_imgproc opencv_dnn WRAP java objc python js)
