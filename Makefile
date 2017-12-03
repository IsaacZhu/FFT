CC=gcc

FFT.o: FFT.c
ifeq (${CC},gcc)
	${CC} -g -Wall FFT.c -o $@ -lm
else
	${CC} -g -Wall FFT.c -o $@
endif

.Phony:clean
clean:
	rm *.o
