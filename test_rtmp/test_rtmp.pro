TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS+=-lrtmp
H264FILE.path=$$OUT_PWD
H264FILE.files=cuc_ieschool.h264
INSTALLS+=H264FILE
