#pragma once

#include "item.h"
#include "line.h"

class LinePort : public Line
{
public:
	LinePort(qreal x1, qreal y1, qreal x2, qreal y2, Line* parent = 0);
};