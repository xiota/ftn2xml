CC=g++
#CFLAGS=-I .

LIBS=-lpodofo

DEPS = auxiliary.h fountain.h

OBJ = ftn2xml.o auxiliary.o fountain.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ALL: ftn2html ftn2pdf ftn2xml ftn2fdx

ftn2xml: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

ftn2pdf: ftn2xml
	ln -sf ftn2xml ftn2pdf

ftn2html: ftn2xml
	ln -sf ftn2xml ftn2html

ftn2fdx: ftn2xml
	ln -sf ftn2xml ftn2fdx

.PHONY: clean

clean:
	rm -f *.o *~ core *~ ftn2xml ftn2pdf ftn2html ftn2fdx
