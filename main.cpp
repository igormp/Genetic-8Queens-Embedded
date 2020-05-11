#include "mbed.h"
#include "genetic.cpp"

Serial pc(PA_2, PA_3); // TX, RX
DigitalOut myled(PC_13);
AnalogIn noise(PB_1);

int main()
{
    uint16_t val = noise.read_u16();
    std::srand(val);

    population pop(10);

    int epochs = 0;

    while (pop.boards[0].fitness != 0)
    {
        printf("Fitest: ");
        for (auto &i : pop.boards[0].rows)
        {
            printf(" %u", i);
        }
        printf(" fitness: %u", pop.boards[0].fitness);
        printf(" | Worst: ");
        for (auto &i : pop.boards.back().rows)
        {
            printf(" %u", i);
        }
        printf(" fitness: %u\r\n", pop.boards.back().fitness);

        pop.reproduce(20);
        pop.selection();
        epochs++;
    }
    printf("Took %d epochs ", epochs);
    for (auto &i : pop.boards[0].rows)
    {
        printf(" %u", i);
    }
}
