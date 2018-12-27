#include "pch.h"
#include "keyboard.h"

int keyState[121 * 2];

int keyLookup(int key)
{
	int ret;
	switch (key)
	{
	case GLFW_KEY_UNKNOWN:
		ret = 0;
		break;
	case GLFW_KEY_SPACE:
		ret = 1;
		break;
	case GLFW_KEY_APOSTROPHE:
		ret = 2;
		break;
	case GLFW_KEY_COMMA:
		ret = 3;
		break;
	case GLFW_KEY_MINUS:
		ret = 4;
		break;
	case GLFW_KEY_PERIOD:
		ret = 5;
		break;
	case GLFW_KEY_SLASH:
		ret = 6;
	case GLFW_KEY_0:
		ret = 7;
		break;
	case GLFW_KEY_1:
		ret = 8;
		break;
	case GLFW_KEY_2:
		ret = 9;
		break;
	case GLFW_KEY_3:
		ret = 10;
		break;
	case GLFW_KEY_4:
		ret = 11;
		break;
	case GLFW_KEY_5:
		ret = 12;
		break;
	case GLFW_KEY_6:
		ret = 13;
		break;
	case GLFW_KEY_7:
		ret = 14;
		break;
	case GLFW_KEY_8:
		ret = 15;
		break;
	case GLFW_KEY_9:
		ret = 16;
		break;
	case GLFW_KEY_SEMICOLON:
		ret = 17;
		break;
	case GLFW_KEY_EQUAL:
		ret = 18;
		break;
	case GLFW_KEY_A:
		ret = 19;
		break;
	case GLFW_KEY_B:
		ret = 20;
		break;
	case GLFW_KEY_C:
		ret = 21;
		break;
	case GLFW_KEY_D:
		ret = 22;
		break;
	case GLFW_KEY_E:
		ret = 23;
		break;
	case GLFW_KEY_F:
		ret = 24;
		break;
	case GLFW_KEY_G:
		ret = 25;
		break;
	case GLFW_KEY_H:
		ret = 26;
		break;
	case GLFW_KEY_I:
		ret = 27;
		break;
	case GLFW_KEY_J:
		ret = 28;
		break;
	case GLFW_KEY_K:
		ret = 29;
		break;
	case GLFW_KEY_L:
		ret = 30;
		break;
	case GLFW_KEY_M:
		ret = 31;
		break;
	case GLFW_KEY_N:
		ret = 32;
		break;
	case GLFW_KEY_O:
		ret = 33;
		break;
	case GLFW_KEY_P:
		ret = 34;
		break;
	case GLFW_KEY_Q:
		ret = 35;
		break;
	case GLFW_KEY_R:
		ret = 36;
		break;
	case GLFW_KEY_S:
		ret = 37;
		break;
	case GLFW_KEY_T:
		ret = 38;
		break;
	case GLFW_KEY_U:
		ret = 39;
		break;
	case GLFW_KEY_V:
		ret = 40;
		break;
	case GLFW_KEY_W:
		ret = 41;
		break;
	case GLFW_KEY_X:
		ret = 42;
		break;
	case GLFW_KEY_Y:
		ret = 43;
		break;
	case GLFW_KEY_Z:
		ret = 44;
		break;
	case GLFW_KEY_LEFT_BRACKET:
		ret = 45;
		break;
	case GLFW_KEY_BACKSLASH:
		ret = 46;
		break;
	case GLFW_KEY_RIGHT_BRACKET:
		ret = 47;
		break;
	case GLFW_KEY_GRAVE_ACCENT:
		ret = 48;
		break;
	case GLFW_KEY_WORLD_1:
		ret = 49;
		break;
	case GLFW_KEY_WORLD_2:
		ret = 50;
		break;
	case GLFW_KEY_ESCAPE:
		ret = 51;
		break;
	case GLFW_KEY_ENTER:
		ret = 52;
		break;
	case GLFW_KEY_TAB:
		ret = 53;
		break;
	case GLFW_KEY_BACKSPACE:
		ret = 54;
		break;
	case GLFW_KEY_INSERT:
		ret = 55;
		break;
	case GLFW_KEY_DELETE:
		ret = 56;
		break;
	case GLFW_KEY_RIGHT:
		ret = 57;
		break;
	case GLFW_KEY_LEFT:
		ret = 58;
		break;
	case GLFW_KEY_DOWN:
		ret = 59;
		break;
	case GLFW_KEY_UP:
		ret = 60;
		break;
	case GLFW_KEY_PAGE_UP:
		ret = 61;
		break;
	case GLFW_KEY_PAGE_DOWN:
		ret = 62;
		break;
	case GLFW_KEY_HOME:
		ret = 63;
		break;
	case GLFW_KEY_END:
		ret = 64;
		break;
	case GLFW_KEY_CAPS_LOCK:
		ret = 65;
		break;
	case GLFW_KEY_SCROLL_LOCK:
		ret = 66;
		break;
	case GLFW_KEY_NUM_LOCK:
		ret = 67;
		break;
	case GLFW_KEY_PRINT_SCREEN:
		ret = 68;
		break;
	case GLFW_KEY_PAUSE:
		ret = 69;
		break;
	case GLFW_KEY_F1:
		ret = 70;
		break;
	case GLFW_KEY_F2:
		ret = 71;
		break;
	case GLFW_KEY_F3:
		ret = 72;
		break;
	case GLFW_KEY_F4:
		ret = 73;
		break;
	case GLFW_KEY_F5:
		ret = 74;
		break;
	case GLFW_KEY_F6:
		ret = 75;
		break;
	case GLFW_KEY_F7:
		ret = 76;
		break;
	case GLFW_KEY_F8:
		ret = 77;
		break;
	case GLFW_KEY_F9:
		ret = 78;
		break;
	case GLFW_KEY_F10:
		ret = 79;
		break;
	case GLFW_KEY_F11:
		ret = 80;
		break;
	case GLFW_KEY_F12:
		ret = 81;
		break;
	case GLFW_KEY_F13:
		ret = 82;
		break;
	case GLFW_KEY_F14:
		ret = 83;
		break;
	case GLFW_KEY_F15:
		ret = 84;
		break;
	case GLFW_KEY_F16:
		ret = 85;
		break;
	case GLFW_KEY_F17:
		ret = 86;
		break;
	case GLFW_KEY_F18:
		ret = 87;
		break;
	case GLFW_KEY_F19:
		ret = 88;
		break;
	case GLFW_KEY_F20:
		ret = 89;
		break;
	case GLFW_KEY_F21:
		ret = 90;
		break;
	case GLFW_KEY_F22:
		ret = 91;
		break;
	case GLFW_KEY_F23:
		ret = 92;
		break;
	case GLFW_KEY_F24:
		ret = 93;
		break;
	case GLFW_KEY_F25:
		ret = 94;
		break;
	case GLFW_KEY_KP_0:
		ret = 95;
		break;
	case GLFW_KEY_KP_1:
		ret = 96;
		break;
	case GLFW_KEY_KP_2:
		ret = 97;
		break;
	case GLFW_KEY_KP_3:
		ret = 98;
		break;
	case GLFW_KEY_KP_4:
		ret = 99;
		break;
	case GLFW_KEY_KP_5:
		ret = 100;
		break;
	case GLFW_KEY_KP_6:
		ret = 101;
		break;
	case GLFW_KEY_KP_7:
		ret = 102;
		break;
	case GLFW_KEY_KP_8:
		ret = 103;
		break;
	case GLFW_KEY_KP_9:
		ret = 104;
		break;
	case GLFW_KEY_KP_DECIMAL:
		ret = 105;
		break;
	case GLFW_KEY_KP_DIVIDE:
		ret = 106;
		break;
	case GLFW_KEY_KP_MULTIPLY:
		ret = 107;
		break;
	case GLFW_KEY_KP_SUBTRACT:
		ret = 108;
		break;
	case GLFW_KEY_KP_ADD:
		ret = 109;
		break;
	case GLFW_KEY_KP_ENTER:
		ret = 110;
		break;
	case GLFW_KEY_KP_EQUAL:
		ret = 111;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		ret = 112;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		ret = 113;
		break;
	case GLFW_KEY_LEFT_ALT:
		ret = 114;
		break;
	case GLFW_KEY_LEFT_SUPER:
		ret = 115;
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		ret = 116;
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		ret = 117;
		break;
	case GLFW_KEY_RIGHT_ALT:
		ret = 118;
		break;
	case GLFW_KEY_RIGHT_SUPER:
		ret = 119;
		break;
	case GLFW_KEY_MENU:
		ret = 120;
		break;
	default:
		ret = 0;
		break;
	}

	return ret * 2;
}

