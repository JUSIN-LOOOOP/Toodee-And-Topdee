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

typedef struct tagSinkBlock : public CEvent {
}SINKBLOCKEVENT;

typedef struct tagSinkeOpen : public CEvent {
}SINKOPENEVENT;

/* RED_BUTTON AND BLOCK_DISAPPEAR Pressed*/
typedef struct tagRedButtonPressed : public CEvent {
}EVENT_REDBUTTON_PRESSED;

/* RED_BUTTON AND BLOCK_DISAPPEAR Unpressed*/
typedef struct tagRedButtonUnpressed : public CEvent {
}EVENT_REDBUTTON_UNPRESSED;

typedef struct tagLevelChange : public CEvent {
	_uint iCurrentLevel;
	_uint iChangeLevel;
}LEVELCHANGE_EVENT;


/* Main Menu Swap */
typedef struct tagMainMenuSwap : public CEvent {
	_float fPositionY;
}MAINMENU_SWAP_EVENT;

/* Main Menu Swap */
typedef struct tagMainMenuStageSelectNumber : public CEvent {
	_uint iNum;
}STAGE_SELECT_EVENT;

/* CamShaking */
typedef struct tagCamShaking : public CEvent {
	_uint fTime;
}SHAKING;

/* LoadingScreen StartPosition*/
typedef struct tagLoadingScreen : public CEvent {
	_float3 pos = { 0.f,0.f,0.f };
}LOADINGSCREEN_EVENT;
END

