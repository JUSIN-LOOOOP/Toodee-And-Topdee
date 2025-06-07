#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CEvent abstract
{
protected:
	virtual ~CEvent() = default;
};

/* ENTER_PORTAL */
typedef struct tagEnterPortalEvent : public CEvent{
	class CGameObject* pPlayer;
}ENTERPORTALEVENT;

/* EXIT_PORTAL */
typedef struct tagExitPortalEvent : public CEvent {
	class CGameObject* pPlayer;
}EXITPORTALEVENT;

/* CAN_CLEAR */
typedef struct tagCanClearEvent : public CEvent {	
	_float3 vPosition;
}CANCLEAREVENT;

/* BLOCK_BREAK */
typedef struct tagBlockBreak : public CEvent {
	_float3 vPosition;
}BLOCKBREAKEVENT;

/* ADD_KEY */
typedef struct tagAddKey : public CEvent {
}ADDKEYEVENT;

/* GET_KEY */
typedef struct tagGetKey : public CEvent {
}GETKEYEVENT;

/* BOSS_STATE */
typedef struct tagMonsterSignal : public CEvent {
	_uint iState;
	_uint iViewMode;
}MONSTERSIGNAL;

/* CHANGE_CAMERA */
typedef struct tagChangeCam : public CEvent {
}CHANGECAM;

END

