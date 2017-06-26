CC=g++
STD=c++11
SRC=score.cpp
LIBS=-lcurl

score:
		$(CC) $(SRC) -o score -std=$(STD) $(LIBS)

clean: 
		rm -rf ./score
