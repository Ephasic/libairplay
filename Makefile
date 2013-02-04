CFLAGS:=-O2 -Wall $(shell pkg-config --cflags openssl) -I./lib -g -c -fPIC
LDFLAGS:= -lm -lpthread $(shell pkg-config --libs openssl)
OBJS=airsock.o airbase64.o airplay.o
all: libairplay

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o src/$@

libairplay:
	    gcc -shared -Wl,-soname,libairplay.so.1 \
                -o libairplay.so.1.0.1 src/$(OBJS)
