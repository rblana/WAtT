/*
 * GPIOclass.h
 *
 *  Created on: Apr 3, 2013
 *      Author: gaby
 */

#ifndef GPIOCLASS_H_
#define GPIOCLASS_H_

#include <string>

using namespace std;

class GPIOClass {
public:
	GPIOClass();
	virtual ~GPIOClass();

	GPIOClass(string x); // create a GPIO object that controls GPIOx, where x is passed to this constructor
	int export_gpio(); // exports GPIO
	int unexport_gpio(); // unexport GPIO
	int setdir_gpio(string dir); // Set GPIO Direction
	int setval_gpio(string val); // Set GPIO Value (putput pins)
	int getval_gpio(string& val); // Get GPIO Value (input/ output pins)
	string get_gpionum(); // return the GPIO number associated with the instance of an object
private:
	string gpionum; // GPIO number associated with the instance of an object

};


#endif /* GPIOCLASS_H_ */
