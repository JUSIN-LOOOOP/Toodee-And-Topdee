#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class STATE { RIGHT, UP, LOOK, POSITION };
	enum class PROTOTYPE { GAMEOBJECT, COMPONENT };
	enum class RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };
	enum class WINMODE { MODE_FULL, MODE_WIN, MODE_END };

	enum class MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum class MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class TEXTURE { RECT, CUBE };
	enum class COLLIDER_SHAPE { RECT, CUBE };
	enum class COLLIDER_DIR {LEFT, RIGHT, FRONT, BACK, TOP, BOTTOM, CD_END};

	enum class DIMENSION { TOODEE, TOPDEE };

	enum class EVENT { ENTER_PORTAL, EXIT_PORTAL };
	enum class REPORT { REPORT_CANCLEAR };

	enum class CHANNELID { SOUND_MENU, SOUND_BOSS, SOUND_PLAYER , SOUND_RESOURCE, SOUND_EFFECT, SOUND_BGM, SOUND_END };
}
#endif // Engine_Enum_h__


