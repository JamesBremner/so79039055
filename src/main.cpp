
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

bool cRect::isBoundaryCollision() const
{
    // check boundary exists
    if (!theProblem.myBoundary.size())
        return false;

    //std::cout << "isBoundaryCollision " << myCenter << "\n";

    auto closed = theProblem.myBoundary;
    closed.push_back( closed[0]);

    // check that the rectangle center is inside the boundary
    // outside counts as a "collision"
    if( ! myCenter.isInside( closed )) {
        //std::cout << "center outside\n";
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
                    re1, re2)) {
                    std::cout << "intersection\n";
                return true;
                    }
        }
        be1 = be2;
    }

    //std::cout << "<= clear\n";
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

     std::cout << myCenter << " collision with " << other.myCenter << "\n";

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

        // check no collision with boundary
        if (fclear)
            if (isBoundaryCollision())
                fclear = false;

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
        cxy(-3,30), 4, 3);
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

    clear();

    for (int col = 1; col < 7; col++)
        for (int row = 0; row < 4; row++)
            myGreens.emplace_back(
                cxy(8 * col, 10 + 12 * row),
                4, 3);

    srand(time(NULL));
    // srand( 100 );
    for (int i = 0; i < redCount; i++)
        myReds.emplace_back(
            cxy(rand() % 45 + 5, rand() % 45 + 5),
            redsize, redsize);

    myBoundary = {
        cxy(1, 1),
        cxy(52, 1),
        cxy(52, 52),
        cxy(1, 52)};
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

std::vector<std::string> tokenize(const std::string &line)
{
    std::vector<std::string> ret;
    std::stringstream sst(line);
    std::string a;
    while (getline(sst, a, ' '))
        ret.push_back(a);
    return ret;
}

void sProblem::readFile(const std::string fname)
{
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open file " + fname);

    std::string line;

    clear();

    while (getline(ifs, line))
    {
        std::cout << line << "\n";
        auto tokens = tokenize(line);
        if (tokens.size() != 5)
        {
            std::cout << line
                      << "\nbad input format\n";
            continue;
        }
        if (line[0] == 'g')
        {
            addGreen(
                cxy(atof(tokens[1].c_str()),
                    atof(tokens[2].c_str())),
                atof(tokens[2].c_str()),
                atof(tokens[3].c_str()));
        }
        if (line[0] == 'r')
        {
            addRed(
                cxy(atof(tokens[1].c_str()),
                    atof(tokens[2].c_str())),
                atof(tokens[2].c_str()),
                atof(tokens[3].c_str()));
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
    // performanceTest2();
    theProblem.generate();
    //theProblem.gen1();
    theProblem.dodge();

    cGUI theGUI;
    return 0;
}
