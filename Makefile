CFLAGS ::= \
	-Wall \
	-Wpedantic

ifeq ($(SHOW_STRUCT_PADDING), y)
	CFLAGS += -Wno-padded
endif

override CFLAGS += -Iinclude -std=c11 -fPIC
override CXXFLAGS += -Iinclude -std=c++11 -fPIC

OBJS ::= obj/MatrixOps.o obj/module.o

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS) $(COFLAGS)

obj/%.o: src/%.cpp include/%.h
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(CXXOFLAGS)

.PHONY: clean
.DEFAULT_GOAL: objs

objs: $(OBJS)

clean:
	rm -f obj/*.o

gizmo_matrix.so: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(CLFLAGS) -shared

all: gizmo_matrix.so
