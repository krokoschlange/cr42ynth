/*
 * helpers.cpp
 *
 *  Created on: 25.06.2020
 *      Author: fabian
 */

#include "helpers.h"

#include <cairomm/context.h>

namespace cr42y
{

void cr42y_rounded_rectangle(Cairo::RefPtr<Cairo::Context> cr,
		double x, double y, double w, double h, double radius, double lineWidth) {
	double halfLW = lineWidth / 2;
	cr->move_to(x + halfLW, y + h - radius - halfLW);
	cr->arc(x + radius + halfLW, y + radius + halfLW, radius, -M_PI, -M_PI / 2);
	cr->arc(x + w - radius - halfLW, y + radius + halfLW, radius, -M_PI / 2, 0);
	cr->arc(x + w - radius - halfLW, y + h - radius - halfLW, radius, 0, M_PI / 2);
	cr->arc(x + radius + halfLW, y + h - radius - halfLW, radius, M_PI / 2, M_PI);
}

}
