#pragma once
#include <windows.h>
#include <vector>

namespace mem {
  void Patch(BYTE* dst, BYTE* src, unsigned int size);
  void Nop(BYTE* dst, unsigned int size);
  uintptr_t GetAddressAtOffset(uintptr_t ptr, std::vector<unsigned int> offsets);
  uintptr_t GetAddress(uintptr_t ptr);
}
