RAYLIB_PATH = C:/raylib/raylib
COMPLER_PATH = C:/raylib/w64devkit/bin
LDLIBS = -lraylib -lgdi32 -lwinmm
CC = gcc

final: src/main.c
	$(COMPLER_PATH)/$(CC) src/main.c -I $(RAYLIB_PATH)/src -o main.exe $(LDLIBS) && main.exe

clean:
	rm main.exe
