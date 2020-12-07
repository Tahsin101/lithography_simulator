# detect OS
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

all:
	gcc -g  agsUtil.c glpParse.c kiss_fft.c kiss_fftnd.c lithosim.c lodepng.c parse_error.c parse_new.c parse_print.c parse.c -lm -o lithosim.exe
preprocess:
	gcc -g  Preprocessing.c agsUtil.c glpParse.c kiss_fft.c kiss_fftnd.c lodepng.c parse_error.c parse_new.c parse_print.c parse.c -lm -o preprocess.exe
preprocess1:
	gcc -g  -O2 Preprocessing1.c agsUtil.c glpParse.c kiss_fft.c kiss_fftnd.c lodepng.c parse_error.c parse_new.c parse_print.c parse.c -lm -o preprocess1.exe
simulator:
	gcc -g  -O2 csvparser.c Simulator.c agsUtil.c glpParse.c kiss_fft.c kiss_fftnd.c lodepng.c parse_error.c parse_new.c parse_print.c parse.c -lm -o simulator.exe
interference:
	gcc -g  interference_map.c agsUtil.c glpParse.c kiss_fft.c kiss_fftnd.c lodepng.c parse_error.c parse_new.c parse_print.c parse.c -lm -o interference.exe
clean:
	rm -fr lithosim
