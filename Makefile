compile: transformation.c brightness.c
	gcc -o trasformation transformation.c `pkg-config --cflags --libs opencv`
	gcc -g -o brightness brightness.c `pkg-config --cflags --libs opencv`
