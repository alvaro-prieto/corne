#pragma once

#define ACTION_TAP_DANCE_FN_ADVANCED_USER(user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset, user_user_data) \
				{ .fn = {user_fn_on_each_tap, user_fn_on_dance_finished, user_fn_on_dance_reset}, .user_data = (void*)user_user_data, }

enum td_codes {
	RSET = 0,
	AC_A,
	AC_E,
	AC_I,
	AC_O,
	AC_U,	
	//ARROWS,
	COMMNT,
	LINES,
	TAG_O,
	TAG_C,
	DSK1,
	DSK2,
	DSK3,
	ORD,
	SCR,
	EXIT,
	GRTR,
	LESS,
	NTEQ,
	GRTP,
	LSTP,
	STR12,
	APPS,
	CP,
	PST,

};

enum tapType{
	BASIC_TAP = 1,
	BASIC_OS_TAP,
	SEQUENCE_TAP,
	ACCENT_TAP,
	CUSTOM_TAP,
};

enum customTap{
	NULLTAP = 0,
	LINE1,
	LINE2,
	RESET_KB,
	CLOSE_WINDOW,
	CLOSE_APP,
	STR1,
	STR2,
};
