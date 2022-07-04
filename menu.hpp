#pragma once
#include <string>
#include <vector>
#include <functional>
#include <gsKit.h> // GS_SETREG_RGBAQ

#include "ee/ee.hpp"
#include "vu/vu.hpp"
#include "gs/gs.hpp"
#include "birdy/birdy.hpp"
#include "pad.hpp"

void menu_reset_gsKit(void);
void menu_init(void);

void menu_loop(void);

namespace Menu
{
	struct Colour
	{
		u16 R, G, B, A;
		Colour(u16 r, u16 g, u16 b, u16 a)
			: R(r)
			, G(g)
			, B(b)
			, A(a){};
		Colour()
			: R(0)
			, G(0)
			, B(0)
			, A(0){};
		u64 toSETREG() { return GS_SETREG_RGBAQ(this->R, this->G, this->B, this->A, 0x0); }
	};

	enum class ObjectType
	{
		CATEGORY,
		FUNCTION,
	};

	class MenuObject
	{
	public:
		bool isTitlePage;
		MenuObject* parent;
		// Category only feature
		std::vector<MenuObject*> children;
		std::string title;
		ObjectType type;
		std::function<void(u32)> clickHandler;
		bool resetVU1;
		MenuObject()
		{
		}

		MenuObject(MenuObject* parent)
			: parent(parent)
		{
			parent->children.push_back(this);
		}

		MenuObject(MenuObject* parent, ObjectType objType, const std::string title, std::function<void(u32)> clickHandler, bool resetVU1State = false)
			: parent(parent)
			, title(title)
			, type(objType)
			, clickHandler(clickHandler)
			, resetVU1(resetVU1State)
		{
			parent->children.push_back(this);
		};
	};

	extern MenuObject topLevel;
	extern MenuObject* currentLevel;

	void Load();
	void InitGS();
	void Loop();
} // namespace Menu
