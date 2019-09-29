#ifndef OPTIONS_H
#define OPTIONS_H

#include <vector>
#include <iostream>
#include <getopt.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

enum Color : int { COLOR_RED=0, COLOR_GREEN=1, COLOR_BLUE = 2};

class Options
{
public:
    static Options* getInstance();
    ~Options();
    void printHelp();
    void processArguments(int argc, char* argv[]);
    bool opt_verbose;
    vector<int> opt_devices;
    vector<uint8_t> opt_rgb;
    bool opt_get;
    vector<int> opt_axes;
    int opt_x;
    int opt_y;
    int opt_z;
    int opt_c;
    string opt_curve;
    bool opt_defaults;
    bool opt_calibrate;
    bool opt_setRGB;
    bool opt_list;
private:
    static Options* Instance;
    Options();
};



#endif
