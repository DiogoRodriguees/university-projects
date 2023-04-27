#include <cstdio>
#include <cstdlib>
#include <array>
#include <iostream>
#include <string>

using namespace std;

void pipecomand(string strcmd)
{
    std::array<char, 80> buffer;

    FILE *pipe = popen(strcmd.c_str(), "r");

    if (!pipe)
    {
        std::cerr << "cannot open pipe read" << endl;
        return;
    }

    int c = 0;

    while (fgets(buffer.data(), 80, pipe) != NULL)
    {
        c++;
        std::cout << c << " " << buffer.data();
    }

    pclose(pipe);
}

int main()
{

    string strcmd = "";

    while (1)
    {
        std::cout << "enter a comand unix: ";
        std::getline(cin, strcmd);
        pipecomand(strcmd);
    }

    return 0;
}