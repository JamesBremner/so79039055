
#include <algorithm>
#include <time.h> /* time */

#include "redgreen.h"

cRect::cRect(const cxy &c, int w, int h)
    : myCenter(c),
      myWidth(w),
      myHeight(h),
      myStatus(eStatus::fixed)
{
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

bool cRect::isBoundaryCollision() const
{
    // check boundary exists
    if (!theProblem.myBoundary.size())
        return false;

    // std::cout << "isBoundaryCollision " << myCenter << "\n";

    auto closed = theProblem.myBoundary;
    closed.push_back(closed[0]);

    // check that the rectangle center is inside the boundary
    // outside counts as a "collision"
    if (!myCenter.isInside(closed))
    {
        // std::cout << "center outside\n";
        return true;
    }

    // loop over boundary edges
    cxy be1 = theProblem.myBoundary[0];
    cxy be2;
    for (int k = 1; k < closed.size(); k++)
    {
        be2 = closed[k];
        cxy re1, re2;
        // loop over rectangle edges
        for (int ei = 0; ei < 4; ei++)
        {
            cxy p;
            switch (ei)
            {
            case 0:
                re1.x = myCenter.x - myWidth / 2;
                re1.y = myCenter.y - myHeight / 2;
                re2.x = myCenter.x + myWidth / 2;
                re2.y = myCenter.y - myHeight / 2;
                break;
            case 1:
                re1 = re2;
                re2.x = myCenter.x + myWidth / 2;
                re2.y = myCenter.y + myHeight / 2;
                break;
            case 2:
                re1 = re2;
                re1.x = myCenter.x - myWidth / 2;
                re1.y = myCenter.y + myHeight / 2;
                break;
            case 3:
                re1 = re2;
                re1.x = myCenter.x - myWidth / 2;
                re1.y = myCenter.y - myHeight / 2;
                break;
            }
            if (cxy::isIntersection(
                    p,
                    be1, be2,
                    re1, re2))
            {
                std::cout << "intersection\n";
                return true;
            }
        }
        be1 = be2;
    }

    // std::cout << "<= clear\n";
    return false;
}
bool cRect::isCollision(
    const cRect &other)
{
    // https://gamedev.stackexchange.com/a/29796/23381

    float w = 0.5 * (myWidth + other.myWidth);
    float h = 0.5 * (myHeight + other.myHeight);
    float dx = myCenter.x - other.myCenter.x;
    float dy = myCenter.y - other.myCenter.y;

    return (abs(dx) <= w && abs(dy) <= h);
}

bool cRect::isClear()
{
    // check that there are no collisions with reds
    for (auto &red : theProblem.myReds)
    {
        if (isCollision(red))
        {
            // std::cout << " collided with " << red.getCenter() << "\n";
            return false;
        }
    }

    // check no collision with boundary
    if (isBoundaryCollision())
        return false;

    return true;
}

std::vector<cxy> cRect::findMoves(const cRect &red)
{
    std::vector<cxy> vv;

    // distance between rectangle centers
    double dx = red.myCenter.x - myCenter.x;
    double dy = red.myCenter.y - myCenter.y;

    // safe movement if 50% overlap
    // ensure margin of at least 1 unit
    double sx = 1 + (myWidth + red.myWidth) / 2;
    double sy = 1 + (myHeight + red.myHeight) / 2;

    // minimal moves in each direction, allowing for less than 50% overlap
    vv.push_back(cxy(sx - abs(dx), 0));
    vv.push_back(cxy(abs(dx) - sx, 0));
    vv.push_back(cxy(0, sy - abs(dy)));
    vv.push_back(cxy(0, abs(dy) - sy));

    // sort into increasing order of distance moved
    std::sort(
        vv.begin(), vv.end(),
        [](const cxy &va, const cxy &vb) -> bool
        {
            return (abs(va.x) + abs(va.y) < (abs(vb.x) + abs(vb.y)));
        });

    return vv;
}

/// @brief collision detector with this rectangle
/// @param other rectangle, must be red
/// @return true if collision occurred

/// If collision, this rectangle ( green ) will be moved to a new location
/// where it does not collide with any red rectangles

bool cRect::dodge(
    const cRect &other)
{
    if (!isCollision(other))
        return false;

    // std::cout << myCenter << " collision with " << other.myCenter << "\n";

    // remember starting location
    cxy firstPosition = myCenter;

    // loop over possible moves until clear spot found
    for (auto v : findMoves(other))
    {
        // move the rectangle
        myCenter.x = firstPosition.x + v.x;
        myCenter.y = firstPosition.y + v.y;

        // std::cout << "try move " << v << " to " << myCenter;

        // check that there are no collisions at the new location
        if (isClear())
        {
            // std::cout << "Moving " << firstPosition
            //           << " by " << v
            //           << " to " << myCenter << "\n";
            myStatus = eStatus::moved;
            break;
        }
    }

    if (myStatus != eStatus::moved)
    {
        myStatus = eStatus::failed;
        myCenter = firstPosition;
        std::cout << myCenter << "failed to move\n";
    }

    return true;
}

void sProblem::clear()
{
    myGreens.clear();
    myReds.clear();
    myBoundary.clear();
}

void sProblem::gen1()
{
    clear();
    myGreens.emplace_back(
        cxy(-3, 30), 4, 3);
    // myReds.emplace_back(
    //     cxy(4, 4), 5, 5);
    myBoundary = {
        cxy(1, 1),
        cxy(52, 1),
        cxy(52, 52),
        cxy(1, 52)};
}
void sProblem::generate()
{
    const int redCount = 40;
    const int redsize = 3;
    const int maxX = 60;
    const int maxY = 60;
    int rowSpace = maxY / 4;

    clear();

    for (int col = 1; col < 7; col++)
        for (int row = 0; row < 4; row++)
            myGreens.emplace_back(
                cxy(8 * col, 10 + rowSpace * row),
                4, 3);

    srand(time(NULL));
    // srand(100);
    for (int i = 0; i < redCount; i++)
        myReds.emplace_back(
            cxy(rand() % (maxX - 6) + 5, rand() % (maxY - 6) + 5),
            redsize, redsize);

    myBoundary = {
        cxy(1, 1),
        cxy(maxX, 1),
        cxy(maxX, maxY),
        cxy(1, maxY)};
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
void sProblem::addBoundary( double x, double y )
{
    myBoundary.emplace_back( x, y );
}

void sProblem::dodge()
{
    for (auto &red : myReds)
    {
        for (auto &green : myGreens)
        {
            green.dodge(red);
        }
    }
}

main()
{
    if (!unitTests())
    {
        std::cout << "unit test failed\n";
        exit(1);
    }
    performanceTest2();
    // theProblem.generate();
    //  theProblem.gen1();
    // theProblem.dodge();

    cGUI theGUI;
    return 0;
}
