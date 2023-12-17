CXX = g++
CXXFLAGS = -Wall -std=c++11 -Wno-deprecated-declarations
LDFLAGS = -lcrypto

OBJS = keygen.o signverify.o lamportscheme.o

HEADERS = keygen.h signverify.h

EXEC = lamport

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

keygen.o: keygen.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

signverify.o: signverify.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

lamportscheme.o: lamportscheme.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
