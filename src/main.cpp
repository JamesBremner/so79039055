
#include <algorithm>
#include <time.h> /* time */

#include "redgreen.h"

cRect::cRect(const cxy &c, int w, int h)
    : myCenter(c),
      myWidth(w),
      myHeight(h),
      myStatus(eStatus::fixed)
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

    //std::cout << myCenter << " collision with " << other.myCenter << "\n";

    // remember starting location
    cxy firstPosition = myCenter;

    // loop over possible moves until clear spot found
    cxy v;
    bool fclear = false;
    int loopCount = 0;
    while (!fclear)
    {
        // restore starting location
        myCenter = firstPosition;

        switch (loopCount)
        {
        case 0:
            v.x = (myWidth + other.myWidth) / 2;
            v.y = 0;
            break;
        case 1:
            v.x = 0;
            v.y = (myHeight + other.myHeight) / 2;
            break;
        case 2:
            v.x = -(myWidth + other.myWidth) / 2;
            v.y = 0;
            break;
        case 3:
            v.x = 0;
            v.y = -(myHeight + other.myHeight) / 2;
            break;
        case 4:
            v.x = myWidth + other.myWidth;
            v.y = -(myHeight + other.myHeight) / 2;
            break;
        default:
            // exhausted all possible moves
            // the green square is trapped with red all around
            // set green rectangle statu to failed
            // leave the green where it started.
            std::cout << myCenter << "failed to move\n";
            myCenter = firstPosition;
            myStatus = eStatus::failed;
            return true;
        }

        // move the rectangle
        myCenter.x = firstPosition.x + v.x;
        myCenter.y = firstPosition.y + v.y;

        //std::cout << "try move " << v << " to " << myCenter;

        loopCount++;

        // check that there are no collisions with reds
        // at the new location
        fclear = true;
        for (auto &red : theProblem.myReds)
        {
            if (isCollision(red))
            {
               // std::cout << " collided with " << red.getCenter() << "\n";
                fclear = false;
                break;
            }
        }
    } // end loop over possible moves

    myStatus = eStatus::moved;

    // std::cout << "Moving " << firstPosition
    //           << " by " << v
    //           << " to " << myCenter << "\n";

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
    const int redsize = 3;
    srand(time(NULL));
    // srand( 100 );
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 6; row++)
            myGreens.emplace_back(
                cxy(10 * row, 15 * col),
                4, 3);
    for (int i = 0; i < redCount; i++)
        myReds.emplace_back(
            cxy(rand() % 50 + 1, rand() % 50 + 1),
            redsize, redsize);
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
            green.dodge(red);
        }
    }
}
void cGUI::draw(wex::shapes &S)
{
    cxy c;
    int w;
    int h;
    S.fill();
    S.textHeight(10);
    for (auto &r : theProblem.myGreens)
    {
        S.color(0x00FF00);
        r.getScaled(c, w, h);
        // std::cout << "grph green " << c << "\n";
        S.rectangle({(int)(c.x - w / 2),
                     (int)(c.y - h / 2),
                     (int)w, (int)h});

        switch (r.getStatus())
        {
        case cRect::eStatus::fixed:
            // original location - no status display
            break;
        case cRect::eStatus::moved:
            // moved location - show 'm' in top left
            S.color(0x0);
            S.text(
                "m",
                {c.x - w / 2, c.y - h / 2});
            break;
        case cRect::eStatus::failed:
            // no clear space = show '!F!' in top left
            S.color(0x0);
            S.text(
                "!F!",
                {c.x - w / 2, c.y - h / 2});
            break;
         }
        // cxy rc = r.getCenter();
        // S.color(0x0);
        // S.text(
        //     std::to_string( (int)rc.x ) + "," + std::to_string((int)rc.y),
        //     {c.x - w/2,c.y-h/2}        );
    }

    S.color(0x0000FF);
    for (auto &r : theProblem.myReds)
    {
        S.color(0x0000FF);
        r.getScaled(c, w, h);
        // std::cout << "grph red " << c << "\n";
        S.rectangle({(int)(c.x - w / 2),
                     (int)(c.y - h / 2),
                     (int)w, (int)h});
        // cxy rc = r.getCenter();
        // S.color(0x0);
        // S.text(
        //     std::to_string( (int)rc.x ) + "," + std::to_string((int)rc.y),
        //     {c.x - w/2,c.y-h/2}        );
    }
}

main()
{
    if (!unitTests())
    {
        std::cout << "unit test failed\n";
        exit(1);
    }
    //performanceTest2();
     theProblem.generate();
     theProblem.dodge();

    cGUI theGUI;
    return 0;
}
