/*
 * helpers.h
 *
 *  Created on: 25.06.2020
 *      Author: fabian
 */

#ifndef SRC_UI_HELPERS_H_
#define SRC_UI_HELPERS_H_

#include <cairomm/context.h>


namespace cr42y
{

void cr42y_rounded_rectangle(Cairo::RefPtr<Cairo::Context> cr,
		double x, double y, double w, double h, double radius);

}



#endif /* SRC_UI_HELPERS_H_ */
