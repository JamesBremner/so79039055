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
public:
    enum class eStatus
    {
        fixed,  // there was no need to move the rectangle
        moved,  // the rect was moved to avoild red collisions
        failed, // the rect was trapped with no move that avoided red collisions
    };

    cRect(const cxy &c, int w, int h);
    cRect()
    {
    }

    /// @brief True if collision between this and other rectangle
    /// @param other
    /// @return
    bool isCollision(
        const cRect &other);

    /// @brief True if collision betwee this recangle and the boundary
    /// @return
    bool isBoundaryCollision() const;

    /// @brief True if no collsion with anything
    /// @return
    bool isClear();

    /// @brief Find minimal moves in each direction to clear red rectangle
    /// @param red
    /// @return Minimal moves sorted into increasing distance

    std::vector<cxy> findMoves(const cRect &red);

    bool dodge(
        const cRect &other);

    // setters

    void status(eStatus s)
    {
        myStatus = s;
    }

    // getters

    void getScaled(cxy &c, int &w, int &h) const;
    void getRaw(cxy &c, int &w, int &h) const;
    cxy getCenter() const
    {
        return myCenter;
    }
    eStatus getStatus() const
    {
        return myStatus;
    }

private:
    cxy myCenter;
    int myWidth;
    int myHeight;
    int myMaxDim;
    eStatus myStatus;
};

struct sProblem
{
    std::vector<cRect> myGreens;
    std::vector<cRect> myReds;
    std::vector<cxy> myBoundary;

    void clear();
    void gen1();
    void generate();
    void dodge();
    void addGreen(cxy c, int w, int h);
    void addRed(cxy c, int w, int h);
    void readFile(const std::string fname);
};

extern sProblem theProblem;

class cGUI : public cStarterGUI
{
public:
    cGUI();

private:
    void draw(wex::shapes &S);
    void menus();
};
