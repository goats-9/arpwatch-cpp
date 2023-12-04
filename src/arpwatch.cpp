#include "arpwatch.hpp"

void exit_handler(int s) {
    std::cout << "Caught signal " << s << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        std::cout << "Usage: ./arpwatch <interface>\n";
        return 0;
    }
    // Define signal handler
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    // Main code goes here
    while (1) extractARP(argv[1]);

    return 0;
}
