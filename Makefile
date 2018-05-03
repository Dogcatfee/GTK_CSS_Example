build:
	g++ main.cpp -g `pkg-config --libs --cflags gtk+-3.0` -o run_main -rdynamic
clean:
	rm run_main
