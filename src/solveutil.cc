#include "solveutil.h"
#include <assert.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

namespace {

string* saved;

void HandleSigint(int sig)
{
    write(1, saved->c_str(), saved->length());
    write(1, "\n", 1);
    _exit(0);
}

};

bool SetupForSignal()
{
    saved = new string();
    return signal(SIGINT, HandleSigint) != SIG_ERR;
}

void SaveOutput(const string& output)
{
    assert(saved);
    *saved = output;  // not sure of signal-safety
}

void WriteOutput()
{
    write(1, saved->c_str(), saved->length());
    write(1, "\n", 1);
}
