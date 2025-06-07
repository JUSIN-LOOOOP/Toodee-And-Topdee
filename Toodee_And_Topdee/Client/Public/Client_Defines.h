#pragma once

#include "../Default/framework.h"
#include <process.h>

#define	CLOUD_INTERVAL_POSITION_X		2.f
#define	CLOUD_INTERVAL_POSITION_Y		4.f



/* 클라이언트에서 사용할 수 있는 공통적인 정의를 모아놓은 파일 */
namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;

	enum class LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY,
						LEVEL_STAGE1, LEVEL_STAGE2, LEVEL_STAGE3, LEVEL_STAGEBOSS,
						LEVEL_STAGE4, LEVEL_STAGE5, LEVEL_STAGE6, LEVEL_FINALBOSS,
						LEVEL_MAPEDIT, LEVEL_END };

	enum class KEYINPUT { NONE = 0, KEY_UP = 1, KEY_DOWN = 1 << 1, KEY_LEFT = 1 << 2, KEY_RIGHT = 1 << 3, KEY_Z = 1 <<4, KEY_X = 1 << 5, KEY_MOVES = 15};

	enum class PLAYERSTATE { IDLE, MOVE, ACTION, STOP, CLEAR, DEAD, PLAYERSTATE_END };

	enum class BLOCKSTATE { STOP, PUSH, ATTACH, DETACH, BLOCKSTATE_END};

	enum class TEXTUREDIRECTION { LEFT, RIGHT };

	enum class MOVEDIRECTION { DOWN, TRANSVERSE, DIAGONAL_DOWN, DIAGONAL_UP, UP };

	enum class DETACHDIRECTION { LEFT, UP, RIGHT, DOWN, DETACHDIRECTION_END};

	enum JUMPSTATE { JUMPING, HANGSTART, HANGING, HANGEND, FALLING, VIEWTURN };

	enum class MAPOBJECT { NONE, WALL, WOOD, BREAK, LOCK, FALL, SPARK, METAL, HOLE, KEY, PORTAL, TOPDEE, TOODEE, MAPOBJECT_END };

	enum class STAGEMONERSTATE { IDLE, ATTACK, CHASE, TURN, HIT, FIRE, VIEWTURN, BOOST, DAMAGE };

	enum class VIEWMODE { TOODEE, TOPDEE };


	typedef struct tagStateDesc {
		PLAYERSTATE					eState;
		unsigned int				iMaxAnimCount;
	}PLAYERSTATE_DESC;
}

extern HWND g_hWnd;
using namespace Client;


#define GRAVITY 9.8f