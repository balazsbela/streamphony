#CXX defaults to g++
CXXFLAGS = -Wall -g -I..

LIB_TOP_DIR = ../..
RSLIBDIR = $(LIB_TOP_DIR)/../libretroshare
LIBS = -L../lib -lbitdht -lpthread -L$(RSLIBDIR)/src/lib -lretroshare -lcrypto

# OK. So in the current dir, /libbitdht/src/auth
# we're including
# #include "../../libretroshare/src/util/rsaes.h"
# in AuthCryptoFns.h.
# To include a header from another lib you need to give the compiler an include path
# and also the lib file to link against.
# Instead I'm giving it the liv (see -Lretroshare opt above) and including the
# file using it's relative path to the current directory.
# INCPATH = -I$(RSLIBDIR)/src
# the above INCPATH path should expand to:
# INCPATH = -I../../../libretroshare/src

EGOBJ = Storage.o AuthCryptoFns.o PasswordAuth.o bdHandler.o

all: main msgtest

main: $(EGOBJ)
	$(CXX) $(CXXFLAGS) -o tests tests.cc $(EGOBJ) $(LIBS)

# test: tests.o
# 	$(CXX) $(CXXFLAGS) -o tests tests.o $(LIBS)

msgtest: udpbitdht_msgtest.o
	$(CXX) $(CXXFLAGS) $(EGOBJ) udpbitdht_msgtest.o -o udpbitdht_msgtest  $(LIBS)

clean:
	rm -f $(EGOBJ) tests.o $(EXEC)
