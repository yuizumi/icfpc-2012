#include <iostream>
#include "input.h"
#include "solve.h"
#include "solveutil.h"

using namespace std;

int main()
{
    SetupForSignal();
    Solver solver(Read(cin));
    solver.Solve();
    WriteOutput();
#ifndef NDEBUG
    cout << "score: " << solver.max_score() << endl;
#endif
    return 0;
}
