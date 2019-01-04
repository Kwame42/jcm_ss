TARGET = jcm_ss
LIBS = -lm -ljson-c -lpthread
CC = clang #-fsanitize=address
CFLAGS = -g -Wall -Wno-deprecated-declarations
PROC_TYPE = $(strip $(shell uname -m | grep 64))
OS = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
DARWIN = $(strip $(findstring DARWIN, $(OS)))
ifneq ($(DARWIN),)
	CFLAGS += -DMAC
	LIBS += -framework OpenCL

	ifeq ($(PROC_TYPE),)
		CFLAGS += -arch i386
	else
		CFLAGS += -arch x86_64
	endif
else
	LIBS += -lOpenCL
	ifeq ($(PROC_TYPE),)
		CFLAGS += -m32
	else
		CFLAGS += -m64
	endif
	ifdef AMDAPPSDKROOT
		INC_DIRS =. $(AMDAPPSDKROOT)/include
		ifeq ($(PROC_TYPE),)
			LIB_DIRS = $(AMDAPPSDKROOT)/lib/x86
		else
			LIB_DIRS = $(AMDAPPSDKROOT)/lib/x86_64
		endif
	else
		ifdef CUDA
			INC_DIRS =. $(CUDA)/OpenCL/common/inc
		endif
	endif
endif

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
