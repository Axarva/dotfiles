/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>


/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 12;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 70;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */

static const char *fonts[]          = { "System San Francisco Display:size=10:style=heavy", "JoyPixels:pixelsize=10", "monospace:size=10" };
static const char dmenufont[]       = "System San Francisco Display:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#faebd7";
static const char col_black[]       = "#000000";
static const char col_border[]      = "#ff79c6";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#ff5555";
static const char col_yellow[]      = "#ffff00";
static const char col_smthFG[]      = "#f5fffa";
static const char col_SeparatorBG[] = "#707070";
static const char col_BGgreen[]     = "#008000";
static const char col_smth[]        = "#eee5de";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	// [SchemeNorm] 		=	 { col_gray3,     col_gray1,  		col_black },
	// [SchemeSel]  		=	 { col_black,	  col_border,		col_border },
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeWarn]   		=	 { col_black,     col_yellow, 		col_red },
	[SchemeUrgent]		=	 { col_smthFG, 	  col_red,    		col_red },
	[SchemeTagUnderline] =	 { col_border,    col_border,   	col_border },
	[SchemeBGgreen] 	=	 { col_gray3,  	  col_BGgreen,		col_gray2 },
	[SchemeWnameBG] 	=	 { col_border, 	  col_gray1,		col_red },
	[SchemeBlockSeparator] 	=	 { col_cyan,      col_SeparatorBG,	col_red },
	[SchemeCmusButton] 	=	 { col_smth,	  col_cyan,		col_red },
};

static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
static const char *briup[] = { "/usr/bin/brightnessctl", "s", "+10%", NULL };
static const char *bridown[] = { "/usr/bin/brightnessctl", "s", "10%-", NULL };
static const char *previous[] = { "/usr/bin/playerctl", "previous", NULL };
static const char *playpause[] = { "/usr/bin/playerctl", "play-pause", NULL };
static const char *next[] = { "/usr/bin/playerctl", "next", NULL };
static const char *printflame[] = { "/usr/bin/flameshot", "gui", "-p", "/home/axarva/Desktop", NULL };
static const char *idleinhibit[] = { "/usr/bin/xdotool", "exec", "/home/axarva/bin/foo.sh", NULL };
static const char *noinhibit[] = { "/usr/bin/xdotool", "exec", "/home/axarva/bin/unfoo.sh", NULL };
static const char *goingout[] = { "/usr/bin/nwgbar", "-o", "0.2", "-t", "dwm.json", NULL };

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 1,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "🕸️",      tile },    /* first entry is default */
	{ "💭",      NULL },    /* no layout function means floating behavior */
	{ "🧐",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {  "/bin/sh", "-c", "rofi -show drun -config ~/rofi-spotlight/rofi.rasi", NULL };
static const char *termcmd[]  = { "alacritty", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      spawn,           {.v = goingout} },
	{ 0,                       XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                       XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                       XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
	{ 0, 			   XF86XK_MonBrightnessUp, spawn, {.v = briup    } },
	{ 0,                       XF86XK_MonBrightnessDown, spawn, {.v = bridown  } },
	{ 0,                       XF86XK_AudioPrev, spawn, {.v = previous } },
	{ 0,                       XF86XK_AudioPlay, spawn, {.v = playpause } },
	{ 0,                       XF86XK_AudioNext, spawn, {.v = next } },
	{ 0,			   XK_Print, spawn, {.v = printflame } },
	{ MODKEY,                       XK_z,      spawn,          {.v = idleinhibit } },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = noinhibit } },
	{ MODKEY|ShiftMask, XK_a, spawn, SHCMD("rofi -modi \"clipboard:greenclip print\" -show clipboard -run-command '{cmd}'") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,      movemouse,      {.i = 1} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,      resizemouse,    {.i = 1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

