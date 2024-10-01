

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

void cRect::getScaled(cxy &c, int &w, int &h) const
{
    const int scale = 10;
    const int off = 50;
    c = cxy(
        off + scale * myCenter.x,
        off + scale * myCenter.y);
    w = scale * myWidth;
    h = scale * myHeight;
}
void cRect::getRaw(cxy &c, int &w, int &h) const
{

    c = myCenter;
    w = myWidth;
    h = myHeight;
}

/// @brief collision detector with this rectangle
/// @param other rectangle, must be red

/// @return true if collsion occurred

bool cRect::isCollision(
    const cRect &other )
{
    // check for centers too far apart
    // for any possibility of a collision

    double d2 = myCenter.dist2(other.myCenter);
    double mind2 = myMaxDim + other.myMaxDim;
    mind2 *= mind2;
    if (d2 > mind2)
    {
        return false;
    }

    // collision possible, check in detail

    double mindx = (myWidth + other.myWidth) / 2;
    double mindy = (myHeight + other.myHeight) / 2;
    double dx = abs(myCenter.x - other.myCenter.x);
    double dy = abs(myCenter.y - other.myCenter.y);
    if (dx > mindx || dy > mindy)
        return false;


    // distance from red to green centers
    double dxy = sqrt( dx*dx+dy*dy);

    // move the green rect well out of the way
    cxy vxy = other.myCenter.vect(myCenter);
    double mindxy = sqrt(myMaxDim * myMaxDim + other.myMaxDim * other.myMaxDim);
    cxy nvxy(
        vxy.x / dxy,
        vxy.y / dxy);
    cxy vfix(
        nvxy.x * mindxy,
        nvxy.y * mindxy);

    std::cout << "Moving " << myCenter 
        << " by " << vfix;


    myCenter.x += vfix.x;
    myCenter.y += vfix.y;

    std::cout << " to " << myCenter << "\n";
    return true;

}

void sProblem::clear()
{
    myGreens.clear();
    myReds.clear();
}

void sProblem::gen1()
{
    clear();
    myGreens.emplace_back(
        cxy(5, 5), 5, 5);
    myReds.emplace_back(
        cxy(4, 4), 5, 5);
}
void sProblem::generate()
{
    clear();
    const int redCount = 40;
    srand(time(NULL));
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 6; row++)
            myGreens.emplace_back(
                cxy(10 * row, 15 * col),
                4, 3);
    for (int i = 0; i < redCount; i++)
        myReds.emplace_back(
            cxy(rand() % 50 + 1, rand() % 50 + 1),
            1, 1);
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
    for (auto &red : myReds)
    {
        for (auto &green : myGreens)
        {
            if (green.isCollision(red))
            {
                // cxy c;
                // int w;
                // int h;
                // green.getRaw(c, w, h);
                // std::cout << "moving " << c << " by " << overlap;

                // green.move(overlap);

                // green.getRaw(c, w, h);
                // std::cout << " to " << c << "\n";
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
                    r.getScaled(c, w, h);
                    //std::cout << "grph green " << c << "\n";
                    S.rectangle({(int)(c.x - w / 2),
                                 (int)(c.y - h / 2),
                                 (int)w, (int)h});
                }
                S.color(0x0000FF);
                for (auto &r : theProblem.myReds)
                {
                    r.getScaled(c, w, h);
                    //std::cout << "grph red " << c << "\n";
                    S.rectangle({(int)(c.x - w / 2),
                                 (int)(c.y - h / 2),
                                 (int)w, (int)h});
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
