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

GSGLOBAL* gsGlobal;
GSFONTM* gsFontM;


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

	menu_loop();
}

using namespace Menu;
void menu_loop(void)
{
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