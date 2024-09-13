CXXFLAGS = -O2 -g `pkg-config --cflags gdk-pixbuf-2.0`
LDFLAGS = `pkg-config --libs gdk-pixbuf-2.0`

OBJECTS = main.o pic.o similar.o
PROGRAM = image-duplicate-finder

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

pic.o: pic.h similar.h
similar.cc: similar.h
main.o: pic.h similar.h

clean:
	rm -f $(PROGRAM) $(OBJECTS)
