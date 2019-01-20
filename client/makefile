GCC = gcc
OBJECTS=client.o
TAR=client

COMMON_INC_DIR = ../common
COMMON_LINK = -L$(COMMON_INC_DIR)/ -lcommon -lpthread
CPPFLAGS = -I$(COMMON_INC_DIR)

all:
	@echo "\n\t Usage : make release\n"

release:$(OBJECTS)
	$(GCC) -o $(TAR) $(OBJECTS) $(COMMON_LINK) 

.c.o:$(COMMON_INC_DIR)
	$(GCC) $(CPPFLAGS) -c $<

.PHONY:clean
clean:
	rm -rf $(OBJECTS) $(TAR)