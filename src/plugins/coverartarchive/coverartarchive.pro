SOURCES = ../shared2/QUMultilingualImageSource.cpp \
	../../QUStringSupport.cpp \
	../../QUSongSupport.cpp \
	QUCoverArtArchiveImageSource.cpp \
	../shared2/QURequestUrl.cpp \
	../shared2/QUHttpCollector.cpp \
	QUCoverArtArchiveCollector.cpp \
	QUCoverArtArchiveRequestUrl.cpp
HEADERS = ../shared2/QUMultilingualImageSource.h \
	../../QUStringSupport.h \
	../../QUSongSupport.h \
	QUCoverArtArchiveImageSource.h \
	../shared2/QURequestUrl.h \
	../shared2/QUHttpCollector.h \
	QUCoverArtArchiveCollector.h \
	QUCoverArtArchiveRequestUrl.h
OTHER_FILES += coverartarchive.json
INCLUDEPATH += . \
	.. \
	../.. \
	../../song \
	../shared2
TARGET = coverartarchive
TEMPLATE = lib
CONFIG += plugin
UI_DIR = ../ui
QT += xml \
	network \
	widgets

CONFIG(release, debug|release) {
	TARGET = coverartarchive
	DESTDIR = ../../../bin/release/plugins
	OBJECTS_DIR = ../tmp/coverartarchive/release
	MOC_DIR = ../tmp/coverartarchive/release
}

CONFIG(debug, debug|release) {
	TARGET = coverartarchive_debug
	DESTDIR = ../../../bin/debug/plugins
	OBJECTS_DIR = ../tmp/coverartarchive/debug
	MOC_DIR = ../tmp/coverartarchive/debug
}
