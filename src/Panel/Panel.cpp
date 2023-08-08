#include "Panel.h"
//TODO: Movement of university name
Panel::Panel()
{
    matrix = new Adafruit_NeoMatrix(64, 32, 15);
    init();
    started = false;
    university = string("Presentation panel");
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
                    started = true;
                };
                break;
                case STOP:
                {
                    started = false;
                    printf("Clocks stoped\n\r");
                };
                break;
                case RESET:
                {
                    started = false;
                    actualTime.minutes = time;
                    actualTime.seconds = 0;
                    actualTime.decr = true;
                    printf("Clocks reset\n\r");
                };
                break;
                case NEW_DATA:
                {
                    printf("data %s for %d\n\r", st->getString().c_str(), st->getData());
                    university = st->getString();
                    time = st->getData();
                    actualTime.decr = true;
                    actualTime.minutes = time;
                    actualTime.seconds = 0;
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
            if (started)
            {
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                changeTime();
                printf("%d:%d\n\r", actualTime.minutes, actualTime.seconds);
            }
            matrix->fillScreen(0);
            matrix->setTextColor(matrix->Color(0, 0, 255));
            matrix->setCursor(2, 14);
            matrix->print(F(university.c_str()));
            if (actualTime.decr)
            {
                matrix->setTextColor(matrix->Color(0, 255, 0));
            }
            else
            {
                matrix->setTextColor(matrix->Color(255, 0, 0));
            }
            matrix->setCursor(2, 30);
            char time_str[5];
            sprintf(time_str, "%d%d:%d%d", actualTime.minutes / 10, actualTime.minutes % 10, actualTime.seconds / 10, actualTime.seconds % 10);
            matrix->print(F(time_str));
            matrix->show();
        }
    }
}

void Panel::changeTime()
{
    if (this->actualTime.decr)
    {
        if (this->actualTime.seconds > 0)
        {
            this->actualTime.seconds--;
        }
        else
        {
            if (this->actualTime.minutes > 0)
            {
                this->actualTime.minutes--;
                this->actualTime.seconds = 59;
            }
            else
            {
                this->actualTime.seconds++;
                this->actualTime.decr = false;
            }
        }
    }
    else
    {
        if (this->actualTime.seconds <= 59)
        {
            this->actualTime.seconds++;
        }
        else
        {
            this->actualTime.minutes++;
            this->actualTime.seconds = 0;
        }
    }
}