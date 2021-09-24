#include "menu.hpp"

// Use this for categories that don't rely on 
// custom click functionality
// Settings the clickHandler object to 0x0 will crash!
void handlerNULL(u32 sel)
{};

void handlerEEBenchArith(u32 ignored)
{
}

void handlerEEBenchArith2(u32 ignored)
{
}

void handlerEEBenchSmallBlock(u32 ignored)
{
}

void handlerEEBenchSDXLDX(u32 ignored)
{
}

void handlerEEMiscRecKiller(u32 sel)
{
}

void handlerVUBenchGeneric(u32 sel)
{

}

void handlerVUBenchRegpressure(u32 sel)
{

}

void handlerVUMiscIBit(u32 sel)
{

}

void handlerAbout(u32 sel)
{
}

void Menu::Load()
{
	printf("[ee] Starting menu load\n");
	currentLevel = &topLevel;
	topLevel.parent = &topLevel;
	topLevel.clickHandler = &handlerNULL;
	topLevel.title = "Fobesmark\n";

	MenuObject* catEE = new MenuObject(&topLevel, ObjectType::CATEGORY, "EE", &handlerNULL);
	{

		MenuObject* catEEBench = new MenuObject(catEE, ObjectType::CATEGORY, "EE Benchmarks", &handlerNULL);
		{
			MenuObject* arithBasic = new MenuObject(catEEBench, ObjectType::FUNCTION, "Basic Arithmetic", &handlerEEBenchArith);

			MenuObject* arithDouble = new MenuObject(catEEBench, ObjectType::FUNCTION, "Doubleword Arithmetic", &handlerEEBenchArith2);

			MenuObject* smallBlocks = new MenuObject(catEEBench, ObjectType::FUNCTION, "Small recompiler blocks", &handlerEEBenchSmallBlock);

			MenuObject* sdxldx = new MenuObject(catEEBench, ObjectType::FUNCTION, "LD(x) and SD(x)", &handlerEEBenchSDXLDX);
		}

		MenuObject* catEEMisc = new MenuObject(catEE, ObjectType::CATEGORY, "EE Misc", &handlerNULL);
		{
			MenuObject* recKiller = new MenuObject(catEEMisc, ObjectType::FUNCTION, "EERec Killer", &handlerEEMiscRecKiller);
		}
	}

	MenuObject* catVU = new MenuObject(&topLevel, ObjectType::CATEGORY, "VU", &handlerNULL);
		{
			MenuObject* catVUBench = new MenuObject(catVU,ObjectType::CATEGORY,"VU Benchmarks",&handlerNULL);
			{
				MenuObject* generic = new MenuObject(catVUBench,ObjectType::FUNCTION,"Generic",&handlerVUBenchGeneric);

				MenuObject* regPressure = new MenuObject(catVUBench,ObjectType::FUNCTION,"Register pressure",&handlerVUBenchRegpressure);
			}

			MenuObject* catVUMisc = new MenuObject(catVU,ObjectType::CATEGORY,"VU Misc",&handlerNULL);
			{
				MenuObject* iBitRecomp = new MenuObject(catVUMisc,ObjectType::FUNCTION,"IBit Recompilation",&handlerVUMiscIBit);
			}
		}

	MenuObject* funcAbout = new MenuObject(&topLevel, ObjectType::CATEGORY, "About", &handlerAbout);
	printf("[ee] Finished menu load!\n");
	return;
}



#include <gsKit.h>

GSGLOBAL* gsGlobal;
GSFONTM* gsFontM;

// I've never a menu before, bare with me
enum menu_location
{
	locs_MENU_MAIN,
	locs_MENU_EE,
	locs_MENU_EE_BENCH,
	locs_MENU_EE_STUPID,
	locs_MENU_VU,
	locs_MENU_VU_BENCH,
	locs_MENU_VU_STUPID,
	locs_MENU_COUNT,
} our_menu_location,
	our_last_menu_location;

const char* l_section_titles[locs_MENU_COUNT] =
	{
		"",
		"EE",
		"EE Benchmarks",
		"EE Stupid Stuff",
		"VU",
		"VU Benchmarks",
		"VU Stupid Stuff",
};

u32 our_current_selection = 0;

// Why didn't I do this in C++ ???

enum main_menu_location
{
	MENU_MAIN_EE,
	MENU_MAIN_VU,
	MENU_MAIN_EXIT,
	MENU_MAIN_COUNT,
};
const char* l_main_sections[MENU_MAIN_COUNT] =
	{
		"EE",
		"VU",
		"Exit",
};

