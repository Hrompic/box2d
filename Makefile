all: and_lib apk
and_lib:
	#aarch64-linux-android21-clang++ main.cpp -D__ANDROID__ -shared -fPIC -lbox2d -lsfml-graphics -lsfml-system -lsfml-window -lsfml-main -o a.so
	aarch64-linux-android21-clang++ -Wl,--whole-archive /home/user/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libsfml-main.a -Wl,--no-whole-archive main.cpp -D__ANDROID__ -shared -fPIC -lbox2d -lsfml-graphics -lsfml-system -lsfml-window -lsfml-activity  -o and/lib/arm64-v8a/liba.so 
apk:
	cd and&&make rlib
install:
	cd and&&make install



