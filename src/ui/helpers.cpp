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
		double x, double y, double w, double h, double radius) {
	cr->move_to(x, y + h - radius);
	cr->arc(x + radius, y + radius, radius, -M_PI, -M_PI / 2);
	cr->arc(x + w - radius, y + radius, radius, -M_PI / 2, 0);
	cr->arc(x + w - radius, y + h - radius, radius, 0, M_PI / 2);
	cr->arc(x + radius, y + h - radius, radius, M_PI / 2, M_PI);
}

}
