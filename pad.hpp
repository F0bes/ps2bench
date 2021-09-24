#pragma once
#include <tamtypes.h>
#include "libpad.h"

// Pretty much all we need
namespace Pad
{

	enum class ButtonState
	{
		X,
		O,
		DOWN,
		UP,
		NONE,
	};


	void init(void);

	ButtonState readButtonState(void);

	bool readButton(ButtonState button);
}; // namespace Pad