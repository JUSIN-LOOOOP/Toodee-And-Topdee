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
}
#endif // Engine_Enum_h__
