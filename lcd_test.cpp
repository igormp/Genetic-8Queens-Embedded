#include "mbed.h"
#include "ILI9341.h"
#include "string"
#include "Arial12x12.h"
#include "Arial24x23.h"
#include "Arial43x48_numb.h"

Serial pc(PA_2, PA_3); // TX, RX

// LCD data pins
PinName busPins[8] = {PA_10, PA_11, PB_12, PB_13,
                      PB_14, PB_15, PA_8, PA_9};

// create the lcd instance
ILI9341 myLCD(BUS_8, busPins, PA_15, PA_12,
              PB_3, PB_4, PB_6, "myLCD"); // BUS 8bit, Bus pins, CS, reset, DC, WR, RD, name

Timer t;

unsigned short backgroundcolor = Black;
unsigned short foregroundcolor = White;

char orient = 1;

int main()
{
    pc.printf("hello ");

    myLCD.set_orientation(orient);
    int time, time2;

    pc.printf("\n\nSystem Core Clock = %.3f MHZ\r\n", (float)SystemCoreClock / 1000000);
    t.start();
    //   myLCD.set_font((unsigned char*) Terminal6x8);
    // myLCD.claim(stdout);      // send stdout to the LCD display
    //myLCD.claim(stderr);      // send stderr to the LCD display
    myLCD.background(backgroundcolor); // set background to black
    myLCD.foreground(foregroundcolor); // set chars to white

    while (1)
    {
        myLCD.set_orientation((orient++) % 4);
        //    myLCD.set_orientation(2);
        myLCD.cls(); // clear the screen
        myLCD.locate(0, 30);
        myLCD.printf("Display ID: %.8X\r\n", myLCD.tftID);
        pc.printf("Display ID: %.8X\r\n", myLCD.tftID);
        // mem write/read test
        unsigned short readback;
        unsigned short colorstep = (0x10000 / myLCD.width());
        for (unsigned short i = 0; i < myLCD.width(); i++)
        {
            myLCD.pixel(i, 0, i * colorstep); // write line
        }
        bool readerror = false;
        for (unsigned short i = 0; i < myLCD.width(); i++) // verify line
        {
            readback = myLCD.pixelread(i, 0);
            if (readback != i * colorstep)
            {
                readerror = true;
                pc.printf("pix %.4X readback %.4X\r\n", i * colorstep, readback);
            }
        }
        myLCD.locate(0, 10);
        myLCD.printf("pixelread test %s\r\n", readerror ? "FAIL" : "PASS");
        wait(2);

        myLCD.cls();
        myLCD.set_font((unsigned char *)Terminal6x8, 32, 127, false); //variable width disabled
        myLCD.locate(0, 0);
        myLCD.printf("Display Test\r\nSome text just to see if auto carriage return works correctly");
        myLCD.set_font((unsigned char *)Terminal6x8);
        //   myLCD.locate(0,0);
        myLCD.printf("\r\nDisplay Test\r\nSome text just to see if auto carriage return works correctly");
        pc.printf("  Display Test \r\n");
        wait(3);
        t.reset();
        myLCD.cls();
        time = t.read_us();
        myLCD.locate(2, 55);
        myLCD.printf("cls: %.3fms", (float)time / 1000);
        pc.printf("cls: %.3fms\r\n", (float)time / 1000);
        wait(3);

        myLCD.cls();
        t.reset();
        // draw some graphics
        //myLCD.cls();
        myLCD.set_font((unsigned char *)Arial24x23);
        myLCD.locate(10, 10);
        myLCD.printf("Test");

        myLCD.line(0, 0, myLCD.width() - 1, 0, foregroundcolor);
        myLCD.line(0, 0, 0, myLCD.height() - 1, foregroundcolor);
        myLCD.line(0, 0, myLCD.width() - 1, myLCD.height() - 1, foregroundcolor);

        myLCD.rect(10, 30, 50, 40, foregroundcolor);
        myLCD.fillrect(60, 30, 100, 40, foregroundcolor);

        myLCD.circle(150, 32, 30, foregroundcolor);
        myLCD.fillcircle(140, 20, 10, foregroundcolor);

        double s;

        for (unsigned short i = 0; i < myLCD.width(); i++)
        {
            s = 10 * sin((long double)i / 10);
            myLCD.pixel(i, 40 + (int)s, foregroundcolor);
        }

        time = t.read_us();
        myLCD.locate(2, 55);
        myLCD.set_font((unsigned char *)Terminal6x8);
        myLCD.printf("plot: %.3fms", (float)time / 1000);
        pc.printf("plot: %.3fms\r\n", (float)time / 1000);
        wait(3);
        myLCD.cls();
        myLCD.set_font((unsigned char *)Arial43x48_numb, 46, 58, false); //only numbers, variable-width disabled
        t.reset();
        myLCD.locate(0, 0);
        myLCD.printf("%d", 12345);
        time = t.read_us();
        myLCD.locate(2, 55);
        myLCD.set_font((unsigned char *)Terminal6x8);
        myLCD.printf("Big Font: %.3fms", (float)time / 1000);
        pc.printf("Big Font: %.3fms\r\n", (float)time / 1000);
        wait(3);
        // sparse pixels test
        myLCD.cls();
        myLCD.FastWindow(true);
        t.reset();
        for (unsigned int i = 0; i < 20000; i++)
        {
            myLCD.pixel((i + (i * 89) % myLCD.width()), (i + (i * 61) % myLCD.height()), White);
        }
        myLCD.copy_to_lcd();
        time = t.read_us();
        myLCD.cls();
        myLCD.FastWindow(false);
        t.reset();
        for (unsigned int i = 0; i < 20000; i++)
        {
            myLCD.pixel((i + (i * 89) % myLCD.width()), (i + (i * 61) % myLCD.height()), White);
        }
        myLCD.copy_to_lcd();
        time2 = t.read_us();
        myLCD.locate(2, 55);
        myLCD.printf("std:%.3fms fastw:%.3fms", (float)time2 / 1000, (float)time / 1000);
        pc.printf("std: %.3fms fastw: %.3fms\r\n", (float)time2 / 1000, (float)time / 1000);
        wait(3);
        // scroll test, only for TFT
        myLCD.cls();
        myLCD.set_font((unsigned char *)Arial24x23);
        myLCD.locate(2, 10);
        myLCD.printf("Scrolling");
        myLCD.rect(0, 0, myLCD.width() - 1, myLCD.height() - 1, White);
        myLCD.rect(1, 1, myLCD.width() - 2, myLCD.height() - 2, Blue);
        myLCD.setscrollarea(0, myLCD.sizeY());
        wait(1);
        myLCD.scroll(1); //up 1
        wait(1);
        myLCD.scroll(0); //center
        wait(1);
        myLCD.scroll(myLCD.sizeY() - 1); //down 1
        wait(1);
        myLCD.scroll(myLCD.sizeY()); // same as 0, center
        wait(1);
        myLCD.scroll(myLCD.sizeY() >> 1); // half screen
        wait(1);
        myLCD.scrollreset(); // center
        wait(1);
        for (unsigned short i = 1; i <= myLCD.sizeY(); i++)
        {
            myLCD.scroll(i);
            wait_ms(2);
        }
        wait(2);
        // color inversion
        for (unsigned short i = 0; i <= 8; i++)
        {
            myLCD.invert(i & 1);
            wait_ms(200);
        }
        wait(2);
    }

    pc.printf("done \r\n");
}