

#include <algorithm>
#include <time.h> /* time */

#include <wex.h>
#include "cStarterGUI.h"

#include "redgreen.h"

cRect::cRect(const cxy &c, int w, int h)
    : myCenter(c),
      myWidth(w),
      myHeight(h)
{
    myMaxDim = myWidth;
    if (myHeight > myWidth)
        myMaxDim = myHeight;
}

void cRect::get(cxy &c, int &w, int &h) const
{
    const int scale = 10;
    const int off = 50;
    c = cxy(
        off + scale * myCenter.x,
        off + scale * myCenter.y);
    w = off + scale * myWidth;
    h = off + scale * myHeight;
}

/// @brief collision detector with this rectangle
/// @param other rectangle
/// @param[out] overlap avoidance vector if collision occurred ( use to clear the collision )
/// @return true if collsion occurred

bool cRect::isCollision(
    const cRect &other,
    cxy &overlap) const
{
    // check for centers too far apart
    // for any possibility of a collision
    double d2 = myCenter.dist2(other.myCenter);
    double mind2 = myMaxDim + other.myMaxDim;
    mind2 *= mind2;
    if (d2 > mind2)
    {
        overlap = cxy(0, 0);
        return false;
    }

    // collision possible, check in detail
    bool ret = false;
    double dy = myCenter.y - other.myCenter.y;
    double miny = ( myHeight + other.myHeight) / 2;
    overlap.y = (dy - miny) / 2;
    if (abs(dy) < miny)
        ret = true;

    double dx = (myCenter.x - other.myCenter.x);
    double minx = abs(myWidth + other.myWidth) / 2;
    overlap.x = ( dx - minx ) / 2;
    if (abs(dx) < minx)
        ret = true;

    if (!ret)
    {
        // no collision occurred
        overlap = cxy(0, 0);
    }

    return ret;
}

void sProblem::clear()
{
    myGreens.clear();
    myReds.clear();
}

void sProblem::generate()
{
    clear();
    const int redCount = 1;
    srand(time(NULL));
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 6; row++)
            myGreens.emplace_back(
                cxy(20 * row, 20 * col),
                4, 1);
    for (int i = 0; i < redCount; i++)
        myReds.emplace_back(
            cxy(rand() % 50 + 1, rand() % 30 + 1),
            10, 10);
}

sProblem theProblem;

void sProblem::addGreen(cxy c, int w, int h)
{
    myGreens.emplace_back(c, w, h);
}
void sProblem::addRed(cxy c, int w, int h)
{
    myReds.emplace_back(c, w, h);
}

void sProblem::dodge()
{
    cxy overlap;
    for (auto &red : myReds)
    {
        for (auto &green : myGreens)
        {
            if (green.isCollision(red, overlap))
            {
                cxy c;
                int w;
                int h;
                green.get(c, w, h);
                std::cout << "moving " << c << " by " << overlap;

                green.move(overlap);

                green.get(c, w, h);
                std::cout << " to " << c << "\n";
            }
        }
    }
}

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
                cxy c;
                int w;
                int h;
                S.fill();
                S.color(0x00FF00);
                for (auto &r : theProblem.myGreens)
                {
                    r.get(c, w, h);
                    S.rectangle({c.x - w / 2,
                                 c.y - h / 2,
                                 w, h});
                }
                S.color(0x0000FF);
                for (auto &r : theProblem.myReds)
                {
                    r.get(c, w, h);
                    S.rectangle({c.x - w / 2,
                                 c.y - h / 2,
                                 w, h});
                }
            });
        show();
        run();
    }

private:
};

main()
{
    if (!unitTests())
    {
        std::cout << "unit test failed\n";
        exit(1);
    }
    performanceTest();
    theProblem.generate();
    theProblem.dodge();

    cGUI theGUI;
    return 0;
}
