#pragma once

#include <GL\glew.h>

namespace oglt {

// define oglt options
#define OGLT_NONE 0x0000
#define OGLT_DEPTH 0x0001
#define OGLT_STENCIL  0x0002
#define OGLT_FULLSCREEN 0x0004

#define OGLT_RENDER_NONE 0X0000
#define OGLT_RENDER_SELF 0x0001
#define OGLT_RENDER_CHILDREN 0x002

#define OGLT_UPDATE_CAMERA_ROTATE 0x0001
#define OGLT_UPDATEA_CAMERA_WALK 0x0002

#define OGLT_INVALID_SHADER_ID 0xFFFFFFFF
#define OGLT_INVALID_MATERIAL_ID 0xFFFFFFFF
#define OGLT_INVALID_MATERIAL_INDEX 0xFFFFFFFF
#define OGLT_INVALID_TEXTURE_ID 0xFFFFFFFF
#define OGLT_INVALID_TEXTURE_INDEX 0xFFFFFFFF
#define OGLT_INVALID_SKYBOX_ID 0xFFFFFFFF

	typedef unsigned int uint;

	enum OGLT_BUTTON {
		OGLT_BUTTON_RIGHT,
		OGLT_BUTTON_MIDDLE,
		OGLT_BUTTON_LEFT,
		OGLT_BUTTON_UNDIFINED = 999
	};

	enum OGLT_BUTTON_STATE {
		OGLT_BUTTON_DOWN,
		OGLT_BUTTON_UP,
		OGLT_BUTTON_STATE_UNDEFINED = 999
	};

	enum OGLT_KEY
	{
		OGLT_KEY_SPACE = 32,
		OGLT_KEY_APOSTROPHE = 39,
		OGLT_KEY_PLUS = 43,
		OGLT_KEY_COMMA = 44,
		OGLT_KEY_MINUS = 45,
		OGLT_KEY_PERIOD = 46,
		OGLT_KEY_SLASH = 47,
		OGLT_KEY_0 = 48,
		OGLT_KEY_1 = 49,
		OGLT_KEY_2 = 50,
		OGLT_KEY_3 = 51,
		OGLT_KEY_4 = 52,
		OGLT_KEY_5 = 53,
		OGLT_KEY_6 = 54,
		OGLT_KEY_7 = 55,
		OGLT_KEY_8 = 56,
		OGLT_KEY_9 = 57,
		OGLT_KEY_SEMICOLON = 58,
		OGLT_KEY_EQUAL = 61,
		OGLT_KEY_A = 65,
		OGLT_KEY_B = 66,
		OGLT_KEY_C = 67,
		OGLT_KEY_D = 68,
		OGLT_KEY_E = 69,
		OGLT_KEY_F = 70,
		OGLT_KEY_G = 71,
		OGLT_KEY_H = 72,
		OGLT_KEY_I = 73,
		OGLT_KEY_J = 74,
		OGLT_KEY_K = 75,
		OGLDEV_KEY_L = 76,
		OGLT_KEY_M = 77,
		OGLT_KEY_N = 78,
		OGLT_KEY_O = 79,
		OGLT_KEY_P = 80,
		OGLT_KEY_Q = 81,
		OGLT_KEY_R = 82,
		OGLT_KEY_S = 83,
		OGLT_KEY_T = 84,
		OGLT_KEY_U = 85,
		OGLT_KEY_V = 86,
		OGLT_KEY_W = 87,
		OGLT_KEY_X = 88,
		OGLT_KEY_Y = 89,
		OGLT_KEY_Z = 90,
		OGLT_KEY_LEFT_BRACKET = 91,
		OGLT_KEY_BACKSLASH = 92,
		OGLT_KEY_RIGHT_BRACKET = 93,
		OGLT_KEY_a = 97,
		OGLT_KEY_b = 98,
		OGLT_KEY_c = 99,
		OGLT_KEY_d = 100,
		OGLT_KEY_e = 101,
		OGLT_KEY_f = 102,
		OGLT_KEY_g = 103,
		OGLT_KEY_h = 104,
		OGLT_KEY_i = 105,
		OGLT_KEY_j = 106,
		OGLT_KEY_k = 107,
		OGLT_KEY_l = 108,
		OGLT_KEY_m = 109,
		OGLT_KEY_n = 110,
		OGLT_KEY_o = 111,
		OGLT_KEY_p = 112,
		OGLT_KEY_q = 113,
		OGLT_KEY_r = 114,
		OGLT_KEY_s = 115,
		OGLT_KEY_t = 116,
		OGLT_KEY_u = 117,
		OGLT_KEY_v = 118,
		OGLT_KEY_w = 119,
		OGLT_KEY_x = 120,
		OGLT_KEY_y = 121,
		OGLT_KEY_z = 122,
		OGLT_KEY_ESCAPE,
		OGLT_KEY_ENTER,
		OGLT_KEY_TAB,
		OGLT_KEY_BACKSPACE,
		OGLT_KEY_INSERT,
		OGLT_KEY_DELETE,
		OGLT_KEY_RIGHT,
		OGLT_KEY_LEFT,
		OGLT_KEY_DOWN,
		OGLT_KEY_UP,
		OGLT_KEY_PAGE_UP,
		OGLT_KEY_PAGE_DOWN,
		OGLT_KEY_HOME,
		OGLT_KEY_END,
		OGLT_KEY_F1,
		OGLT_KEY_F2,
		OGLT_KEY_F3,
		OGLT_KEY_F4,
		OGLT_KEY_F5,
		OGLT_KEY_F6,
		OGLT_KEY_F7,
		OGLT_KEY_F8,
		OGLT_KEY_F9,
		OGLT_KEY_F10,
		OGLT_KEY_F11,
		OGLT_KEY_F12,
		OGLT_KEY_UNDEFINED = 999,
	};

	enum OGLT_KEY_STATE {
		OGLT_KEY_PRESS,
		OGLT_KEY_RELEASE
	};

	enum OGLT_MODEL_TYPE {
		OGLT_ASSIMP_MODEL,
		OGLT_NONE_MODEL = 999
	};
}