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

class Options
{
public:
    static Options* getInstance();
    ~Options();
    void printHelp();
    void processArguments(int argc, char* argv[]);
    bool opt_verbose;
    vector<int> opt_devices;
    vector<unsigned char> opt_rgb;
    bool opt_get;
    vector<int> opt_axes;
    int opt_x;
    int opt_y;
    int opt_z;
    int opt_c;
    string opt_curve;
    bool opt_defaults;
    bool opt_calibrate;
private:
    static Options* Instance;
    Options();
};



#endif
