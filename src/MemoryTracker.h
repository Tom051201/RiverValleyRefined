#pragma once
#include <iostream>

struct MemoryMetrics {
public:

	uint32_t m_totalAllocated;
	uint32_t m_totalFreed;
	uint32_t m_countAllocated;

	MemoryMetrics();

	uint32_t currentHeapUsage();
	
	void printHeapUsage();
	void printHeapAllocated();
	void printStackUsage();

	void report();

};

/*
void* operator new(size_t size) {

	s_allocationMetrics.m_totalAllocated += size;
	s_allocationMetrics.m_countAllocated++;

	return malloc(size);

}
void operator delete(void* memory, size_t size) {

	s_allocationMetrics.m_totalFreed += size;
	s_allocationMetrics.m_countAllocated--;

	free(memory);
}
*/