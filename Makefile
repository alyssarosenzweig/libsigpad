all: hid.o
	cc -o cat cat.c hid.o libsigpad.c stext.c -std=c99 `freetype-config --cflags` `freetype-config --libs` -lusb -lpthread

hid.o: hid_LINUX.c hid.h
	cc $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o cat
	rm -rf tmp

