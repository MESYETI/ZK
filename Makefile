SRC   = $(wildcard source/*.c) $(wildcard source/**/*.c)
DEPS  = $(wildcard source/*.h) $(wildcard source/**/*.h)
OBJ   = $(addsuffix .o,$(subst source/,bin/,$(basename ${SRC})))
OUT   = NotElite
FLAGS = -std=c99 -Wall -Wextra -pedantic -g

ifeq ($(PLAT), windows)
	CC     = x86_64-w64-mingw32-gcc
	LIBS   = $(shell x86_64-w64-mingw32-pkg-config sdl2 --libs) -lopengl32
	FLAGS += $(shell x86_64-w64-mingw32-pkg-config sdl2 --cflags)
else
	LIBS = -lSDL2 -lm -lGL
endif

ifeq ($(BUILD), release)
	FLAGS += -O3
endif

compile: ./bin $(OBJ) $(SRC) $(DEPS)
	$(CC) $(OBJ) $(LIBS) -o $(OUT)

./bin:
	mkdir -p bin

bin/%.o: source/%.c $(DEPS)
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -r bin/* $(OUT)
	if [ -f bin ]; then rm -r bin; fi
	if [ -f $(OUT) ]; then rm $(OUT); fi


