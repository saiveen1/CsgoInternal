#include "render.h"


Render::Render()
{
	D3DXCreateLine(pDevice, &LineL);
	D3DXCreateFont(pDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &FontF);
}

Render::~Render()
{
	FontF->Release();
	LineL->Release();
}


VOID Render::DrawLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT thickness, D3DCOLOR color) {

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Begin();
	LineL->Draw(Line, 2, color);
	LineL->End();
}

VOID Render::DrawLine(Vec2 src, Vec2 dst, FLOAT thickness, D3DCOLOR color)
{
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

VOID Render::DrawText(LPCWSTR text, Vec2 pos, D3DCOLOR color)
{
	RECT rect;

	rect = { (LONG)pos.x, (LONG)pos.y, (LONG)pos.x, (LONG)pos.y };
	FontF->DrawText(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

//清除矩形区域内的渲染...简单来说就是画一个不透明的矩形
VOID Render::DrawFilledRect(Rect rect, D3DCOLOR color)
{
	D3DRECT r = rect.ToD3DRect();
	pDevice->Clear(1, &r, D3DCLEAR_TARGET, color, 0, 0);
}

VOID Render::DrawRect(Rect rect, FLOAT thickness, D3DCOLOR color)
{
	Vec2 topLeft, topRight;
	Vec2 bottomLeft, bottomRight;
	topLeft.x = bottomLeft.x = rect.topLeftX;
	topLeft.y = topRight.y = rect.topLeftY;
	topRight.x = bottomRight.x = rect.topLeftX + rect.width;
	//屏幕Y轴向下
	bottomLeft.y = bottomRight.y = rect.topLeftY + rect.height;

	DrawLine(topLeft, topRight, thickness, color);
	DrawLine(topRight, bottomRight, thickness, color);
	DrawLine(bottomRight, bottomLeft, thickness, color);
	DrawLine(bottomLeft, topLeft, thickness, color);

}

VOID Render::DrawRectWithFilledCol(Rect box, Rect fill, FLOAT lineThickness, D3DCOLOR lineColor, D3DCOLOR filledColor)
{
	DrawRect(box, lineThickness, lineColor);
	DrawFilledRect(fill, filledColor);
}

//半径的三分之一正好
VOID Render::DrawCircle(Vec2 center, FLOAT radius, UINT sides, FLOAT width, D3DCOLOR color)
{
	auto angle = D3DX_PI * 2 / sides;
	auto _cos = cosf(angle);
	auto _sin = sinf(angle);
	auto x1 = radius, y1 = 0.f, x2 = 0.f, y2 = 0.f;

	for (auto i = 0ul; i < sides; i++)
	{
		x2 = _cos * x1 - _sin * y1 + center.x;
		y2 = _sin * x1 + _cos * y1 + center.y;
		x1 += center.x;
		y1 += center.y;
		DrawLine(x1, y1, x2, y2, width, color);
		x1 = x2 - center.x;
		y1 = y2 - center.y;
	}
}

