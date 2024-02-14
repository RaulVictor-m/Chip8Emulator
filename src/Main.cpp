#include "Chip8.h"

int main()
{
    Chip8 emulator;
    while (true) {

        std::cout << "PUT THE PATH OF THE CHOSEN ROM:\n";
        char buffer[200];
        std::cin >> buffer;
        if (emulator.InitEmulator(buffer))break;
        std::cout << "NOT VALID TRY AGAIN\n";
    }
    if (emulator.Construct(64 * 1, 32 * 1, 12, 12))
        emulator.Start();

    return 0;
}
