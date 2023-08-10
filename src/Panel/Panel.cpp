#include "Panel.h"

Panel::Panel()
{
    matrix = new Adafruit_NeoMatrix(64, 32, 15);
    init();
    started = false;
    university = string("Stretnutie katedier 2023");
    size = len(university.c_str(), university.length());
    index = 32;
    printf("%d\n\r", size);
}

int Panel::len(const char *str, int length)
{
    int size = 0;
    for (int i = 0; i < length; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            size += LOWER_CASE;
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            size += UPPER_CASE;
        }
        else if (str[i] == '1')
        {
            size += ONE;
        }
        else if (str[i] >= '0' && str[i] <= '9')
        {
            size += NUMBER;
        }
        else if (str[i] == ' ')
        {
            size += WHITESPACE;
        }
        else if (str[i] == ':')
        {
            size += COLON;
        }
    }
    return size;
}

int Panel::timerOffset(const char *str, int length)
{
    int offset = 7;
    for (int i = 0; i < length; i++)
    {
        if (str[i] == '1')
        {
            offset += NUMBER - ONE;
        }
    }
    return offset;
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
    matrix->setBrightness(10);
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
                    lastTimeChange = (uint32_t)esp_timer_get_time() / 1000;
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
                    university = st->getString();
                    size = len(university.c_str(), university.length());
                    started = false;
                    printf("%d\n\r", size);
                    time = st->getData();
                    actualTime.decr = true;
                    actualTime.minutes = time;
                    actualTime.seconds = 0;
                    printf("data %s for %d\n\r", university.c_str(), actualTime.minutes);
                    lastStringChange = (uint32_t)esp_timer_get_time() / 1000;
                    index = 16;
                    isChange = true;
                    if (size < 64)
                    {
                        index = (int)((64 - size) / 2);
                    }
                };
                break;
                case NEW_BRIGHTNESS:
                {
                    matrix->setBrightness(st->getBrightness());
                    isChange = true;
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
                if (((uint32_t)esp_timer_get_time() / 1000) - lastTimeChange > DELAY_TIME)
                {
                    changeTime();
                    printf("%d:%d\n\r", actualTime.minutes, actualTime.seconds);
                    isChange = true;
                    lastTimeChange = ((uint32_t)esp_timer_get_time() / 1000);
                }
            }
            matrix->fillScreen(0);
            matrix->setTextColor(matrix->Color(0, 0, 255));
            if (((uint32_t)esp_timer_get_time() / 1000) - lastStringChange > DELAY_STRING)
            {
                if (size > 64)
                {
                    printf("%d\n\r", this->index);
                    if ((this->index) < (size * (-1)))
                    {
                        this->index = 64;
                    }
                    this->index--;
                    isChange = true;
                    lastStringChange = ((uint32_t)esp_timer_get_time() / 1000);
                    printf("%d\n\r", this->index);
                }
            }
            matrix->setCursor(index, 14);
            matrix->print(F((university).c_str()));
            if (actualTime.decr)
            {
                matrix->setTextColor(matrix->Color(0, 255, 0));
            }
            else
            {
                matrix->setTextColor(matrix->Color(255, 0, 0));
            }
            char time_str[5];
            sprintf(time_str, "%d%d:%d%d", actualTime.minutes / 10, actualTime.minutes % 10, actualTime.seconds / 10, actualTime.seconds % 10);
            matrix->setCursor(timerOffset(time_str, 2), 30);
            matrix->print(F(time_str));
            if (isChange)
            {
                matrix->show();
                isChange = false;
            }
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