#include "input.h"
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include "metadata.h"
#include "state.h"

using namespace std;

State* Read(istream& in)
{
    string line;

    vector<string> rows;
    while (getline(in, line) && !line.empty()) {
        rows.push_back(line);
    }

    Metadata md;
    while (getline(in, line) && !line.empty()) {
        istringstream sin(line);
        string word;
        sin >> word;

        if (word == "Water") { sin >> md.water; }
        if (word == "Flooding") { sin >> md.flood; }
        if (word == "Waterproof") { sin >> md.waterproof; }
        if (word == "Growth") { sin >> md.growth; }
        if (word == "Razors") { sin >> md.razors; }

        if (word == "Trampoline") {
            char src, dst;
            string targets;
            sin >> src >> targets >> dst;
            md.trampolines += src;
            md.trampolines += dst;
        }
    }

    return new State(new MineMap(rows), md);
}
