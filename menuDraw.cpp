#include "menu.hpp"
#include "birdy/birdy.hpp"

GSGLOBAL* gsGlobal;
GSFONTM* gsFontM;

using namespace Menu;

void Menu::InitGS()
{
	GIFCTRL = 1;

	gsGlobal = gsKit_init_global();
	gsFontM = gsKit_init_fontm();

	dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
		D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

	dmaKit_chan_init(DMA_CHANNEL_GIF);

	gsGlobal->PSMZ = GS_PSMZ_16S;
	gsGlobal->ZBuffering = GS_SETTING_ON;
	gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
	gsKit_init_screen(gsGlobal);

	gsKit_fontm_upload(gsGlobal, gsFontM);
	gsFontM->Spacing = 0.7f;


	printf("Initializing our menu, using defaults. (%dx%d)\n", gsGlobal->Width, gsGlobal->Height);

	gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(0x0, 0xcc, 0xcc, 0x60, 0x0));

	gsKit_queue_exec(gsGlobal);
	gsKit_sync_flip(gsGlobal);

	Birdy::VU1::Initialize();
}

void Menu::Loop()
{
	float height = gsGlobal->Height;
	float width = gsGlobal->Width;

	gsKit_mode_switch(gsGlobal, GS_ONESHOT);
	u64 bg_colour = GS_SETREG_RGBAQ(0x0, 0xcc, 0xcc, 0x0, 0x0);
	const u64 sel_colour = GS_SETREG_RGBAQ(0xff, 0x40, 0x40, 0x67, 0x0);
	const u64 unsel_colour = GS_SETREG_RGBAQ(0x40, 0x40, 0xff, 0x67, 0x0);

	gsFontM->Align = GSKIT_FALIGN_CENTER;

	u32 selection = 0;
	while (1)
	{
		gsGlobal->PrimAlphaEnable = false;
		gsKit_clear(gsGlobal, bg_colour);
		gsGlobal->PrimAlphaEnable = true;
		
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

		{
			using namespace Pad;
			ButtonState bs = readButtonState();
			switch (bs)
			{
				case ButtonState::UP:
					if (selection == 0)
						selection = currentLevel->children.size() - 1;
					else
						selection--;
					break;
				case ButtonState::DOWN:
					if (selection == currentLevel->children.size() - 1)
						selection = 0;
					else
						selection++;
					break;
				case ButtonState::X:
				{
					MenuObject* nextLevel = currentLevel->children.at(selection);

					if (nextLevel->type == ObjectType::CATEGORY)
					{
						currentLevel->clickHandler(selection);
						currentLevel = nextLevel;
					}
					else
					{
						nextLevel->clickHandler(selection);
						if(nextLevel->resetVU1)
							Birdy::VU1::Initialize();
					}
					selection = 0;
					break;
				}
				case ButtonState::O:
				{
					currentLevel = currentLevel->parent;
					selection = 0;
				}
				break;
				default:
					// ..
					break;
			}
		}

		gsFontM->Align = GSKIT_FALIGN_LEFT;
		gsKit_fontm_print_scaled(gsGlobal, gsFontM, 0, height - 70, 1, 0.8f, GS_SETREG_RGBA(0x62, 0x73, 0xa8, 0x40), "X - Enter");

		if (currentLevel != &topLevel)
			gsKit_fontm_print_scaled(gsGlobal, gsFontM, 0, height - 35, 1, 0.8f, GS_SETREG_RGBA(0xbb, 0x23, 0x22, 0x40), "O - Back");
		gsFontM->Align = GSKIT_FALIGN_CENTER;

		gsKit_queue_exec(gsGlobal);
		Birdy::VU1::Frame();
		gsKit_sync_flip(gsGlobal);
		
	}
};
