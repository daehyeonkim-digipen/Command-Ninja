#ifdef USE_MEM

#pragma once
#include <map>
#include <string>

class MemManager {
public:
	MemManager() {}
	~MemManager();
	void* Alloc(std::size_t size, const std::string info);
	void Free(void* ptr);
private:
	std::map<void*, std::string> allocationToSource;
};
void* operator new(std::size_t size, const std::string fileName, int lineNum);
void* operator new[](std::size_t size, const std::string fileName, int lineNum);
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr, const std::string fileName, int lineNum) noexcept;
void operator delete[](void* ptr, const std::string fileName, int lineNum) noexcept;

#define new new(__FILE__,__LINE__)

#endif