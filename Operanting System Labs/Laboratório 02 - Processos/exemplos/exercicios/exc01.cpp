#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void createChildProcess(int level, int maxLevel) {
    if (level >= maxLevel) {
        cout << "Final process at level " << level << " (PID: " << getpid() << ")" << endl;
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        cout << "Error: fork failed!" << endl;
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        cout << "Process at level " << level << " (PID: " << getpid() << ")" << endl;
        createChildProcess(level + 1, maxLevel);
    }
    else {
        wait(NULL);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <number of levels>" << endl;
        return 1;
    }

    int maxLevel = atoi(argv[1]);

    cout << "Creating process hierarchy with " << maxLevel << " levels" << endl;
    createChildProcess(0, maxLevel);

    return 0;
}