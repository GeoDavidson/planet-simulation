RAYLIB_PATH = C:/raylib/raylib
COMPLER_PATH = C:/raylib/w64devkit/bin
LDLIBS = -lraylib -lgdi32 -lwinmm
CC = gcc

final: obj/main.o
	$(COMPLER_PATH)/$(CC) obj/main.o -o main.exe $(LDLIBS) && main.exe

obj/main.o: src/main.c
	$(COMPLER_PATH)/$(CC) -c src/main.c -I $(RAYLIB_PATH)/src -o obj/main.o

clean:
	rm obj/main.o main.exe