void handle_main_section_click(u32 selection)
{
	switch (selection)
	{
		case MENU_MAIN_EE:
			our_menu_location = locs_MENU_EE;
			break;
		case MENU_MAIN_VU:
			our_menu_location = locs_MENU_VU;
			break;
	}
}

enum ee_menu_location
{
	MENU_EE_BENCHING,
	MENU_EE_STUPID,
	MENU_EE_BACK,
	MENU_EE_COUNT,
};
const char* l_ee_sections[MENU_EE_COUNT] =
	{
		"Benching (4)",
		"Stupid stuff (1)",
		"Back",
};

void handle_ee_section_click(u32 selection)
{
	switch (selection)
	{
		case MENU_EE_BENCHING:
			our_menu_location = locs_MENU_EE_BENCH;
			break;
		case MENU_EE_STUPID:
			our_menu_location = locs_MENU_EE_STUPID;
			break;
		case MENU_EE_BACK:
			our_menu_location = locs_MENU_MAIN;
			break;
	}
}

enum ee_menu_bench_location
{
	MENU_EE_BENCH_BASIC,
	MENU_EE_BENCH_SMALLBLOCK,
	MENU_EE_BENCH_SDXLDX,
	MENU_EE_BENCH_DWA,
	MENU_EE_BENCH_BACK,
	MENU_EE_BENCH_COUNT,
};
const char* l_ee_bench_sections[MENU_EE_BENCH_COUNT] =
	{
		"Basic arithmetic",
		"Small recompiler blocks",
		"SDL/SDR & LDL/LDR benching",
		"Double word arithmetic",
		"Back",
};

void handle_ee_bench_section_click(u32 selection)
{
	switch (selection)
	{
		case MENU_EE_BENCH_BASIC:
			eebench_loopArithmetic();
			break;
		case MENU_EE_BENCH_SMALLBLOCK:
			eebench_smallBlocks();
			break;
		case MENU_EE_BENCH_SDXLDX:
			eebench_SDXLDXFallback();
			break;
		case MENU_EE_BENCH_DWA:
			eebench_dwArithmetic();
			break;
		case MENU_EE_BENCH_BACK:
			our_menu_location = locs_MENU_EE;
			break;
	}
}

enum ee_menu_stupid_location
{
	MENU_EE_STUPID_RECKILLER,
	MENU_EE_STUPID_BACK,
	MENU_EE_STUPID_COUNT,
};
const char* l_ee_stupid_sections[MENU_EE_STUPID_COUNT] =
	{
		"Recompiler killer (really dumb)",
		"Back",
};

void handle_ee_stupid_section_click(u32 selection)
{
	switch (selection)
	{
		case MENU_EE_STUPID_RECKILLER:
			eestupid_recKiller();
			break;
		case MENU_EE_STUPID_BACK:
			our_menu_location = locs_MENU_EE;
			break;
	}
}


enum vu_menu_location
{
	MENU_VU_BENCHING,
	MENU_VU_STUPID,
	MENU_VU_BACK,
	MENU_VU_COUNT,
};
const char* l_vu_sections[MENU_VU_COUNT] =
	{
		"Benching (2)",
		"Stupid stuff (1)",
		"Back",
};

// There has to be a better way to do this right??
// This is getting old
void handle_vu_section_click(u32 selection)
{
	switch (selection)
	{
		case MENU_VU_BENCHING:
			our_menu_location = locs_MENU_VU_BENCH;
			break;
		case MENU_VU_STUPID:
			our_menu_location = locs_MENU_VU_STUPID;
			break;
		case MENU_VU_BACK:
			our_menu_location = locs_MENU_MAIN;
			break;
	}
}

enum vu_menu_bench_location
{
	MENU_VU_BENCH_GENERIC,
	MENU_VU_BENCH_REGPRESSURE,
	MENU_VU_BENCH_BACK,
	MENU_VU_BENCH_COUNT,
};
const char* l_vu_bench_sections[MENU_VU_BENCH_COUNT] =
	{
		"Generic stress",
		"Register pressure",
		"Back",
};

void handle_vu_bench_section_click(u32 selection)
{
	switch (selection)
	{
		case MENU_VU_BENCH_GENERIC:
			vubench_generic();
			break;
		case MENU_VU_BENCH_REGPRESSURE:
			vubench_registerPressure();
			break;
		case MENU_VU_BENCH_BACK:
			our_menu_location = locs_MENU_VU;
			break;
	}
}

