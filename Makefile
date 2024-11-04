SOURCES := $(wildcard source/*.c) $(wildcard source/**/*.c)
OBJECTS := $(patsubst source/%.c,objects/%.o,$(SOURCES))
OUT     := zk

ifeq ($(PLAT),windows)
	CC := x86_64-w64-mingw32-gcc
	override LDLIBS := $(LDLIBS) $(shell x86_64-w64-mingw32-pkg-config sdl2 --libs) -lopengl32
	override CFLAGS := $(CFLAGS) $(shell x86_64-w64-mingw32-pkg-config sdl2 --cflags)
else
	override LDLIBS += -lSDL2 -lGL
endif
LD := $(CC)

override CFLAGS += -std=c99 -Wall -Wextra -Wuninitialized -Wundef -pedantic
override LDLIBS += -lm

ifeq ($(BUILD),release)
	override CFLAGS += -O3
else
	override CFLAGS += -g
	ifeq ($(ASAN),y)
		override CFLAGS += -fsanitize=address
	endif
endif

.SECONDEXPANSION:

define inc
$$(patsubst /dev/null\:,,$$(patsubst /dev/null,,$$(wildcard $$(shell $(CC) $(CFLAGS) $(CPPFLAGS) -xc -MM /dev/null $$(wildcard $(1)) -MT /dev/null))))
endef

all: $(OUT)
	@:

run: $(OUT)
	'$(dir $<)$(notdir $<)' $(RUNFLAGS)

$(OUT): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

objects/:
	mkdir -p objects

objects/%.o: source/%.c $(call inc,source/%.c) | objects/
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

clean:
	rm -r objects

distclean: clean
	rm $(OUT)

.PHONY: all run clean distclean
