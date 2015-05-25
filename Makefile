compile: transformation.c birghtness.c
	gcc -o trasformation transformation.c `pkg-config --cflags --libs opencv`
	gcc -g -o brightness birghtness.c `pkg-config --cflags --libs opencv`
