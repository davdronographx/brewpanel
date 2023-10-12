rm -rf bin/*.*

cp -a img/. bin

gcc ./src/panel/brewpanel-x11.cpp -g  -I./dependencies -lstdc++ -o ./bin/brewpanel -w -fpermissive -lm -ldl -lX11 -lXext