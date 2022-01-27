mkdir build
sudo apt install unzip -y
cd Base
ls
unzip -q ogre-master
unzip -q openal-soft-1.21.1.zip
unzip -q vorbis-1.3.7.zip
cd ..
cd build
sudo apt install build-essential automake libtool libfreetype6-dev libfreeimage-dev libzzip-dev libxrandr-dev libxaw7-dev freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev libpoco-dev libtbb-dev doxygen libcppunit-dev libsdl2-dev build-essential
cmake -D OGRE_STATIC=TRUE ..
make -j8
./GameApp
