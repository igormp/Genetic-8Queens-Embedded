#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

class board
{
public:
    board();
    board(
        std::array<uint8_t, 8> *parent1, std::array<uint8_t, 8> *parent2,
        uint8_t crossoverPoint, uint16_t mutationRatio = 10);
    ~board();

    std::array<uint8_t, 8> rows;
    uint8_t fitness;

    void checkFitness();
    void mutate();
};

board::board()
{
    // Populates our board
    for (uint8_t i = 0; i < rows.size(); i++)
    {
        uint8_t randVal = std::rand() / ((RAND_MAX + 1u) / 8);
        rows[i] = randVal;
    }

    checkFitness();
}

board::board(std::array<uint8_t, 8> *parent1, std::array<uint8_t, 8> *parent2,
             uint8_t crossoverPoint, uint16_t mutationRatio)
{
    // Populates our board
    for (uint8_t i = 0; i < crossoverPoint; i++)
    {
        rows[i] = (*parent1)[i];
    }
    for (uint8_t i = crossoverPoint; i < rows.size(); i++)
    {
        rows[i] = (*parent2)[i];
    }
    if (1 == (std::rand() / ((RAND_MAX + 1u) / mutationRatio)))
    {
        mutate();
    }

    checkFitness();
}

board::~board()
{
}

void board::checkFitness()
{
    fitness = 0;

    // Hashtable to easily verify queens on the same column
    std::array<uint8_t, 8> values;
    values.fill(0);
    for (uint8_t i = 0; i < rows.size(); i++)
    {
        values[rows[i]]++;
    }

    // Increases fitness for every queen in the same column
    for (unsigned int i = 0; i < values.size(); i++)
    {
        if (values[i] > 1)
        {
            fitness += values[i] - 1;
        }
    }

    // Increases fitness for every queen in a diagonal
    for (unsigned int i = 0; i < rows.size(); i++)
    {
        if (rows[i] < 7)
        {
            auto temp = rows[i];
            for (auto j = i + 1; j < rows.size(); j++)
            {
                temp += 1;
                if (temp > 7)
                    break;
                if (temp == rows[j])
                {
                    fitness++;
                }
            }
        }

        if (rows[i] > 0)
        {
            auto temp = rows[i];
            for (auto j = i + 1; j < rows.size(); j++)
            {
                temp -= 1;
                if (temp > 7)
                    break;
                if (temp == rows[j])
                {
                    fitness++;
                }
            }
        }
    }
}

void board::mutate()
{
    uint8_t temp1 = (std::rand() / ((RAND_MAX + 1u) / 8));
    uint8_t temp2 = (std::rand() / ((RAND_MAX + 1u) / 8));
    rows[temp1] = temp2;
}

class population
{
public:
    population();
    population(uint32_t size);
    ~population();

    std::vector<board> boards;
    uint8_t size;

    void sortPopulation();
    void reproduce(uint32_t amount = 50);
    void purge();
};

population::population() : population(50)
{
}

population::population(uint32_t size)
{
    this->size = size;
    for (uint32_t i = 0; i < size; i++)
    {
        board temp;
        boards.push_back(temp);
    }

    sortPopulation();
}

population::~population()
{
}

void population::sortPopulation()
{
    std::sort(
        boards.begin(), boards.end(), [](board a, board b) {
            return a.fitness < b.fitness;
        });
}

void population::reproduce(uint32_t amount)
{
    auto populationSize = boards.size();

    for (uint32_t i = 0; i < amount; i++)
    {
        auto first = (std::rand() / ((RAND_MAX + 1u) / populationSize));
        auto second = (std::rand() / ((RAND_MAX + 1u) / populationSize));

        uint8_t crossoverPoint = 1 + (std::rand() / ((RAND_MAX + 1u) / 7));

        board temp(&(boards[first].rows), &(boards[second].rows), crossoverPoint);
        boards.push_back(temp);
    }
    sortPopulation();
}

void population::purge()
{
    uint32_t len = boards.size();

    // Let's keep diversity in our gene pool
    for (uint32_t i = 0; i < size / 10; i++)
    {
        std::iter_swap(boards.begin() + size - i, boards.end() - i);
    }

    for (uint32_t i = size; i < len; i++)
    {
        boards.pop_back();
    }
}
