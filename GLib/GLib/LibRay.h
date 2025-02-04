#pragma once

#include "LibLine.h"
#include <variant>

template<typename T>
class LibRay: public LibLine<T> 
{
public:
	bool IsIntersectionLine(const LibLine<T>& lnOther, LibPoint<T>& intersPoint) const override;
	bool IsIntersectionRay(const LibRay<T>& lnOther, LibPoint<T>& interûPoint) const;
	LibPoint<T> ClosestPointOnLine(const LibPoint<T>& point) const override;
	bool IsPointOnLine(const LibPoint<T>& point) const override;
};
