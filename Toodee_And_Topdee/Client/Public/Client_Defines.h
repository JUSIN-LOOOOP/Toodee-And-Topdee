#pragma once

#include "../Default/framework.h"
#include <process.h>

/* Ŭ���̾�Ʈ���� ����� �� �ִ� �������� ���Ǹ� ��Ƴ��� ���� */
namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_MAPEDIT, LEVEL_END };

	enum class KEYINPUT { NONE = 0, KEY_UP = 1, KEY_DOWN = 1 << 1, KEY_LEFT = 1 << 2, KEY_RIGHT = 1 << 3, KEY_Z = 1 <<4, KEY_X = 1 << 5, KEY_MOVES = 15};

	enum class PLAYERSTATE { IDLE, MOVE, ACTION, STOP, CLEAR, DEAD, PLAYERSTATE_END };

	enum class TEXTUREDIRECTION { LEFT, RIGHT };

	enum class MOVEDIRECTION { DOWN, TRANSVERSE, DIAGONAL_DOWN, DIAGONAL_UP, UP };

	enum JUMPSTATE { JUMPING, HANGSTART, HANGING, HANGEND, FALLING };

	enum class BLOCKTYPE { NONE, WALL, WOOD, BREAK, LOCK, FALL, BLOCK_END };

	typedef struct tagStateDesc {
		PLAYERSTATE					eState;
		unsigned int				iMaxAnimCount;
	}PLAYERSTATE_DESC;

}

extern HWND g_hWnd;
using namespace Client;


#define GRAVITY 9.8f


/* 0. ��ġ�ӿ� �����е��� ������.  */
/* 1. �˽ѳ�(��). */
/* 1. �����ǽİ�.(��) */
/* 1. ��õ����.(��) */
/* 1. ���̻�, ���̻� */
/* 1. ���¹����ڻ��� 1, 2 .(��) */
/* 1. �߰׸�����2��.(��) */
/* 1. ���ѻڶ���.(��) */
/* 1. ���չ��� ���̵�(����) */
/* 1. ����.(��) */
/* 1. �ڽ�����.(��, ��) */
/* 1. ���Ʈ��. */
/* 1. ������Ӿ�ī���� */
/* 1. ����� ���� */
/* 1. ����� Ż��. (���, �ְ� ������, �ְ� ī��, ) */
