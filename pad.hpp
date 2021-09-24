#pragma once
#include <tamtypes.h>
#include "libpad.h"

// Pretty much all we need
typedef enum pad_input_state
{
	BTN_X,
	BTN_O,
	BTN_DOWN,
	BTN_UP,
	BTN_NONE,
} pad_input_state_t;


void init_pad(void);

pad_input_state_t pad_get_input_state(void);

u32 pad_do_i_leave(void);