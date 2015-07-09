compile: transformation.c brightness.c circles.c main.c
	gcc -Wall -Werror -c transformation.c `pkg-config --cflags --libs opencv` 
	gcc -g -o brightness brightness.c `pkg-config --cflags --libs opencv`-lm
	gcc -Wall -Werror -c circles.c `pkg-config --cflags --libs opencv` 
	gcc -o homography main.c
