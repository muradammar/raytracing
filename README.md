#raytracing

This is a simple raytracing algorithm in C. The SDL2 library display and event handling. This project was created following along with @HirschDaniel on youtube.

to run the program, the SDL2 library is required.

the program can be run in powershell with the following command:

gcc -o ray raytracing.c $(sdl2-config --cflags --libs)

alternatively, the MSYS UCRT64 shell command:

gcc -o ray raytracing.c $(sdl2-config --cflags --libs)
