#pragma once

#include "Client_Defines.h"
//#include "Engine_Defines.h"

BEGIN(Client)
inline const pair<_float3, _float3> Stage1_ColliderInfo(_uint iNum);
inline const pair<_float3, _float3> Stage2_ColliderInfo(_uint iNum);
inline const pair<_float3, _float3> Stage3_ColliderInfo(_uint iNum);



inline const _uint Stage_ColliderCount(_uint iLevel) {
	static const _uint arrayType[] =
	{
		0,0,0, 14,0,0

	};
	return arrayType[iLevel];
}

inline const  pair<_float3, _float3> MapCollider_Builder(_uint iLevel, _uint iNum)
{
	if (iLevel == 2) return Stage1_ColliderInfo(iNum);
	if (iLevel == 3) return Stage1_ColliderInfo(iNum);

	return Stage1_ColliderInfo(iNum);
}


/* 가로 : 좌하단부터 순서대로 우상단까지 타일 단위로 보면서 위치 구하기  */
/* 세로 : 좌하단부터 순서대로 우상단까지 타일 단위로 보면서 위치 구하기 */
inline const pair<_float3, _float3> Stage1_ColliderInfo(_uint iNum) 
{
	static const _float3 Position[] = {
		/* 가로 벽*/
		{0.f,	1.f,	-17.f },
		{-18.f,	1.f,	-15.f },
		{28.f,	1.f,	-15.f },
		{-22.f,	1.f,	5.f   },
		{-14.f,	1.f,	5.f   },
		{0.f,	1.f,	17.f  },

		/* 세로 벽*/
		{-31.f,	1.f,	0.f		},
		{-23.f,	1.f,	-9.f	},
		{-13.f,	1.f,	-9.f	},
		{31.f,	1.f,	0.f		},
		{-23.f,	1.f,	2.f		},
		{-13.f,	1.f,	2.f		},
		{-23.f,	1.f,	11.f	},
		{-13.f,	1.f,	11.f	},

	};
	static const _float3 Scale[] = {
		{	60.f,	2.f,	2.f	},
		{	12.f,	2.f,	2.f },
		{	8.f,	2.f,	2.f },
		{	4.f,	2.f,	2.f },
		{	4.f,	2.f,	2.f },
		{	60.f,	2.f,	2.f	},

		{	2.f,	2.f,	34.f	},
		{	2.f,	2.f,	10.f	},
		{	2.f,	2.f,	10.f	},
		{	2.f,	2.f,	34.f	},
		{	2.f,	2.f,	4.f 	},
		{	2.f,	2.f,	4.f		},
		{	2.f,	2.f,	10.f	},
		{	2.f,	2.f,	10.f	}
	};

	return { Position[iNum],Scale[iNum] };
}

inline const pair<_float3, _float3> Stage2_ColliderInfo(_uint iNum)
{
	static const _float3 Position[] = {
		/* 가로 벽*/
		{0.f,	0.f,	-16.f},
		{-16.f,	0.f,	-14.f},
		{26.f,	0.f,	-14.f},
		{-20.f,	0.f,	4.f},
		{-12.f,	0.f,	4.f},
		{0.f,	0.f,	16.f},

		/* 세로 벽*/
		{-29.f,	0.f,	0.f},
		{-21.f,	0.f,	-8.f},
		{-11.f,	0.f,	-8.f},
		{29.f,	0.f,	0.f},
		{-21.f,	0.f,	1.f},
		{-11.f,	0.f,	1.f},
		{-21.f,	0.f,	10.f},
		{-11.f,	0.f,	10.f},

	};
	static const _float3 Scale[] = {
		{	60.f,	2.f,	2.f	},
		{	12.f,	2.f,	2.f },
		{	8.f,	2.f,	2.f },
		{	4.f,	2.f,	2.f },
		{	4.f,	2.f,	2.f },
		{	60.f,	2.f,	2.f	},

		{	2.f,	30.f,	2.f},
		{	2.f,	10.f,	2.f},
		{	2.f,	10.f,	2.f},
		{	2.f,	30.f,	2.f},
		{	2.f,	4.f,	2.f},
		{	2.f,	4.f,	2.f},
		{	2.f,	10.f,	2.f},
		{	2.f,	10.f,	2.f}
	};

	return { Position[iNum],Scale[iNum] };
}

inline const pair<_float3, _float3> Stage3_ColliderInfo(_uint iNum)
{
	static const _float3 Position[] = {
		/* 가로 벽*/
		{0.f,	0.f,	-16.f},
		{-16.f,	0.f,	-14.f},
		{26.f,	0.f,	-14.f},
		{-20.f,	0.f,	4.f},
		{-12.f,	0.f,	4.f},
		{0.f,	0.f,	16.f},

		/* 세로 벽*/
		{-29.f,	0.f,	0.f},
		{-21.f,	0.f,	-8.f},
		{-11.f,	0.f,	-8.f},
		{29.f,	0.f,	0.f},
		{-21.f,	0.f,	1.f},
		{-11.f,	0.f,	1.f},
		{-21.f,	0.f,	10.f},
		{-11.f,	0.f,	10.f},

	};
	static const _float3 Scale[] = {
		{	60.f,	2.f,	2.f	},
		{	12.f,	2.f,	2.f },
		{	8.f,	2.f,	2.f },
		{	4.f,	2.f,	2.f },
		{	4.f,	2.f,	2.f },
		{	60.f,	2.f,	2.f	},

		{	2.f,	30.f,	2.f},
		{	2.f,	10.f,	2.f},
		{	2.f,	10.f,	2.f},
		{	2.f,	30.f,	2.f},
		{	2.f,	4.f,	2.f},
		{	2.f,	4.f,	2.f},
		{	2.f,	10.f,	2.f},
		{	2.f,	10.f,	2.f}
	};

	return { Position[iNum],Scale[iNum] };
}



END