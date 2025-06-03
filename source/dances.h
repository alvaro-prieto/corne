#pragma once

#define CUSTOM_TAP_FN(user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset, user_user_data) \
				{ .fn = {user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset}, .user_data = (void*)user_user_data, }

enum td_codes {
	TDNULL = 0,
	RSET,
	CMNT,
	LN,
	TAGO,
	TAGC,
	DSK1,
	DSK2,
	DSK3,
	SCR,
	EXIT,
	GRTR,
	LESS,
	STR12,
    STR34,
	CP,
	PST,
	DLT,
    BOOT,
    BOX,
};

enum tapType{
	BASIC_TAP = 1,
	BASIC_OS_TAP,
	SEQUENCE_TAP,
	CUSTOM_TAP,
	REPLACE_TAP,
	REPLACE_SEQ_TAP,
};

enum customTap{
	NULLTAP = 0,
	RESET_KB,
	CLOSE_WINDOW,
	CLOSE_APP,
	STR1,
	STR2,
    STR3,
    STR4,
};