enum vu_menu_stupid_location
{
	MENU_VU_STUPID_RECOMPILATION,
	MENU_VU_STUPID_BACK,
	MENU_VU_STUPID_COUNT,
};
const char* l_vu_stupid_sections[MENU_VU_STUPID_COUNT] =
	{
		"IBit Recompilation",
		"Back",
};

void handle_vu_stupid_section_click(u32 selection)
{
	switch (selection)
	{
		case MENU_VU_STUPID_RECOMPILATION:
			vustupid_IBitRecompilation();
			break;
		case MENU_VU_STUPID_BACK:
			our_menu_location = locs_MENU_VU;
			break;
	}
}

// An easy way to delay, making it easier to navigate the menu without analogs going BRR
// Not needed anymore, but can be useful in the future??
void delay_vsync(u32 count)
{
	for (u32 i = 0; i < count; i++)
	{
		gsKit_vsync_wait();
	}
}

// Required for VU benching, idk why, do we mess up the GIF somehow???
void menu_reset_gsKit(void)
{
	gsKit_init_screen(gsGlobal);
}

void menu_init(void)
{
	GIFCTRL = 1;

	gsGlobal = gsKit_init_global();
	gsFontM = gsKit_init_fontm();

	dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
		D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

	dmaKit_chan_init(DMA_CHANNEL_GIF);

	gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
	gsKit_init_screen(gsGlobal);

	gsKit_fontm_upload(gsGlobal, gsFontM);
	gsFontM->Spacing = 0.7f;

	printf("Initializing our menu, using defaults. (%dx%d)\n", gsGlobal->Width, gsGlobal->Height);

	gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(0x0, 0xcc, 0xcc, 0x60, 0x0));

	gsKit_queue_exec(gsGlobal);
	gsKit_sync_flip(gsGlobal);

	our_menu_location = locs_MENU_MAIN;
	menu_loop();
}

using namespace Menu;
void menu_loop(void)
{
	//printf("in the loop\n");
	// Will spent our time here if we aren't doing anything :)

	float height = gsGlobal->Height;
	float width = gsGlobal->Width;

	gsKit_mode_switch(gsGlobal, GS_ONESHOT);
	u64 bg_colour = GS_SETREG_RGBAQ(0x0, 0xcc, 0xcc, 0x60, 0x0);
	const u64 sel_colour = GS_SETREG_RGBAQ(0xff, 0x40, 0x40, 0x40, 0x0);
	const u64 unsel_colour = GS_SETREG_RGBAQ(0x40, 0x40, 0xff, 0x40, 0x0);

	gsFontM->Align = GSKIT_FALIGN_CENTER;

	u32 selection = 0;
	while (1)
	{
		gsKit_clear(gsGlobal, bg_colour);

		gsKit_fontm_print_scaled(gsGlobal, gsFontM, width / 2, 2, 1, 1.0f, sel_colour, currentLevel->title.c_str());
		float optionY = 150;

		u32 iterSelection = 0;
		for (MenuObject* currentLevelChild : currentLevel->children)
		{
			if (iterSelection == selection)
				gsKit_fontm_print_scaled(gsGlobal, gsFontM, width / 2, optionY, 1, 0.8f, sel_colour, currentLevelChild->title.c_str());
			else
				gsKit_fontm_print_scaled(gsGlobal, gsFontM, width / 2, optionY, 1, 0.8f, unsel_colour, currentLevelChild->title.c_str());

			iterSelection++;
			optionY += 30;
		}

		pad_input_state_t pis = pad_get_input_state();
		switch (pis)
		{
			case BTN_UP:
				if (selection == 0)
					selection = currentLevel->children.size() - 1;
				else
					selection--;
				break;
			case BTN_DOWN:
				if (selection == currentLevel->children.size() - 1)
					selection = 0;
				else
					selection++;
				break;
			case BTN_X:
			{
				currentLevel = currentLevel->children.at(selection);
				selection = 0;
				break;
			}
			case BTN_O:
			{
				currentLevel = currentLevel->parent;
				selection = 0;
			}
			break;
			default:
				// ..
				break;
		}
		gsKit_queue_exec(gsGlobal);
		gsKit_sync_flip(gsGlobal);
	}
}