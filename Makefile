CFLAGS ::= \
	-Wall \
	-Wpedantic \
	-isystem /usr/local/musl/include

ifeq ($(SHOW_STRUCT_PADDING), y)
	CFLAGS += -Wno-padded
endif

override CFLAGS += -Iinclude -std=c11 -fPIC
override CXXFLAGS += -Iinclude -std=c++11 -fPIC

OBJS ::= obj/MatrixOps.o obj/module.o

include c.mak
include cpp.mak

.PHONY: clean
.DEFAULT_GOAL: objs

objs: $(OBJS)

clean:
	rm -f obj/*.o

gizmo_matrix.so: $(OBJS)
	$(call recipe_link, -shared)

all: gizmo_matrix.so
