#ifdef USE_MEM

#include <iostream>
#include "MemManager.h"

#undef new
MemManager gMemManager;

void* MemManager::Alloc(std::size_t size, std::string info) {
	static long long count = 0;
	info = "(" + std::to_string(count++) + ")" + info;
	void* memLocation = malloc(size);
	allocationToSource.insert(std::make_pair(memLocation, info));
	std::cout << "Our new: " << size << "  " << info << std::endl;
	return memLocation;
}

void MemManager::Free(void* ptr) {
	if (allocationToSource.empty() == false && allocationToSource.find(ptr) != allocationToSource.end()) {
		std::cout << "Deleting:   " << allocationToSource[ptr].c_str() << std::endl;
		allocationToSource.erase(ptr);
	}
	free(ptr);
}

MemManager::~MemManager() {
	std::cout << "Unfreed memory:" << std::endl;
	while (allocationToSource.empty() == false) {
		std::pair<void*, std::string> front = *allocationToSource.begin();
		gMemManager.Free(front.first);
	}
}

void* operator new[](std::size_t size, const std::string fileName, int lineNum) {
	unsigned int folderIndex = static_cast<unsigned int>(fileName.find_last_of('\\')) + 1;
	std::string trackName = fileName.substr(folderIndex, fileName.length() - folderIndex)
		+ ':' + std::to_string(lineNum);
	return gMemManager.Alloc(size, trackName);
}
void* operator new(std::size_t size, const std::string fileName, int lineNum) {
	unsigned int folderIndex = static_cast<unsigned int>(fileName.find_last_of('\\')) + 1;
	std::string trackName = fileName.substr(folderIndex, fileName.length() - folderIndex)
		+ ':' + std::to_string(lineNum);
	return gMemManager.Alloc(size, trackName);
}
void operator delete(void* ptr) noexcept { gMemManager.Free(ptr); }
void operator delete[](void* ptr) noexcept { gMemManager.Free(ptr); }
void operator delete(void* ptr, const std::string, int) noexcept { gMemManager.Free(ptr); }
void operator delete[](void* ptr, const std::string, int) noexcept { gMemManager.Free(ptr); }

#endif