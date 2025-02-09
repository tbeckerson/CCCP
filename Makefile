build:
	cd lib/libspm; echo "Now on libspm"; \
        make all; \
		make formats; \
		make install 
	cd ../.. && echo "Now on CCCP"
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp 

all: build install clean

buildnolib: 
	[ -d bin ] || mkdir bin
	gcc src/main.c -lspm -O2 -o bin/cccp	

clean:
	rm -rf bin/*

install: build
	cp bin/cccp /usr/bin
	cp cccp.conf /etc/cccp.conf

uninstall:
	rm -rf /usr/bin/cccp && rm -rf /etc/cccp.conf && rm -rf /var/cccp/data
