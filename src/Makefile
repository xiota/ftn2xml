VERSION ?= 0.1.0

CXX ?= g++
CPP ?= -O3

LIBS ?= -lpodofo

DEPS = auxiliary.h fountain.h ftn2xml.h

OBJ = ftn2xml.o auxiliary.o fountain.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

ALL: ftn2html SYMLINKS

SYMLINKS: ftn2pdf ftn2xml ftn2fdx

ftn2xml.o: ftn2xml.h
ftn2xml.h:
	sed -E -e 's&@VERSION@&'${VERSION}'&' ftn2xml.h.in > ftn2xml.h

ftn2xml: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

ftn2pdf: ftn2xml
	ln -sf ftn2xml ftn2pdf

ftn2html: ftn2xml
	ln -sf ftn2xml ftn2html

ftn2fdx: ftn2xml
	ln -sf ftn2xml ftn2fdx

.PHONY: clean

clean:
	rm -f *.o *~ core *~ ftn2xml ftn2xml.h ftn2pdf ftn2html ftn2fdx
