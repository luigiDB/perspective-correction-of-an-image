compile: transformation.c birghtness.c
	gcc -o trasformation transformation.c `pkg-config --cflags --libs opencv`
	gcc -o birghtness birghtness.c `pkg-config --cflags --libs opencv`
