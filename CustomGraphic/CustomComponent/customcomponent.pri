
# 添加此行代码才可以将GraphicControl相关文件和.Pro的文件联系起来
INCLUDEPATH += $$PWD/GraphicControl \

HEADERS += \
    $$PWD/GraphicControl/graphicitem.h \
    $$PWD/GraphicControl/graphicpoint.h \
    $$PWD/GraphicControl/graphicscene.h

SOURCES += \
    $$PWD/GraphicControl/graphicitem.cpp \
    $$PWD/GraphicControl/graphicpoint.cpp \
    $$PWD/GraphicControl/graphicscene.cpp
	
