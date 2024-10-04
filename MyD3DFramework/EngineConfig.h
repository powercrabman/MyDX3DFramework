#pragma once

//-------------------------------
// Light
//-------------------------------
#define DIRECTIONAL_LIGHT_MAXIMUM_COUNT		2
#define POINT_LIGHT_MAXIMUM_COUNT			8
#define SPOT_LIGHT_MAXIMUM_COUNT			4
#define USABLE_LIGHT_MAXIMUM_COUNT			DIRECTIONAL_LIGHT_MAXIMUM_COUNT + POINT_LIGHT_MAXIMUM_COUNT + SPOT_LIGHT_MAXIMUM_COUNT
#define MSAA_OPTION							true

namespace EngineConfig
{
}