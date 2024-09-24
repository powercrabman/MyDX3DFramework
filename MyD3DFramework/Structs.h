#pragma once

struct WindowSize
{
	WindowSize() = default;
	WindowSize(int inW, int inH)
		: Width(inW)
		, Height(inH)
	{
	}

	WindowSize(const RECT& inRect)
		: Width(inRect.right - inRect.left)
		, Height(inRect.bottom - inRect.top)
	{
	}

	const static WindowSize s_defaultWindowSize;

	unsigned __int64 Width = 0;
	unsigned __int64 Height = 0;
};

struct WindowPosition
{
	WindowPosition() = default;
	WindowPosition(int inX, int inY)
		: X(inX)
		, Y(inY)
	{
	}

	const static WindowPosition s_defaultWindowPosition;

	unsigned __int64 X = 0;
	unsigned __int64 Y = 0;
};

struct LightColor
{
	Color Ambient;
	Color Diffuse;
	Color Specular;
};