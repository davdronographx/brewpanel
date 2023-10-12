rm -rf bin/*.*

cp -a img/. bin

arm-linux-gnueabi-g++ ./src/panel/brewpanel-sdl2.cpp  -I./dependencies -lSDL2 -lstdc++ -o ./bin/brewpanel -w -fpermissive -lm -ldl