#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
    int width;
    int height;
    char background;
}	t_zone;

typedef struct s_circle
{
    char type;
    float x;
    float y;
    float radius;
    char color;
} 	t_circle;

int ft_strlen(char *s)
{
    int i;

	i = 0;
    if (!s)
        return (i);
    while (s[i])
        i++;
    return (i);
}

int fail(char *s)
{
    write(1, s, ft_strlen(s));
    return(1);
}

int free_all(FILE *file, char *s)
{
    fclose(file);
    if (s)
        free(s);
    return (1);
}

char *get_zone(FILE *file, t_zone *zone)
{
    int i;
    char *array;

    if ((i = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
        return (NULL);
    if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
        return (NULL);
    if (!(array = (char *)malloc(sizeof(char) * (zone->width * zone->height))))
        return (NULL);
    i = 0;
    while (i < (zone->width * zone->height))
    {
        array[i] = zone->background;
        i++;
    }
    return (array);
}

int is_rad(float x, float y, t_circle *circle)
{
    float dist;

    dist = sqrtf(((x - circle->x) * (x - circle->x)) + ((y - circle->y) * (y - circle->y)));

    if (dist <= circle->radius)
    {
        if ((circle->radius - dist) < 1.00000000)
            return (2);
        return (1);
    }
    return (0);
}

void get_draw(t_circle *circle, t_zone *zone, char *draw)
{
    int x, y, rad;

    y = 0;
    while (y < zone->height)
    {
        x = 0;
        while (x < zone->width)
        {
            rad = is_rad((float)x, (float)y, circle);
            if ((rad == 2 && circle->type == 'c') || (rad && circle->type == 'C'))
                draw[(y * zone->width) + x] = circle->color;
            x++;
        }
        y++;
    }
}

int drawing(FILE *file, t_zone *zone, char *draw)
{
    t_circle circle;
    int count;

    while ((count = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color)) == 5)
    {
        if (circle.radius <= 0.00000000 && (circle.type != 'c' || circle.type != 'C'))
            return (0);
        get_draw(&circle, zone, draw);
    }
    if (count != (-1))
        return (0);
    return (1);
}

void print_draw(char *draw, t_zone *zone)
{
    int i = 0;

    while (i < zone->height)
    {
        write(1, draw + (i * zone->width), zone->width);
        write(1, "\n", 1);
        i++;
    }
}

int main(int ac, char **av)
{
	FILE	*file;
	char	*draw;
    t_zone	zone;

    if (ac != 2)
        return (fail("Error: argument\n"));
    if (!(file = fopen(av[1], "r")))
        return (fail("Error: Operation file corrupted\n"));
    if (!(draw = get_zone(file, &zone)))
        return (free_all(file, NULL) && fail("Error: Operation file corrupted\n"));
    if (!(drawing(file, &zone, draw)))
        return (free_all(file, draw) && fail("Error: Operation file corrupted\n"));
    print_draw(draw, &zone);
    free_all(file, draw);
    return (0);
}