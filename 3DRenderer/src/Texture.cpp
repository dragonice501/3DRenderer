#include "Texture.h"

Tex2 Tex2Clone(const Tex2* t)
{
	Tex2 tex = { t->u, t->v };
	return tex;
}
