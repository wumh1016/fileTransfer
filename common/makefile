GCC = gcc
ARFLAGS = rvf
AR = ar
OBJECTS=threadpool.o transfer.o mmapFile.o
TAR=libcommon.a

all:
	@echo "\n\t Usage : make release\n"

release:$(OBJECTS)
	$(AR) $(ARFLAGS) $(TAR) $(OBJECTS)

.c.o:
	$(GCC) -c $< -std=c99

.PHONY:clean
clean:
	rm -rf $(OBJECTS) $(TAR)