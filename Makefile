CC=i686-w64-mingw32-g++.exe

dllmain: dllmain.cpp
	$(CC) -shared -std=c++17 --static -o gtcef.dll *.cpp -lws2_32
