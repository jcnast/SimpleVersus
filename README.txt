SDL2 is used for all graphics components, the code should be straight forward for those interested in making some custom changes.

Compiling information is given below: (although a make file is included)...

		Compile to Windows (from Linux): x86_64-linux-gnu-g++ -o SimpleVersus.exe *.cc -lSDL2 -lSDL2_image

		Compile to Linux: g++ -o SimpleVersus *.cc -lSDL2 -lSDL2_image
