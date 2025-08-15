#define _WIN32_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>
#include "vtable.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include "mem.h"
#include <stdlib.h>

#include "CChat.h"
#include "CGame.h"
using namespace sampapi::v03dl;

typedef HRESULT(__stdcall* _EndScene)(IDirect3DDevice9* pDevice);
_EndScene oEndScene;

bool draw = false;

HRESULT __stdcall hkEndScene(IDirect3DDevice9* pDevice)
{
  if(draw) {
    D3DRECT BarRect = { 100,100,200,200 };
    pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(1, 1, 1, 1), 0.0f, 0);
  }
  return oEndScene(pDevice);
}

std::vector<char> vBuffer(20 * 1024);

enum LOG_TYPE {
  LOG_INFO,
  LOG_ERROR,
  LOG_SUCCESS,
  LOG_DEBUG
};

const int byteCodesToInt(BYTE data[]) {
  int value;
  memcpy(&value, data, sizeof(int));
  return value;
}

const std::string currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
  return buf;
}

void logger(LOG_TYPE logType, std::string message, ...) {
  std::cout << "[" << currentDateTime() << " ";
  switch(logType) {
    case LOG_INFO:
      std::cout << "\033[93;1m" << "   info";
      break;
    case LOG_ERROR:
      std::cout << "\033[91;1m" << "  error";
      break;
    case LOG_SUCCESS:
      std::cout << "\033[92;1m" << "success";
      break;
    case LOG_DEBUG:
      std::cout << "\033[35;1m" << "  debug";
      break;
  }
  std::cout << "\033[0m] ";
  va_list args;
  va_start(args, message);
  std::vfprintf(stdout, message.c_str(), args);
  std::cout << std::endl;
}


DWORD WINAPI MainThread(HMODULE hModule) {
  AllocConsole();
  FILE* f = new FILE;
  freopen_s(&f, "CONOUT$", "w", stdout);
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hConsole, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hConsole, dwMode);

  uintptr_t gtaBase = (uintptr_t)GetModuleHandle("gta_sa.exe");
  
  logger(LOG_INFO, "Starting up..");
  CGame *&pGame = RefGame();

  while(!pGame->IsStarted()) {
    Sleep(1);
    logger(LOG_INFO, "Waiting game start..");
  }

  Sleep(1000);

  void** vTableDevice = *(void***)(*(DWORD*)0xC97C28);
  VTableHookManager* vmtHooks = new VTableHookManager(vTableDevice, 119);
  oEndScene = (_EndScene)vmtHooks->Hook(42, (void*)hkEndScene);

  
 
  //asio::error_code ec;

  //asio::io_context context;

  //asio::io_context::work idleWork(context);
  //std::thread thrContext = std::thread([&]() { context.run(); });
  //asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", ec), 8080);

  //asio::ip::tcp::socket socket(context);

  //socket.connect(endpoint, ec);

  //if (!ec) {
  //  logger(LOG_SUCCESS, "Connected!");
  //} else {
  //  logger(LOG_ERROR, "Failed to connect: %s", ec.message());
  //}

  while(true) {
    

    if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
      //ProcessServerData(socket);
      //std:: string sRequest = "getServerInfo";

      if(draw) {
        
	draw = false;
      } else {
        
	draw = true;
      }

      //socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
    }

    if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
      //for (int i = 0; i < sizeof(vBuffer); i++)
      //std::cout << vBuffer[i];
      CChat *&pChat = RefChat();
      if(pChat) pChat->AddMessage(-1, "svya aime les hommes");
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
      //context.stop();
      //socket.close();
      break;
    }


    Sleep(5);
  }

  fclose(f);
  FreeConsole();
  FreeLibraryAndExitThread(hModule, 0);
  return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL));
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
