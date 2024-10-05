#include "redgreen.h"

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
        else if (line[0] == 'r')
        {
            addRed(
                cxy(atof(tokens[1].c_str()),
                    atof(tokens[2].c_str())),
                atof(tokens[2].c_str()),
                atof(tokens[3].c_str()));
        }
        else if (line[0] == 'b')
        {
            for (int i = 1; i < tokens.size(); i += 2)
                addBoundary(
                    atof(tokens[i].c_str()),
                    atof(tokens[i + 1].c_str()));
        }
    }
}
void sProblem::writeFile(const std::string fname)
{
    std::ofstream ofs(fname);
    if (!ofs.is_open())
        throw std::runtime_error(
            "Cannot open file " + fname);

    cxy c;
    int w;
    int h;
    for (auto &g : myGreens)
    {
        g.getRaw(c,w,h);
        ofs << "g "<< c.x <<" "<< c.y <<" "<< w <<" "<< h <<"\n";
    }
}