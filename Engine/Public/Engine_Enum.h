#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{


	enum class WINMODE { FULL, WIN };
	enum class RENDERGROUP { PRIORITY, NONBLEND, BLEND, BLOCKER, UI,END };
	enum class STATE { RIGHT, UP, LOOK, POSITION, END };
	enum class D3DTS { VIEW, PROJ, END };
	enum class DIMK { LBUTTON, RBUTTON, WHEEL, END };
	enum class DIMM { X, Y, WHEEL, END };
	enum class MODEL { NONANIM, ANIM, END };
}
#endif // Engine_Enum_h__
