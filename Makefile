all: and_lib apk
and_lib:
	aarch64-linux-android21-clang++ main.cpp -fPIC -lbox2d -lsfml-graphics -lsfml-system -lsfml-window -o a.so
apk:

