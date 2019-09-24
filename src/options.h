#ifndef OPTIONS_H
#define OPTIONS_H

#include <vector>
using namespace std;

class Options
{
public:
    static bool opt_verbose;
    static vector<int>* opt_devices;
    static unsigned char opt_rgb[2];
    static bool opt_get;
    static vector<int>* opt_axes;
    static int opt_x;
    static int opt_y;
    static int opt_z;
    static int opt_c;
    static char opt_curve;
    static bool opt_defaults;
    static bool opt_calibrate;
};

bool Options::opt_verbose = false;
bool Options::opt_get = false;
int Options::opt_x = 0;
int Options::opt_y = 0;
int Options::opt_z = 0;
int Options::opt_c = 0;
char Options::opt_curve = '\0';
bool Options::opt_defaults = false;
bool Options::opt_calibrate = false;
vector<int>* Options::opt_devices = new vector<int>;
vector<int>* Options::opt_axes = new vector<int>;

#endif
