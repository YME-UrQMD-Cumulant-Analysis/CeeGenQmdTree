all: cvt

cvt: convert.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -O3