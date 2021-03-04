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
    char    c;
    float   x;
    float   y; 
    float   width;
    float   height;
    char    cc;
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

// static int  check_rect()
// {
// }

static int  check_rect(FILE *file, t_bg *paper, char **draw)
{
    t_draw  rect;
    int     get;

    get = 0;
    while ((get = fscanf(file, "%c %f %f %f %f %c", &rect.c, &rect.x, &rect.y, &rect.width, &rect.height, &rect.cc)) == 6)
    {
        printf("ici\n");
        if (!(rect.width > 0. && rect.height > 0. && (rect.c == 'r' || rect.c == 'R')))
        {
            printf("wtf\n");
            return (0);
        }
    }
    if (get != -1)
    {
        printf("get\n");
        return (0);
    }
    return (1);
}

static void paint_all(char *draw, t_bg paper)
{
    int i;
    int j;

    i = 0;
    while (i < paper.width)
    {
        j = 0;
        while (j < paper.height)
            printf("%c", draw[j++]);
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

    if (ac != 2)
        return(msg_error("Error: argument" ,1));
    if (!(file = fopen(av[1], "r")))
        return(msg_error("Error: Operation file corrupted" ,1));
    if (!(check_zone(file, &paper, &draw)))
    {
        fclose(file);
        return(msg_error("Error: Operation file corrupted zone" ,1));
    }
    if (!(check_rect(file, &paper, &draw)))
    {
        fclose(file);
        return(msg_error("Error: Operation file corrupted rect" ,1));
    }
    paint_all(draw, paper);
    fclose(file);
    return (0);
}