init:
	aapt package -f -m -M AndroidManifest.xml -S res/ -J gen -I /home/user/Android/Sdk/platforms/android-23/android.jar
	/usr/lib/jvm/java-8-openjdk-amd64/bin/javac -d obj -cp obj -bootclasspath /home/user/Android/Sdk/platforms/android-23/android.jar gen/org/sfmldev/android/* #with java 11 not working
	dx --dex --output=./classes.dex obj
	aapt package -f -m -F app.apk -A assets/ -M AndroidManifest.xml -S res/ -I /home/user/Android/Sdk/platforms/android-23/android.jar
	aapt add app.apk classes.dex lib/arm64-v8a/*

rlib:
	aapt r app.apk lib/arm64-v8a/liba.so 
	aapt add app.apk lib/arm64-v8a/liba.so
	$(MAKE) sign

sign:
	zipalign -f 4 app.apk app1.apk
	apksigner sign --ks ~/mykey.keystore app1.apk <~/keypass
install:
	adb install -r app1.apk
	sleep 0.2 #Need sleep any time after installation to run
	adb shell am start org.sfmldev.android/android.app.NativeActivity
