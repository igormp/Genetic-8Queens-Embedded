#include <cstdint>
#include <array>

class board
{
public:
    board();
    ~board();

    std::array<uint8_t, 8> rows;
    uint8_t fitness;

    void checkFitness();
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