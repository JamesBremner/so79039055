#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h> /* time */

#include <wex.h>
#include "cStarterGUI.h"
#include "cxy.h"
#include "cRunWatch.h"

class cRect
{
    cxy myCenter;
    int myWidth;
    int myHeight;
    int myMaxDim;

public:
    cRect(const cxy &c, int w, int h)
        : myCenter(c),
          myWidth(w),
          myHeight(h)
    {
        myMaxDim = myWidth;
        if (myHeight > myWidth)
            myMaxDim = myHeight;
    }
    bool isCollision(
        const cRect &other) const;
};

struct sProblem
{
};

bool cRect::isCollision(const cRect &other) const
{
    // check for centers too far apart
    // for any possibility of a collision
    double d2 = myCenter.dist2(other.myCenter);
    double mind2 = myMaxDim + other.myMaxDim;
    mind2 *= mind2;
    if (d2 > mind2)
        return false;

    // collision possible, check in detail
    double dy = abs(myCenter.y - other.myCenter.y);
    double miny = abs(myHeight + other.myHeight) / 2;
    if (dy < miny)
        return true;
    double dx = abs(myCenter.x - other.myCenter.x);
    double minx = abs(myWidth + other.myWidth) / 2;
    if (dx < minx)
        return true;

    return false;
}

bool unitTests()
{
    cRect A(cxy(0, 0), 10, 20);
    cRect B(cxy(100, 100), 1, 2);
    if (A.isCollision(B))
        return false;
    cRect C(cxy(9, 100), 1, 2);
    if (C.isCollision(A))
        return false;
    cRect D(cxy(6, 11), 1, 2);
    if (D.isCollision(A))
        return false;
    cRect E(cxy(5, 11), 1, 2);
    if (E.isCollision(A))
        return false;
    cRect F(cxy(4, 11), 1, 2);
    if (!F.isCollision(A))
        return false;

    std::cout << "Unit tests passed\n";
    return true;
}
void performanceTest()
{
    raven::set::cRunWatch::Start();
    srand(time(NULL));
    cRect A(cxy(0, 0), 10, 20);
    for (int r = 0; r < 10; r++)
    {
        cxy t(rand() % 30 + 1, rand() % 30 + 1);
        cRect B(t, 1, 1);
        for (int i = 0; i < 1000; i++)
        {
            raven::set::cRunWatch aWatcher("collision detector");

            A.isCollision(B);
        }
    }
    raven::set::cRunWatch::Report();
}
class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        show();
        run();
    }

private:
    wex::label &lb;
};

main()
{
    if (!unitTests())
    {
        std::cout << "unit test failed\n";
        exit(1);
    }
    performanceTest();
    cGUI theGUI;
    return 0;
}
