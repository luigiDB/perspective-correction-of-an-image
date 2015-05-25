compile: transformation.c
	gcc -o trasformation transformation.c `pkg-config --cflags --libs opencv`
