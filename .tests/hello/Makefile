CC=gcc
CFLAGS=-I.
DEPS=hellomake.h
BUILD=build
FILES= *.o

%(BUILD)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: hellomake.o hellofunc.o
	$(CC) -o hello hellomake.o hellofunc.o
	mkdir $(BUILD)
	cp -r $(FILES) $(BUILD)
	rm -f hellofunc.o hellomake.o

.PHONY: clean

clean:
	rm -f hellofunc.o hellomake.o