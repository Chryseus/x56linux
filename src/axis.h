#ifndef AXIS_H
#define AXIS_H

#include "util.h"
#include <string>
using namespace std;

class Axis
{
public:
    Axis(unsigned char axisID, const string& name="Unamed axis", word defaultXSat=1000, word defaultYSat=1000, word defaultDeadzone=0, word defaultCurvature=500,
        const string& defaultCurveType="s", bool isHallEffect = false, uint8_t dataOffset=0)
    {
        this->axisID = axisID;
        this->axisName = name;
        this->defaultXSat = defaultXSat;
        this->defaultYSat = defaultYSat;
        this->defaultDeadzone = defaultDeadzone;
        this->defaultCurvature = defaultCurvature;
        this->defaultCurveType = defaultCurveType;
        this->isHallEffect = isHallEffect;
        this->dataOffset = dataOffset;
    }
    unsigned char axisID;
    string axisName;
    bool isHallEffect;
    word defaultXSat;
    word defaultYSat;
    word defaultDeadzone;
    word defaultCurvature;
    string defaultCurveType;
    uint8_t dataOffset;
};

#endif
