#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "debugutil.h"
#include "input.h"
#include "state.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cerr << argv[0] << ": Invalid number of arguments." << endl;
        return 1;
    }

    boost::scoped_ptr<State> q(Read(cin));
    PrintState(cout, *q);

    int num_lams = q->num_lams();

    const char* p = argv[1];
    while (!( *p == '\0' || *p == 'A' || q->IsGameOver() )) {
        q->Command(*p);
        ++p;
        PrintState(cout, *q);
    }

    cout << "\033[32m";
    int lambda_pts = 50;
    if (q->IsSuccess()) { cout << "[Win!] "; lambda_pts = 75; }
    if (q->IsFailure()) { cout << "[Lose] "; lambda_pts = 25; }
    int score = (num_lams - q->num_lams()) * lambda_pts - q->commands().length();
    cout << "Score: " << score << "\033[m" << endl;

    return 0;
}
