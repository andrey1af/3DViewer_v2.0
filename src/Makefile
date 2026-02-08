CXX = g++
FLAGS = -Wall -Werror -Wextra -pedantic-errors -std=c++20 -fPIC -O3 -march=native
VALGRIND = valgrind --tool=memcheck --leak-check=yes
LEAKS = leaks -atExit --
TEST_FLAGS = -L/usr/local/lib -lgtest -pthread -L/usr/local/lib -lgtest_main
LCOV_FLAGS = -fprofile-arcs -ftest-coverage
DESKTOP_FLAGS = $(shell pkg-config --cflags --libs Qt5Widgets) -lGL

OS = $(shell uname)

ifeq ($(OS), Linux)
	CHECK_FLAGS = -lcheck -lm -lsubunit -lgcov
	LEAK_CHECKER = $(VALGRIND)
else
	CHECK_FLAGS = -lcheck -lm -lgcov
	LEAK_CHECKER = $(LEAKS)
endif

PREFIX ?= ~
BINDIR = $(PREFIX)/3DViewer
DISTDIR = 3DViewer_source
DOCFILE = 3DViewer.texi
TEXI2DVI = texi2dvi

QT_RESOURCES_FILE = ./gui/resources/resources.qrc
QT_RESOURCES = $(QT_RESOURCES_FILE:.qrc=.cpp)

HEADERS_FOR_MOC = ./gui/headers/menu.h ./gui/headers/settings.h  \
	./gui/headers/main_window.h ./gui/headers/view.h ./gui/headers/settings_panel.h \
	./gui/headers/model_menu.h ./gui/headers/drag_spinbox.h
MOC_SOURCES = $(HEADERS_FOR_MOC:.h=_moc.cpp)

MATRIX_LIBRARY = ./backend/libraries/s21_matrix_oop.a

BACKEND = $(wildcard ./backend/*/*.cpp) $(wildcard ./backend/model/*/*.cpp)
HEADERS_BACKEND = $(wildcard ./backend/headers/*.h)

FRONTEND = $(wildcard ./gui/src/*.cpp) $(wildcard ./gui/src/*/*.cpp)
HEADERS_FRONTEND = $(wildcard ./gui/headers/*.h)

FRONTEND_BUILD = $(FRONTEND) $(QT_RESOURCES) $(MOC_SOURCES)

TEST_FUNC = $(wildcard ./tests/*.cpp)

ALL_FUNC = $(BACKEND) $(FRONTEND)

ALL_HEADERS = $(HEADERS_BACKEND) $(HEADERS_FRONTEND) ./headers/*.h

BACKEND_OBJS = $(BACKEND:.cpp=.o)
FRONTEND_OBJS = $(FRONTEND_BUILD:.cpp=.o)

LCOV_MODULES = ./backend/model/transform_matrix.cpp ./backend/model/file_reader.cpp

TARGET = 3DViewer
LIBRARY = lib_3DViewer.a

.PHONY: all clean gcov_report clang clang_edit tests \
	install uninstall dist dvi memory cpp $(TARGET) $(LIBRARY) \
	install_lib install_dvi

all: install

%.o: %.cpp
	$(CXX) $(FLAGS) $(DESKTOP_FLAGS) -c $< -o $@

%_moc.cpp: %.h
	moc $< -o $@

$(QT_RESOURCES): $(QT_RESOURCES_FILE)
	rcc $< -o $@

$(LIBRARY): $(BACKEND_OBJS)
	ar rc $@ $^
	ranlib $@

$(TARGET): $(LIBRARY) $(FRONTEND_OBJS)
	$(CXX) $(FLAGS) $(FRONTEND_OBJS) -L. $(LIBRARY) -L. $(MATRIX_LIBRARY) -o $@ $(DESKTOP_FLAGS)

install: $(TARGET)
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(TARGET)

tests: $(LIBRARY) $(TEST_FUNC)
	$(CXX) $(FLAGS) $(TEST_FUNC) -L. $(LIBRARY) -L. $(MATRIX_LIBRARY) -o test $(TEST_FLAGS)
	./test

test_lcov: clean
	$(CXX) $(FLAGS) $(LCOV_FLAGS) $(BACKEND) -c
	$(CXX) $(FLAGS) *.o $(TEST_FUNC) -L. $(MATRIX_LIBRARY) -o $@ $(TEST_FLAGS) -lgcov
	./test_lcov
	lcov --capture --directory . --output-file coverage_dirty.info
	lcov --extract coverage_dirty.info "$(PWD)/backend/model/transform_matrix.cpp" "$(PWD)/backend/model/file_reader.cpp" --output-file coverage.info

gcov_report: test_lcov
	genhtml coverage.info --output-directory coverage

clean:
	rm -rf *.o game *.db *.out *.a *.gc* *coverage* \
	$(TARGET) *.tar.gz *.log *.dvi *.toc *.aux test \
	$(FRONTEND_OBJS) $(BACKEND_OBJS) $(MOC_SOURCES) \
	$(QT_RESOURCES) $(DISTDIR) test_lcov

dist:
	mkdir -p $(DISTDIR)
	cp -r backend gui headers tests Makefile $(DOCFILE) $(DISTDIR)/
	tar czf $(DISTDIR).tar.gz $(DISTDIR)

dvi:
	$(TEXI2DVI) $(DOCFILE)

clang:
	clang-format -n $(ALL_FUNC) $(TEST_FUNC) $(ALL_HEADERS)

clang_edit:
	clang-format -i $(ALL_FUNC) $(TEST_FUNC) $(ALL_HEADERS)

memory: tests
	$(LEAK_CHECKER) ./test

cpp: 
	cppcheck --enable=all --language=c++ --std=c++20 \
	-Dslots -Dsignals -DQ_OBJECT --suppress=missingIncludeSystem $(ALL_FUNC)

install_lib:
	sudo apt update
	sudo apt install gcc
	sudo apt install lcov
	sudo apt install valgrind
	sudo apt install qt5-qmake qtbase5-dev qtchooser qt5-qmake-bin qtbase5-dev-tools
	sudo apt install libglm-dev
	sudo apt install pkgconf

install_dvi:
	sudo apt install texinfo
	sudo apt install texlive
