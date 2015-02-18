.DELETE_ON_ERROR:

ARCH ?= intel

INSTALLDIR ?= ../$(ARCH)

CXX=g++
CFLAGS=-Wall -O3 -c 
LDFLAGS=-lz

PROG = applyTokenizer properHighlighter extr-propnames corpusStat splitParallel mycat applyRegexMap filterBiText2  simplifyText

all: $(PROG) 

ALLOBJ=applyTokenizer.o properHighlighter.o extr-propnames.o corpusStat.o filterlongSen.o gzstream.o applyRegexMap.o IOfile.o myassert.o mycat.o RegexMap.o splitParallel.o StringMap.o Tokenizer.o SplitParagraph.o

%.o: %.cc
	$(CXX) $(CFLAGS) -o $@ $<

applyTokenizer: applyTokenizer.o Tokenizer.o myassert.o gzstream.o IOfile.o RegexMap.o StringMap.o
	$(CXX) $^ $(LDFLAGS) -o $@

applyRegexMap: applyRegexMap.o RegexMap.o gzstream.o IOfile.o StringMap.o SplitParagraph.o
	$(CXX) $^ $(LDFLAGS) -o $@

properHighlighter: properHighlighter.o
	$(CXX) $^ $(LDFLAGS) -o $@

corpusStat: corpusStat.o gzstream.o IOfile.o
	$(CXX) $^ $(LDFLAGS) -o $@

extr-propnames: extr-propnames.o
	$(CXX) $^ $(LDFLAGS) -o $@

splitParallel: splitParallel.o IOfile.o gzstream.o
	$(CXX) $^ $(LDFLAGS) -o $@

mycat: mycat.o IOfile.o gzstream.o
	$(CXX) $^ $(LDFLAGS) -o $@


filterBiText2: filterBiText2.o 
	$(CXX) $^ $(LDFLAGS) -o $@


simplifyText: simplifyText.o gzstream.o IOfile.o
	$(CXX) $^ $(LDFLAGS) -o $@


INSTALL: $(PROG)
	-mkdir -p $(INSTALLDIR)
	mv $^ $(INSTALLDIR)


clean:
	rm *.o

