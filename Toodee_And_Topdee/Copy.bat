:: 명령어			옵션			원본 파일이 있는 위치			사본 파일을 저장할 위치

xcopy			/y			.\Engine\Public\*.*			.\EngineSDK\Inc\	

xcopy			/y			.\Engine\Bin\Engine.dll		.\Client\Bin\
xcopy			/y			.\Engine\Bin\Engine.pdb		.\Client\Bin\

xcopy			/y			.\Engine\Bin\Engine.lib		.\EngineSDK\Lib\

xcopy			/y			.\Engine\Public\Fmod\lib\fmod.dll		.\Client\Bin\