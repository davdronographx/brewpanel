rm -rf bin/*.*

cp -a img/. bin

cp -a run-scripts/. bin

gcc ./src/panel/brewpanel-x11.cpp -g  -I./dependencies -lstdc++ -o ./bin/brewpanel -w -fpermissive -lm -ldl -lX11 -lpthread