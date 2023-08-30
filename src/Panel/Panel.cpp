#include "Panel.h"

Panel::Panel()
{
    matrix = new Adafruit_NeoMatrix(64, 32, 15);
    init();
    started = false;
    university = string("SKAKUII 23");
    size = len(university.c_str(), university.length());
    index = 32;
    time = 0;
    printf("%d\n\r", size);
    program = new Program();
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

int Panel::timerOffset(const char *str, int length, int font)
{
    int offset = 0;
    if (font == 0)
    {
        offset = 7;
        for (int i = 0; i < length; i++)
        {
            if (str[i] == '1')
            {
                offset += NUMBER - ONE;
            }
        }
    }
    else
    {
        offset = 23;
        for (int i = 0; i < length; i++)
        {
            if (str[i] == '1')
            {
                offset += 5 - 4;
            }
        }
    }
    return offset;
}

Panel::~Panel()
{
}

void Panel::setup(State *st, SemaphoreHandle_t mut, TaskHandle_t handle)
{
    this->st = st;
    this->mutex = mut;
    this->handle = handle;
}

void Panel::setRTC(RTC *rtc, SemaphoreHandle_t mut)
{
    this->rtc = rtc;
    this->rtcMutex = mut;
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
            if (st->getMode())
            {
                if (st->getChange())
                {
                    matrix->setFont(&dotmat10pt7b_v2);
                    st->resetChange();
                }
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
                        printf("%d:%d\n\r", actualTime.minutes, actualTime.seconds);
                    };
                    break;
                    case NEW_DATA:
                    {
                        if (st->getString().length() > 0)
                        {
                            university = st->getString();
                        }
                        size = len(university.c_str(), university.length());
                        started = false;
                        // printf("%d\n\r", size);
                        time = st->getData();
                        actualTime.decr = true;
                        actualTime.minutes = time;
                        actualTime.seconds = 0;
                        // printf("data %s for %d\n\r", university.c_str(), actualTime.minutes);
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
                        //
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
                        // printf("%d\n\r", this->index);
                        if ((this->index) < (size * (-1)))
                        {
                            this->index = 64;
                        }
                        this->index--;
                        isChange = true;
                        lastStringChange = ((uint32_t)esp_timer_get_time() / 1000);
                        // printf("%d\n\r", this->index);
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
            else
            {
                if (st->getChange())
                {
                    matrix->setFont(&font);
                    matrix->setTextColor(matrix->Color(0, 0, 255));
                    this->lastProgramChange = ((uint32_t)esp_timer_get_time() / 1000);
                    program->getUpcommingEvents(rtc->getActualTime(), this->lastProgram);
                }
                if (xSemaphoreTake(rtcMutex, 100) == pdPASS)
                {
                    if (st->getState() == NEW_BRIGHTNESS)
                    {
                        matrix->setBrightness(st->getBrightness());
                    }
                    if (rtc->getChange() || st->getChange())
                    {
                        matrix->fillScreen(0);
                        string prg[3];
                        program->getUpcommingEvents(rtc->getActualTime(), prg);
                        if ((prg[0].length() != this->lastProgram[0].length()) ||
                            (memcmp(prg[0].c_str(), this->lastProgram[0].c_str(), min(prg[0].length(), this->lastProgram[0].length()))))
                        {
                            program->getUpcommingEvents(rtc->getActualTime(), this->lastProgram);
                            this->iteration = 0;
                        }
                        rtc->removeChange();
                    }
                    if ((((uint32_t)esp_timer_get_time() / 1000) - lastProgramChange) > DELAY_PROGRAM)
                    {
                        this->iteration++;
                        lastProgramChange =((uint32_t)esp_timer_get_time() / 1000);
                        char time_str[10];
                        sprintf(time_str, "%d%d:%d%d", rtc->getActualTime().hour / 10, rtc->getActualTime().hour % 10, rtc->getActualTime().minutes / 10, rtc->getActualTime().minutes % 10);
                        matrix->setCursor(timerOffset(time_str, 2, 1), 8);
                        matrix->fillScreen(0);
                        matrix->print(F(time_str));
                        placeProgram(this->lastProgram[0], 1, this->iteration);
                        placeProgram(this->lastProgram[1], 2, this->iteration);
                        placeProgram(this->lastProgram[2], 3, this->iteration);
                        matrix->show();
                    }
                    xSemaphoreGive(rtcMutex);
                }
                st->resetChange();
                xSemaphoreGive(mutex);
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
            if (this->actualTime.minutes == 0 && this->actualTime.seconds == 0)
            {
                xTaskNotify(this->handle, 1, eSetBits);
            }
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

void Panel::placeProgram(string program, int column, int iteration)
{
    if (program.length() > 6)
    {
        int indexY = (column + 1) * 8;
        int indexX = 6 + iteration % (program.length() - 6);
        matrix->setCursor(0, indexY);
        matrix->setTextColor(matrix->Color(0, 255, 0));
        matrix->print(F(program.substr(0, 5).c_str()));
        matrix->setCursor(22, indexY);
        matrix->setTextColor(matrix->Color(0, 0, 255));
        matrix->print(F(program.substr(indexX, program.length() - indexX).c_str()));
        //printf("%s\n\r", program.c_str());
    }
}