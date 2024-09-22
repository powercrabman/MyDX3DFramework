#pragma once

namespace DirectX
{
	namespace SimpleMath
	{
		inline constexpr Vector4 ToVector4(const Vector3& inVector, bool isPoint = true) 
		{
			return Vector4(inVector.x, inVector.y, inVector.z, isPoint ? 1 : 0);
		}
	}
}