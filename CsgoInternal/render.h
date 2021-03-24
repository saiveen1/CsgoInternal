#pragma once
#include <windows.h>
#include <memory>

#include "math.h"
#include "hack.h"
#include "dx9.h"

#define WHITE D3DCOLOR_ARGB(255, 255, 255, 255)
#define BLACK D3DCOLOR_ARGB(255, 0, 0, 0)
class Render
{

public:
	ID3DXLine* LineL;
	ID3DXFont* FontF;

public:
	Render();
	~Render();

	VOID DrawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT thickness, D3DCOLOR color);

	VOID DrawLine(Vec2 src, Vec2 dst, FLOAT thickness, D3DCOLOR color);

	VOID DrawText(LPCWSTR text, Vec2 pos, D3DCOLOR color);

	VOID DrawFilledRect(Rect rect, D3DCOLOR color);

	VOID DrawRect(Rect rect, FLOAT thickness, D3DCOLOR color);

	VOID DrawRectWithFilledCol(Rect box, Rect fill, FLOAT lineThickness, D3DCOLOR lineColor, D3DCOLOR filledColor);

	VOID DrawCircle(Vec2 center, FLOAT radius, UINT sides, FLOAT width, D3DCOLOR color);
};

inline std::unique_ptr<Render> render;
