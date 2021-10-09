#include "menu.hpp"

Menu::MenuObject Menu::topLevel;
Menu::MenuObject* Menu::currentLevel;

// Use this for categories that don't rely on
// custom click functionality
// Settings the clickHandler object to 0x0 will crash!
void handlerNULL(u32 sel){};

void handlerEEBenchArith(u32 ignored)
{
	eebench_loopArithmetic();
}

void handlerEEBenchArith2(u32 ignored)
{
	eebench_dwArithmetic();
}

void handlerEEBenchSmallBlock(u32 ignored)
{
	eebench_smallBlocks();
}

void handlerEEBenchSDXLDX(u32 ignored)
{
	eebench_SDXLDXFallback();
}

void handlerEEMiscRecKiller(u32 sel)
{
	eemisc_recKiller();
}

void handlerVUBenchGeneric(u32 sel)
{
	vubench_generic();
}

void handlerVUBenchRegpressure(u32 sel)
{
	vubench_registerPressure();
}

void handlerVUMiscIBit(u32 sel)
{
	vumisc_IBitRecompilation();
}

void handlerAbout(u32 sel)
{
}

void Menu::Load()
{
	printf("[ee] Starting menu load\n");
	currentLevel = &topLevel;
	topLevel.parent = &topLevel;
	topLevel.clickHandler = handlerNULL;
	topLevel.title = "Fobesmark\n";

	MenuObject* catEE = new MenuObject(&topLevel, ObjectType::CATEGORY, "EE", handlerNULL);
	{

		MenuObject* catEEBench = new MenuObject(catEE, ObjectType::CATEGORY, "EE Benchmarks", handlerNULL);
		{
			new MenuObject(catEEBench, ObjectType::FUNCTION, "Basic Arithmetic", handlerEEBenchArith);

			new MenuObject(catEEBench, ObjectType::FUNCTION, "Doubleword Arithmetic", handlerEEBenchArith2);

			new MenuObject(catEEBench, ObjectType::FUNCTION, "Small recompiler blocks", handlerEEBenchSmallBlock);

			new MenuObject(catEEBench, ObjectType::FUNCTION, "LD(x) and SD(x)", handlerEEBenchSDXLDX);
		}

		MenuObject* catEEMisc = new MenuObject(catEE, ObjectType::CATEGORY, "EE Misc", handlerNULL);
		{
			new MenuObject(catEEMisc, ObjectType::FUNCTION, "EERec Killer", handlerEEMiscRecKiller);
		}
	}

	MenuObject* catVU = new MenuObject(&topLevel, ObjectType::CATEGORY, "VU", handlerNULL);
	{
		MenuObject* catVUBench = new MenuObject(catVU, ObjectType::CATEGORY, "VU Benchmarks", handlerNULL);
		{
			new MenuObject(catVUBench, ObjectType::FUNCTION, "Generic", handlerVUBenchGeneric,true);

			new MenuObject(catVUBench, ObjectType::FUNCTION, "Register pressure", handlerVUBenchRegpressure,true);
		}

		MenuObject* catVUMisc = new MenuObject(catVU, ObjectType::CATEGORY, "VU Misc", handlerNULL);
		{
			new MenuObject(catVUMisc, ObjectType::FUNCTION, "IBit Recompilation", handlerVUMiscIBit,true);
		}
	}

	new MenuObject(&topLevel, ObjectType::FUNCTION, "About", handlerAbout);
	printf("[ee] Finished menu load!\n");
	return;
}
