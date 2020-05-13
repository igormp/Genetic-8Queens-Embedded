#include <array>
#include "genetic.cpp"
#include "mbed.h"
#include "ILI9341.h"
#include "Arial12x12.h"
#include "Arial24x23.h"

Serial pc(PA_2, PA_3); // TX, RX

// LCD data pins
PinName busPins[8] = {PA_10, PA_11, PB_12, PB_13,
                      PB_14, PB_15, PA_8, PA_9};

// create the lcd instance
ILI9341 myLCD(BUS_8, busPins, PA_15, PA_12,
              PB_3, PB_4, PB_6, "myLCD"); // BUS 8bit, Bus pins, CS, reset, DC, WR, RD, name

DigitalOut myLed(PC_13);
AnalogIn noise(PB_1);
Timer t;

unsigned short bgColor = Black;
unsigned short fgColor = White;

char dispOrient = 2;

void drawGrid(unsigned short color = fgColor)
{
    for (int i = 0; i < 9; i++)
    {
        myLCD.line(0, (80 + (30 * i)),
                   myLCD.width() - 1, (80 + (30 * i)), color);

        myLCD.line(((i * myLCD.width()) / 8), 80, ((i * myLCD.width()) / 8),
                   myLCD.height() - 1, color);
    }
}

void drawQueens(std::array<uint8_t, 8> *queens, unsigned short color = fgColor)
{
    static std::array<uint8_t, 8> previ = *queens;
    for (int i = 0; i < 8; i++)
    {
        myLCD.fillcircle(15 + ((30 * previ[i]) % 240), 95 + ((30 * i) % 240), 10, bgColor);
    }
    previ = *queens;
    for (int i = 0; i < 8; i++)
    {
        myLCD.fillcircle(15 + ((30 * (*queens)[i]) % 240), 95 + ((30 * i) % 240), 10, color);
    }
}

int main()
{
    uint16_t val = noise.read_u16();
    std::srand(val);

    myLCD.set_orientation(dispOrient);

    myLed = 1;

    myLCD.background(bgColor); // set background to black
    myLCD.foreground(fgColor); // set chars to white

    drawGrid();

    int epochs = 0;
    myLCD.set_font((unsigned char *)Arial24x23);

    t.start();

    population pop(150);

    while (pop.boards[0].fitness != 0)
    {
        pc.printf("Fittest: ");
        for (auto &i : pop.boards[0].rows)
        {
            pc.printf(" %u", i);
        }
        pc.printf(" fitness: %u", pop.boards[0].fitness);
        pc.printf(" | Worst: ");
        for (auto &i : pop.boards.back().rows)
        {
            pc.printf(" %u", i);
        }
        pc.printf(" fitness: %u\r\n", pop.boards.back().fitness);

        drawQueens(&pop.boards[0].rows);
        myLCD.locate(0, 0);
        myLCD.printf("Epoch: %d\nBest fitness: %u\n", epochs, pop.boards[0].fitness);

        pop.reproduce(100);
        pop.selection();
        epochs++;
        myLed != myLed;
    }

    t.stop();

    myLCD.locate(0, 0);
    myLCD.printf("Epoch: %d\nBest fitness: %u\n", epochs, pop.boards[0].fitness);

    drawGrid(DarkGreen);
    drawQueens(&pop.boards[0].rows, DarkGreen);

    myLCD.set_font((unsigned char *)Arial12x12);
    myLCD.printf("DONE! Time taken: %fs", t.read());

    pc.printf("Took %d epochs ", epochs);
    for (auto &i : pop.boards[0].rows)
    {
        pc.printf(" %u", i);
    }
    pc.printf("time taken: %fs\r\n", t.read());
}