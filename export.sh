mkdir android
cd Base
unzip -q ogre-13.1.0.zip
cd ..
cd android
cmake -DCMAKE_TOOLCHAIN_FILE=/home/rgr/Android/Sdk/ndk/23.1.7779620/build/cmake/android.toolchain.cmake -DANDROID_NDK=/home/rgr/Android/Sdk/ndk/23.1.7779620 ..
make -j8
