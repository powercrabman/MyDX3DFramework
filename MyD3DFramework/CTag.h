#pragma once
#include "CAttribute.h"

template <typename Ty>
class CTag : public CAttribute final
{
public:
	CTag() = default;
	virtual ~CTag() = default;
};