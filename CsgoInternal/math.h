#pragma once

#include <windows.h>
#include <algorithm>
#include <vector>
#include <stdio.h>

#include "dx9.h"
#define PI 3.1415926
#define ANGLE(rad) (FLOAT)(180 / PI * rad)
#define W2S(w,s) Math::WorldToScreen(w,s)

struct Vec2 {
	FLOAT x, y;

	//屏幕坐标用..
	Vec2 operator+(FLOAT d) {
		return{ x, y + d };
	}

	Vec2 operator-(Vec2 d) {
		return { abs(x - d.x), abs(y - d.y) };
	}

	auto distFromCross() {
		auto xpow = powf(x - windowWidth / 2.f, 2);
		auto ypow = powf(y - windowHeight / 2.f, 2);
		return sqrtf(xpow + ypow);
	}

	VOID operator++() {
		x++;
		y++;
	}
};

struct Vec3 {
	float x, y, z;
	Vec3 operator+(Vec3 d) {
		return { x + d.x, y + d.y, z + d.z };
	}
	Vec3 operator-(Vec3 d) {
		return { x - d.x, y - d.y, z - d.z };
	}
	Vec3 operator*(FLOAT d) {
		return { x * d, y * d, z * d };
	}
	Vec3 operator/(FLOAT d) {
		return { x / d, y / d, z / d };
	}
	VOID operator-=(Vec3 d) {
		x -= d.x;
		y -= d.y;
		z -= d.z;
	}

	//角度用
	Vec3 Normalize() {
		if (y < -180) { y += 360; }
		if (y > 180) { y -= 360; }
		if (x > 89) { x = 89; }
		if (x < -89) { x = -89; }
		if (z) { z = 0; }
		return { x,y,z };
	}
};


struct Vec4 {
	FLOAT x, y, z, w;
};

struct Matrix4p4 {
	FLOAT arr[16];
};

struct Rect {
	FLOAT topLeftX, topLeftY, width, height;

	//这里只是填充长方形用到, 旨在缩小d个像素不覆盖原框
	VOID operator+= (float d) {
		topLeftX += d;
		topLeftY += d; //Y轴向下
		height -= d;
		width -= d;
	}

	//D3D的矩形参数是左上和右下坐标
	D3DRECT ToD3DRect() {
		return { (LONG)topLeftX, (LONG)topLeftY, (LONG)(width + topLeftX), (LONG)(topLeftY + height) };
	}
};

namespace Math
{
	BOOL WorldToScreen(Vec3 pos, Vec2* screen);
	FLOAT GetDistance2D(Vec2 src, Vec2 dst);
}

