#include <ncurses/ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define TB_HEIGHT 5
#define TB_WIDTH 25
#define COUNT_DROPS 100

typedef struct
{
    float x, y;
    float vx, vy;
} Raindrop;

Raindrop drops[COUNT_DROPS];
float vx;

void genRaindrop(int i, bool scatter)
{
    int width, height;
    getmaxyx(stdscr, height, width);

    drops[i].x = rand() % (width - 2);

    if (scatter)
        drops[i].y = rand() % (height - 2);
    else
        drops[i].y = 0;

    drops[i].vx = vx;
    drops[i].vy = 1;
}

void initRaindrops(void)
{
    vx = rand() % 9 - 4; // -4 to +4

    for (int i = 0; i < COUNT_DROPS; i++)
    {
        genRaindrop(i, true);
    }
}

void moveRain(WINDOW *rain_win)
{
    int width, height;
    getmaxyx(rain_win, height, width);

    for (int i = 0; i < COUNT_DROPS; i++)
    {
        drops[i].x += drops[i].vx;
        drops[i].y += drops[i].vy;

        if (drops[i].x < 0 ||
            drops[i].x >= width ||
            drops[i].y >= height)
        {
            genRaindrop(i, false);
        }
    }
}

void drawRain(WINDOW *rain_win)
{
    int width, height;
    getmaxyx(rain_win, height, width);

    char symbol;
    if (vx < 0)
        symbol = '/';
    else if (vx > 0)
        symbol = '\\';
    else
        symbol = '|';

    werase(rain_win);

    for (int i = 0; i < COUNT_DROPS; i++)
    {
        int x = (int)drops[i].x;
        int y = (int)drops[i].y;

        if (x >= 0 && x < width &&
            y >= 0 && y < height)
        {
            mvwaddch(rain_win, y, x, symbol);
        }
    }
}

void drawTitleBox(WINDOW *title_win)
{
    werase(title_win);

    box(title_win, 0, 0);

    const char *title = "Raindrops";

    mvwaddstr(
        title_win,
        TB_HEIGHT / 2,
        (TB_WIDTH - strlen(title)) / 2,
        title
    );
}

int main(void)
{
    srand((unsigned)time(NULL));

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    int width, height;
    getmaxyx(stdscr, height, width);

    WINDOW *rain_win =
        newwin(height - 2, width - 2, 1, 1);

    WINDOW *title_win =
        newwin(
            TB_HEIGHT,
            TB_WIDTH,
            (height - TB_HEIGHT) / 2,
            (width - TB_WIDTH) / 2);

    initRaindrops();

    int ch;

    while ((ch = getch()) != 'q')
    {
        erase();

        box(stdscr, 0, 0);

        mvprintw(
            height - 1,
            2,
            "Velocity X = %.0f | Press q to quit",
            vx);

        moveRain(rain_win);

        drawRain(rain_win);
        drawTitleBox(title_win);

        wnoutrefresh(stdscr);
        wnoutrefresh(rain_win);
        wnoutrefresh(title_win);

        doupdate();

        napms(100);
    }

    delwin(rain_win);
    delwin(title_win);

    endwin();

    return 0;
}