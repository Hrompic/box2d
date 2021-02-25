init:
	aapt package -f -m -M AndroidManifest.xml -S res/ -J gen -I /home/user/Android/Sdk/platforms/android-23/android.jar
	javac -d obj -cp obj -bootclasspath /home/user/Android/Sdk/platforms/android-23/android.jar gen/org/sfmldev/android/*
	dx --dex --output=./classes.dex obj
	aapt package -f -m -F app.apk -A assets/ -M AndroidManifest.xml -S res/ -I /home/user/Android/Sdk/platforms/android-23/android.jar
	aapt add app.apk classes.dex lib/arm64-v8a/*
trlib: 
	aapt r app.apk lib/arm64-v8a/liba.so 
	aapt add app.apk lib/arm64-v8a/liba.so
#	$(MAKE) sign
	$(MAKE) cp
rlib:
	aapt r app.apk lib/arm64-v8a/liba.so 
	aapt add app.apk lib/arm64-v8a/liba.so
	$(MAKE) sign

sign:
	zipalign -f 4 app.apk app1.apk
	apksigner sign --ks ~/mykey.keystore app1.apk <~/keypass
cp:
	cp app1.apk /sdcard/temp/
install:
	adb install -r app1.apk
