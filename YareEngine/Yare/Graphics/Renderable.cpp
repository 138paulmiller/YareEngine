#include "Renderable.hpp"

namespace yare { 
	RenderColorMask operator|(RenderColorMask l, RenderColorMask r)
	{
		return RenderColorMask( ( char)l | char(r) );
	}
	RenderColorMask operator&(RenderColorMask l, RenderColorMask r)
	{
		return RenderColorMask((char)l & char(r));
	}
} 