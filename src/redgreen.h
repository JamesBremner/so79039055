#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "cxy.h"
#include <wex.h>
#include "cStarterGUI.h"
#include "redgreen.h"

bool unitTests();
void performanceTest();
void performanceTest2();

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
        const cRect &other);

    bool dodge(
    const cRect &other);

    void getScaled(cxy &c, int &w, int &h) const;
    void getRaw(cxy &c, int &w, int &h) const;
    cxy getCenter() {
        return myCenter;
    }
};

struct sProblem
{
    std::vector<cRect> myGreens;
    std::vector<cRect> myReds;

    void clear();
    void gen1();
    void generate();
    void dodge();
    void addGreen(cxy c, int w, int h);
    void addRed(cxy c, int w, int h);
};

extern sProblem theProblem;

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500})
    {

        fm.events().draw(
            [this](PAINTSTRUCT &ps)
            {
                wex::shapes S(ps);
                draw(S);
            });
        show();
        run();
    }

private:
    void draw(wex::shapes &S);
};
