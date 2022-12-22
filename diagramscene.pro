QT += widgets
requires(qtConfig(fontcombobox))

HEADERS	    =   mainwindow.h \
		datatypes.h \
		diagramitem.h \
		diagramscene.h \
		arrow.h \
                pugixml.hpp \
                pugiconfig.hpp \
		diagramtextitem.h
SOURCES	    =   mainwindow.cpp \
    datatypes.cpp \
		diagramitem.cpp \
		main.cpp \
		arrow.cpp \
                pugixml.cpp \
		diagramtextitem.cpp \
		diagramscene.cpp
RESOURCES   =	diagramscene.qrc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target
