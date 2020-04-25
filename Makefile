all:
	gcc main.c `pkg-config --cflags --libs gstreamer-1.0` -o test
