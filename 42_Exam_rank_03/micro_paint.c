#include <stdio.h>
#include <string.h>

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

static int check_zone(FILE *file)
{
    int     get;
    t_bg    zone;

    get = 0;
    memset(&zone, 0, sizeof(t_bg));
    if ((get = fscanf(file, "%d %d %c\n", &zone.width, &zone.height, &zone.c)) != 3)
        return (0);
    if (!(zone.width > 0 && zone.width <= 300) || !(zone.height > 0 && zone.height <= 300))
        return (0);
    if (get == -1)
        return (0);
    return (1);
}

static int check_draw(FILE *file)
{
    int     get;
    t_draw  draw;

    get = 0;
    memset(&draw, 0, sizeof(t_draw));
    while (get = fscanf(file, "%c %f %f %f %f %c\n", &draw.c, &draw.x, &draw.y, &draw.width, &draw.height, &draw.cc)) == 6)
    {
       
    }
}

int main(int ac, char **av)
{
    FILE *file;

    if (ac != 2)
        return(msg_error("Error: argument" ,1));
    if (!(file = fopen(av[1], "r")))
        return(msg_error("Error: Operation file corrupted" ,1));
    if (!(check_zone(file)))
        return(msg_error("Error: Operation file corrupted" ,1));
    fclose(file);
    return (0);
}