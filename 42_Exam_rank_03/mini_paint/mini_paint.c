#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct  s_bg
{
    int     width;
    int     height;
    char    c; 
}               t_bg;

typedef struct  s_draw
{
    char    type;
    float   x;
    float   y; 
    float   radius;
    char    c;
}               t_draw;

static int     msg_error(const char *msg, int ret)
{
    printf("%s\n", msg);
    return (ret);
}

static int  check_zone(FILE *file, t_bg *paper, char **draw)
{
    int     get;

    get = 0;
    if ((get = fscanf(file, "%d %d %c\n", &paper->width, &paper->height, &paper->c)) != 3)
        return (0);
    if (!(paper->width > 0 && paper->width <= 300) || !(paper->height > 0 && paper->height <= 300))
        return (0);
    if (get == -1)
        return (0);
    *draw = calloc(paper->width * paper->height, sizeof(char));
    if (!*draw)
        return (0);
    memset(*draw, paper->c, paper->width * paper->height);
    return (1);
}

static int  creat_cicle(float x, float y, t_draw *cicle)
{
    // coming soon ...
}

static void  draw_cicle(t_bg *paper, char **draw, t_draw *cicle)
{
    int i;
    int j;

    if (cicle->type == 'R')
    {
        i = 0;
        while (i < paper->height)
        {
            j = 0;
            while (j < paper->width)
            {
                if (creat_cicle((float)j, (float)i, cicle))
                    (*draw)[i * paper->width + j] = cicle->c;
                ++j;
            }
            ++i;
        }
    }
    else if (cicle->type == 'r')
    {
        i = 0;
        while (i < paper->height)
        {
            j = 0;
            while (j < paper->width)
            {
                if (creat_cicle((float)j, (float)i, cicle) == 2)
                    (*draw)[i * paper->width + j] = cicle->c;
                ++j;
            }
            ++i;
        }
    }
}

static int  check_cicle(FILE *file, t_bg *paper, char **draw)
{
    t_draw  cicle;
    int     get;

    get = 0;
    while ((get = fscanf(file, "%c %f %f %f %c\n", &cicle.type, &cicle.x, &cicle.y, &cicle.radius, &cicle.c)) == 6)
    {
        if (!(cicle.radius > 0. && (cicle.type == 'r' || cicle.type == 'R')))
            return (0);
        draw_cicle(paper, draw, &cicle);
    }
    if (get != -1)
        return (0);
    return (1);
}

static void paint_all(char *draw, t_bg paper)
{
    int i;
    int j;

    i = 0;
    while (i < paper.height)
    {
        j = 0;
        while (j < paper.width)
            printf("%c", draw[i * paper.width + j++]);
        printf("\n");
        ++i;
    }
    if (draw)
        free(draw); 
}

int main(int ac, char **av)
{
    FILE    *file;
    t_bg    paper;
    char    *draw;

    memset(&paper, 0, sizeof(t_bg));
    if (ac != 2)
        return(msg_error("Error: argument" ,1));
    if (!(file = fopen(av[1], "r")))
        return(msg_error("Error: Operation file corrupted" ,1));
    if (!(check_zone(file, &paper, &draw)))
    {
        fclose(file);
        return(msg_error("Error: Operation file corrupted" ,1));
    }
    if (!(check_cicle(file, &paper, &draw)))
    {
        fclose(file);
        return(msg_error("Error: Operation file corrupted" ,1));
    }
    paint_all(draw, paper);
    fclose(file);
    return (0);
}