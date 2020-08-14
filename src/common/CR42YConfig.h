/*
 * CR42YConfig.h
 *
 *  Created on: 02.07.2020
 *      Author: fabian
 */

#ifndef SRC_COMMON_CR42YCONFIG_H_
#define SRC_COMMON_CR42YCONFIG_H_

#include <string>
#include <unordered_map>

namespace cr42y
{

class CR42YConfig
{
public:
	CR42YConfig();
	virtual ~CR42YConfig();

	int read(std::string conf);

	std::string get(std::string prop);

private:
	std::unordered_map<std::string, std::string> map;

};

} /* namespace cr42y */

#endif /* SRC_COMMON_CR42YCONFIG_H_ */
