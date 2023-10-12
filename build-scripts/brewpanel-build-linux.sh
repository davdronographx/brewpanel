rm -rf bin/*.*

cp -a img/. bin

gcc ./src/panel/brewpanel-sdl2.cpp  -I./dependencies -lSDL2 -lstdc++ -o ./bin/brewpanel -w -fpermissive -lm -ldl