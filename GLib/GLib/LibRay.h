#pragma once

#include "LibLine.h"
#include <variant>

template<typename T>
class LibRay: public LibLine<T> 
{
public:
	std::variant<LibPoint<T>, bool> IsIntersectionLine(const LibLine<T>& lnOther) const override;
	std::variant<LibPoint<T>, bool> IsIntersectionRay(const LibRay<T>& lnOther) const override;
	LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const override;
	bool IsPointOnLine(const LibPoint<T>& point) const override;
};
