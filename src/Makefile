CXX ?= g++
CPP ?= -O3

LIBS ?= -lpodofo

DEPS = auxiliary.h fountain.h

OBJ = ftn2xml.o auxiliary.o fountain.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

ALL: ftn2html ftn2pdf ftn2xml ftn2fdx

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
	rm -f *.o *~ core *~ ftn2xml ftn2pdf ftn2html ftn2fdx
