/*
 * SplitSTR.h
 *
 *  Created on: Apr 20, 2013
 *      Author: gaby
 */

#ifndef SPLITSTR_H_
#define SPLITSTR_H_

#include <string>
#include <vector>
#include <iostream>
using namespace std;


class splitstring : public string {
    vector<string> flds;
public:
    splitstring(char *s) : string(s) { };
    vector<string>& split(char delim, int rep=0);
};




#endif /* SPLITSTR_H_ */