int keyPressedOnce(int key)
{
	int keyCode  = keyLookup(key);
	int *current = &keyState[keyCode];
	int *last    = &keyState[keyCode + 1];

	if (*current == GLFW_PRESS && *last == GLFW_RELEASE)
	{
		*last = GLFW_PRESS;
		return true;
	}
	if (*current == GLFW_RELEASE && *last == GLFW_PRESS)
	{
		*last = GLFW_RELEASE;
		return false;
	}
	return false;
}

int keyReleasedOnce(int key)
{
	int keyCode  = keyLookup(key);
	int *current = &keyState[keyCode];
	int *last    = &keyState[keyCode + 1];

	if (*current == GLFW_PRESS && *last == GLFW_RELEASE)
	{
		*last = GLFW_PRESS;
		return false;
	}
	if (*current == GLFW_RELEASE && *last == GLFW_PRESS)
	{
		*last = GLFW_RELEASE;
		return true;
	}
	return false;
}

int keyPressed(int key)
{
	int keyCode = keyLookup(key);
	int keyStatus = keyState[keyCode];
	return (keyStatus == GLFW_PRESS);
}

int keyReleased(int key)
{
	int keyCode = keyLookup(key);
	int keyStatus = keyState[keyCode];
	return (keyStatus == GLFW_RELEASE);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	int keyCode = keyLookup(key);
	keyState[keyCode] = action;
}
