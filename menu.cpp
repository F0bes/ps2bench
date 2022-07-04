#include "menu.hpp"

Menu::MenuObject Menu::topLevel;
Menu::MenuObject* Menu::currentLevel;
extern GSGLOBAL* gsGlobal; // HACK
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

void handlerGSmppsSprite(u32 sel)
{
	gs_mpps_sprite();
}

void handlerGSMaxPoly(u32 sel)
{
	gs_maxpoly(false);
}

void handlerGSMaxPolyUnsafe(u32 sel)
{
	gs_maxpoly(true);
}

void handlerBirdyVU1(u32 sel)
{
	Birdy::VU1::Bench(gsGlobal);
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

	MenuObject* catGS = new MenuObject(&topLevel, ObjectType::CATEGORY, "GS", handlerNULL);
	{
		new MenuObject(catGS, ObjectType::FUNCTION, "mpps (sprite)", handlerGSmppsSprite, true);
		new MenuObject(catGS, ObjectType::FUNCTION, "maxpoly (points)", handlerGSMaxPoly, true);
		new MenuObject(catGS, ObjectType::FUNCTION, "maxpoly (points) (unsafe)", handlerGSMaxPolyUnsafe, true);
	}

	MenuObject* catBirdy = new MenuObject(&topLevel,ObjectType::CATEGORY, "Birdy", handlerNULL);
	{
		new MenuObject(catBirdy, ObjectType::FUNCTION,"VU1",handlerBirdyVU1,true);
	}
	new MenuObject(&topLevel, ObjectType::FUNCTION, "About", handlerAbout);
	printf("[ee] Finished menu load!\n");
	return;
}
