//FONTE: https://blogs.oracle.com/linux/disown%2c-zombie-children%2c-and-the-uninterruptible-sleep-v2

#include <unistd.h>

int main() {
    vfork();
    sleep(60);

    return 0;
}
