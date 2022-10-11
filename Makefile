LIB_TARGET = libcrashdump.a
LIB_SO_TARGET = libcrashdump.so

WORKSPACE := $(shell pwd)

OUTPUT := $(WORKSPACE)/release

CC = $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
STRIP := $(CROSS_COMPILE)strip
RM := rm -rf

CFLAGS += -fPIC -funwind-tables
CFLAGS += -lm -pthread -ldl -lrt
LIBSO_FLAGS += -shared

LIBS_SRC =  $(wildcard $(WORKSPACE)/source/*.c)

LIBS_INCLUDES = -I $(WORKSPACE)/include

LIB_OBJS := $(patsubst %.c, %.o, $(LIBS_SRC))

all: $(LIB_SO_TARGET) $(LIB_TARGET)

%.o : %.c
	$(CC) $(CFLAGS) $(LIBS_INCLUDES) -c $< -o $@

$(LIB_TARGET) :$(LIB_OBJS)
	$(AR)  cr $(OUTPUT)/$@ $(LIB_OBJS)
	@echo "static library build finish"

$(LIB_SO_TARGET) :$(LIB_OBJS)
	mkdir -p $(OUTPUT)
	$(CC) $(CFLAGS) $(LIBSO_FLAGS) $(LIBS_INCLUDES) -o $(OUTPUT)/$@ $(LIB_OBJS)
	@echo "shared library build finish"
	
clean:
	@$(RM) $(LIB_OBJS) $(OUTPUT)/$(LIB_TARGET) $(OUTPUT)/$(LIB_SO_TARGET)
	@$(RM) $(WORKSPACE)/release
