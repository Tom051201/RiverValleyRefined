#include "MemoryTracker.h"

#include <windows.h>
#include <iostream>

MemoryMetrics::MemoryMetrics() {
	m_totalAllocated = 0;
	m_totalFreed = 0;
	m_countAllocated = 0;
}

uint32_t MemoryMetrics::currentHeapUsage() { return m_totalAllocated - m_totalFreed; }

void MemoryMetrics::printHeapUsage() {
	std::cout << "> Current Usage " << currentHeapUsage() << " bytes" << std::endl;
}

void MemoryMetrics::printHeapAllocated() {
	std::cout << "> Unfreed allocations: " << m_countAllocated << " times" << std::endl;
}

void MemoryMetrics::printStackUsage() {
	NT_TIB* pTib = (NT_TIB*)NtCurrentTeb();
	void* stackBase = pTib->StackBase;
	void* stackLimit = pTib->StackLimit;
	void* current = &pTib;  // address of local variable = current stack pointer

	size_t used = (size_t)stackBase - (size_t)current;
	size_t total = (size_t)stackBase - (size_t)stackLimit;

	std::cout << "> Stack used: " << used << " bytes\n";
	std::cout << "> Stack total: " << total << " bytes\n";
}

void MemoryMetrics::report() {
	printHeapUsage();
	printHeapAllocated();
	printStackUsage();
}