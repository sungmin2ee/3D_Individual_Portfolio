// 명령어				옵션			원본파일이 있는 위치			사본파일을 저장할 위치

xcopy				/y/I			.\Engine\Header\*.h			.\Reference\Header\
xcopy				/y/I			.\Engine\Bin\Engine.dll		.\SRClient\Bin\
xcopy				/y/I			.\Engine\Bin\Engine.lib		.\Reference\Library\
xcopy				/y/I			.\Engine\ThirdParty\imgui\imgui-1.83\imgui.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\imgui-1.83\imconfig.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\imgui-1.83\imgui_internal.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\imgui-1.83\imstb_rectpack.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\imgui-1.83\imstb_textedit.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\imgui-1.83\imstb_truetype.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\ImGuizmo-1.83\ImGuizmo.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\ImGuizmo-1.83\ImSequencer.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\ImGuizmo-1.83\ImZoomSlider.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\ImGuizmo-1.83\ImCurveEdit.h		.\Reference\Header\
xcopy				/y/I			.\Engine\ThirdParty\imgui\ImGuizmo-1.83\GraphEditor.h		.\Reference\Header\
xcopy				/y/I/E			.\Engine\ThirdParty\nlohmann_3.12.0\include					.\Reference\Header\
xcopy				/y/I/E			.\Engine\ThirdParty\tweeny-3.2.1\include				.\Reference\Header\

xcopy				/y/I			.\Engine\ThirdParty\fmod\lib\x64\fmod.dll	.\SRClient\Bin\
xcopy				/y/I			.\Engine\ThirdParty\assimp_6.0.4\lib\assimp-vc143-mt.dll 		.\SRClient\Bin\
