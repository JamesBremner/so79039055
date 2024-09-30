#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "cxy.h"
#include "redgreen.h"

bool unitTests();
void performanceTest();

class cRect
{
    cxy myCenter;
    int myWidth;
    int myHeight;
    int myMaxDim;

public:
    cRect(const cxy &c, int w, int h);
    cRect()
    {
    }
    bool isCollision(
        const cRect &other,
        cxy &overlap) const;
    void move(const cxy &v)
    {
        myCenter.x += v.x;
        myCenter.y += v.y;
    }
    void get(cxy &c, int &w, int &h) const;
};

struct sProblem
{
    std::vector<cRect> myGreens;
    std::vector<cRect> myReds;

    void clear();
    void generate();
    void dodge();
    void addGreen(cxy c, int w, int h);
    void addRed(cxy c, int w, int h);
};

extern sProblem theProblem;