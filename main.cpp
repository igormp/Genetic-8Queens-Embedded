#include <iostream>
#include <ctime>
#include "genetic.cpp"

int main(int argc, char const *argv[])
{
    std::srand(std::time(nullptr));
    uint32_t x = atoi(argv[1]);
    uint32_t y = atoi(argv[1]);

    population pop(x);

    int epochs = 0;

    while (pop.boards[0].fitness != 0)
    {
        std::cout << "Fitest: ";
        for (auto &i : pop.boards[0].rows)
        {
            std::cout << " " << unsigned(i);
        }
        std::cout << " fitness: " << unsigned(pop.boards[0].fitness);
        std::cout << " | Worst: ";
        for (auto &i : pop.boards.back().rows)
        {
            std::cout << " " << unsigned(i);
        }
        std::cout << " fitness: " << unsigned(pop.boards.back().fitness) << std::endl;

        pop.reproduce(y);
        pop.selection();
        epochs++;
    }
    std::cout << "Took " << epochs << " epochs ";
    for (auto &i : pop.boards[0].rows)
    {
        std::cout << " " << unsigned(i);
    }

    return 0;
}
