CXX=g++
LD=g++
CXXFLAGS=--std=c++17 -Wall --pedantic -g
SRCDIR=src
TESTDIR=tests

all: chvojvoj

chvojvoj: main.o CAppParam.o CArgParser.o CDownloading.o CLog.o CActionMediator.o \
	CAction.o CRecursionDecider.o CHtmlProcessing.o CUrlTranslator.o CProcessing.o CSaving.o \
	CHttpParsing.o CUrlAuthority.o CUrl.o IAction.o CActionQueue.o CCompositeDecider.o \
	CDuplicateFileDecider.o CRegexDecider.o CNegationDecider.o
	$(LD) $(CXXFLAGS) -o $@ $^ -lxml2 -lpthread -lstdc++fs

CHtmlProcessing.o: $(SRCDIR)/CHtmlProcessing.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I/usr/include/libxml2

main.o: $(SRCDIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I/usr/include/libxml2

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf *.o chvojvoj doc/* Makefile.d chvojvoj-tests

deps:
	$(CXX) -MM src/*.cpp > Makefile.d

compile: chvojvoj

run:
	./chvojvoj

doc: FORCE
	doxygen

FORCE: ;

test: chvojvoj-tests
	./chvojvoj-tests

chvojvoj-tests: TCArgParser.o TCAppParam.o CArgParser.o CAppParam.o CLog.o \
		TCUrl.o TCUrlAuthority.o CUrl.o CUrlAuthority.o test_main.o 
	$(LD) $(CXXFLAGS) -o $@ $^ -lstdc++fs

test_main.o: $(TESTDIR)/test_main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

T%.o: $(TESTDIR)/T%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


-include Makefile.d
