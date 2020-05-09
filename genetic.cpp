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
        uint8_t crossoverPoint);
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
             uint8_t crossoverPoint)
{
    //uint8_t crossoverPoint = 1 + (std::rand() / ((RAND_MAX + 1u) / 7));

    // Populates our board
    for (uint8_t i = 0; i < crossoverPoint; i++)
    {
        rows[i] = (*parent1)[i];
    }
    for (uint8_t i = crossoverPoint; i < rows.size(); i++)
    {
        rows[i] = (*parent2)[i];
    }

    checkFitness();
}

board::~board()
{
}

void board::checkFitness()
{
    fitness = 0;

    // Hashtable to easily verify queens on the same row
    std::array<uint8_t, 8> values;
    values.fill(0);
    for (uint8_t i = 0; i < rows.size(); i++)
    {
        values[rows[i]]++;
    }

    // Increases fitness for every queen in a row
    for (auto i = 0; i < values.size(); i++)
    {
        if (values[i] > 1)
        {
            fitness += values[i] - 1;
        }
    }

    // Increases fitness for every queen in a diagonal
    for (auto i = 0; i < rows.size(); i++)
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
    std::vector<board> boards;
    population(uint8_t size = 50);
    ~population();
    void sortPopulation();
};

population::population(uint8_t size)
{
    for (auto i = 0; i < size; i++)
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