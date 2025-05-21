#ifdef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:console")
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "MemoryTracker.h"
#include <memory>
#include "Application.h"
#include "screens/MainMenu.h"

MemoryMetrics memoryMetrics;

#ifdef _DEBUG
void* operator new(size_t size) {

	memoryMetrics.m_totalAllocated += size;
	memoryMetrics.m_countAllocated++;

	return malloc(size);

}
void operator delete(void* memory, size_t size) {

	memoryMetrics.m_totalFreed += size;
	memoryMetrics.m_countAllocated--;

	free(memory);
}
#endif

int main(int argc, char* argv[]) {
	
	{
		Application app(memoryMetrics);

		app.init();

		app.setupWindow();

		app.setupApp();

		app.setActiveScreen(std::make_unique<MainMenu>(&app));

		memoryMetrics.report();

		app.startLoop();

		memoryMetrics.report();

		app.close();
	}

	memoryMetrics.report();

	return 0;
}
