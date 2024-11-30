RAYLIB_PATH = C:/raylib/raylib
COMPLER_PATH = C:/raylib/w64devkit/bin
LDLIBS = -lraylib -lgdi32 -lwinmm
CC = gcc

run: final
	main.exe

final: src/main.c
	$(COMPLER_PATH)/$(CC) src/main.c -I $(RAYLIB_PATH)/src -o main.exe $(LDLIBS)

clean:
	rm main.exe
