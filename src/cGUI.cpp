#include "redgreen.h"

    cGUI::cGUI()
        : cStarterGUI(
              "RedGreen",
              {50, 50, 800, 750})
    {
        menus();

        fm.events().draw(
            [this](PAINTSTRUCT &ps)
            {
                wex::shapes S(ps);
                draw(S);
            });
        show();
        run();
    }

void cGUI::menus()
{
    wex::menubar mb(fm);

    wex::menu mf(fm);
    mf.append("Open", [&](const std::string &title)
              {
        // prompt for file to open
        wex::filebox fb( fm );
        auto paths = fb.open();
        if( paths.empty() )
            return;
        try
        {
             // read the file
            theProblem.readFile( paths );

            // move greens to avoid collisions
            theProblem.dodge();

            // refresh display with contents of opened file
            fm.text("RedGreen " + paths);
            fm.update();
        }
        catch( std::runtime_error& e )
        {
            wex::msgbox mb(
                           std::string("Error reading file\n")+e.what());
            exit(1);
        } });

    mb.append("File", mf);
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
                {(int)(c.x - w / 2), (int)(c.y - h / 2)});
            break;
        case cRect::eStatus::failed:
            // no clear space = show '!F!' in top left
            S.color(0x0);
            S.text(
                "!F!",
                {(int)(c.x - w / 2), (int)(c.y - h / 2)});
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

    S.color(0xFF0000);
    cxy p = theProblem.myBoundary[0];
    cxy n;
    for (int k = 1; k < theProblem.myBoundary.size(); k++)
    {
        n = theProblem.myBoundary[k];
        S.line({50+10 * p.x, 50+10 * p.y,
                50+10 * n.x, 50+10 * n.y});
        p = n;
    }
    n = theProblem.myBoundary[0];
    S.line({(int)(50+10 * p.x), (int)(50+10 * p.y),
            (int)(50+10 * n.x), (int)(50+10 * n.y)});
}
