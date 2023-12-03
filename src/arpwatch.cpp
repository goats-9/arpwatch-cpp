#include "arpwatch.hpp"

void exit_handler(int s) {
    std::cout << "Caught signal " << s << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
    // Define signal handler
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    // Main code goes here
    while (1) {

    }

    return 0;
}
