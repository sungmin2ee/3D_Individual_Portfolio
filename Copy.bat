// 명령어				옵션			원본파일이 있는 위치			사본파일을 저장할 위치

xcopy				/y/s			.\Engine\Public\*.h				.\EngineSDK\Inc\
xcopy				/y/I			.\Engine\Bin\Engine.dll				.\Client\Bin\
xcopy				/y/I			.\Engine\Bin\Engine.lib				.\EngineSDK\Lib\
xcopy				/y			.\Engine\Bin\ShaderFiles\*.*			.\Client\Bin\ShaderFiles\



xcopy				/y/I			.\ThirdParty\fmod\lib\x64\fmod.dll				.\Client\Bin\
xcopy				/y/I			.\ThirdParty\assimp_6.0.4\lib\assimp-vc143-mt.dll 		.\Client\Bin\
