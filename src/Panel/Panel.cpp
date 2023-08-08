#include "Panel.h"

Panel::Panel()
{
    matrix = new Adafruit_NeoMatrix(64, 32, 15);
    init();
}

Panel::~Panel()
{
}

void Panel::setup(State *st, SemaphoreHandle_t mut)
{
    this->st = st;
    this->mutex = mut;
}

uint16_t Panel::ledInPanel(uint16_t x, uint16_t y, bool odd)
{
    if (!odd)
        return 16 * (x) + ((x % 2) ? 16 - y - 1 : y);
    else
        return 16 * (16 - x - 1) + ((x % 2) ? 16 - y - 1 : y);
}
uint16_t Panel::myRemapFn(uint16_t x, uint16_t y)
{
    int panel_X = x / PANEL_W;
    int panel_Y = y / PANEL_H;
    int base_matrix[2][4] = {{0, 3, 4, 7},
                             {1, 2, 5, 6}};

    return PANEL_COUNT * base_matrix[panel_Y][panel_X] + ledInPanel(x % PANEL_W, y % PANEL_H, panel_X % 2);
}

void Panel::init()
{
    matrix->setRemapFunction(myRemapFn);
    matrix->begin();
    matrix->setTextWrap(false);
    matrix->setBrightness(20);
    matrix->setFont(&dotmat10pt7b_v2);
    matrix->fillScreen(0);
    matrix->setTextColor(matrix->Color(0, 0, 255));
    matrix->setCursor(2, 14);
    matrix->print(F("UNIZA"));
    matrix->setTextColor(matrix->Color(0, 255, 0));
    matrix->setCursor(2, 30);
    matrix->print(F("00:00"));
    matrix->show();
}

void Panel::run()
{
    while (true)
    {
        if (xSemaphoreTake(mutex, 100) == pdPASS)
        {
            if (st->getState() != NONE)
            {
                switch (st->getState())
                {
                case START:
                {
                    printf("Clocks started\n\r");
                };
                break;
                case STOP:
                {
                    printf("Clocks stoped\n\r");
                };
                break;
                case RESET:
                {
                    printf("Clocks reset\n\r");
                };
                break;
                case NEW_DATA:
                {
                    printf("data %s for %d\n\r", st->getString().c_str(), st->getData());
                };
                break;
                default:
                    break;
                }
                st->setState(NONE);
                xSemaphoreGive(mutex);
            }
            else
            {
                xSemaphoreGive(mutex);
            }
        }
    }
}