TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


QMAKE_LFLAGS+=-Wl,--rpath=/root/source/opencv-3.2.0/build/__install/lib
INCLUDEPATH+=/root/source/opencv-3.2.0/build/__install/include
LIBS+=-L/root/source/opencv-3.2.0/build/__install/lib -lopencv_core -lopencv_video -lopencv_videoio -lopencv_imgcodecs  -lopencv_imgproc
LIBS+=-L/root/source/ffmpeg-3.4.5/__install/lib -lswresample -lavcodec -lavformat -lavutil
QMAKE_CXXFLAGS+=-I/root/source/ffmpeg-3.4.5/__install/include
