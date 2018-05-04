QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Infinity-Translator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Folders (using folder structure conventions)
INCLUDEDIR = $$PWD/include
LOGICDIR = $$PWD/logic
UIDIR = $$PWD/ui
UIFORMSDIR = $$UIDIR/forms
BUILDDIR = $$PWD/../build

SOURCES += \
        $$files(./*.cpp, true) #\
#	$$files($$UIDIR/*.cpp) \
#	$$files($$LOGICDIR/*.cpp)

HEADERS += \
        $$files($$INCLUDEDIR/*.h)

FORMS += \
        $$files($$UIFORMSDIR/*.ui)

INCLUDEPATH += $$INCLUDEDIR

# Build dir
DESTDIR = $$BUILDDIR
OBJECTS_DIR = $$BUILDDIR

RESOURCES += \
    res.qrc
