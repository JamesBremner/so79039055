#include "redgreen.h"
#include "cRunWatch.h"

bool dodgeTest()
{
    bool ret = false;
    theProblem.clear();
    theProblem.addGreen(
        cxy(5,5), 5, 5 );
    theProblem.addRed(
        cxy(4,4),5,5);

    theProblem.dodge();

                cxy c;
                int w;
                int h;
    theProblem.myGreens[0].getRaw(c,w,h);
    // if( c.x == 140 && c.y == 140 )
         ret = true;

    theProblem.clear();
    return ret;
}
bool unitTests()
{
        if( ! dodgeTest() )
        return false;

    // cxy overlap;
    // cRect A(cxy(0, 0), 10, 20);
    // cRect B(cxy(100, 100), 1, 2);
    // if (A.isCollision(B, overlap))
    //     return false;
    // cRect C(cxy(9, 100), 1, 2);
    // if (C.isCollision(A, overlap))
    //     return false;
    // cRect D(cxy(6, 11), 1, 2);
    // if (D.isCollision(A, overlap))
    //     return false;
    // cRect E(cxy(5, 11), 1, 2);
    // if (E.isCollision(A, overlap))
    //     return false;
    // cRect F(cxy(4, 11), 1, 2);
    // if (!F.isCollision(A, overlap))
    //     return false;



    std::cout << "Unit tests passed\n";
    return true;
}
void performanceTest()
{
    raven::set::cRunWatch::Start();
    srand(time(NULL));
    cxy overlap;
    cRect A(cxy(0, 0), 10, 20);
    for (int r = 0; r < 10; r++)
    {
        cxy t(rand() % 30 + 1, rand() % 30 + 1);
        cRect B(t, 1, 1);
        for (int i = 0; i < 1000; i++)
        {
            raven::set::cRunWatch aWatcher("collision detector");

            A.isCollision(B, overlap);
        }
    }
    raven::set::cRunWatch::Report();
}