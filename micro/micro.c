#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_zone
{
    int		width;
    int		height;
    char	backgrond;
}	t_zone;

typedef struct s_rect
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}	t_rect;

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
    return (1);
}

int free_all(FILE *file, char *draw)
{
    fclose(file);
    if (draw)
        free(draw);
    return (1);
}

int check_zone(t_zone *zone)
{
    return ((zone->width >= 0 && zone->width <= 300) && (zone->height >= 0 && zone->height <= 300));
}

char *get_zone(FILE *file, t_zone *zone)
{
	char	*draw;
	int		count;
	int		i;

	i = 0;
    if ((count = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->backgrond)) != 3)
        return (0);
    if (count == (-1))
        return (0);
	if (zone->width < 0 || zone->width > 300 || zone->height < 0 || zone->height > 300)
        return (0);
    draw = (char *)malloc(sizeof(char) * (zone->width * zone->height));
    while (i < zone->width * zone->height)
    {
        draw[i] = zone->backgrond;
        i++;
    }
    return (draw);
}

int is_rec(float y, float x, t_rect *rect)
{
    float check = 1.00000000;
    if ((x < rect->x) || (rect->x + rect->width < x) || (y < rect->y) || (rect->y + rect->height < y))
        return (0);
    if (((x - rect->x) < check) || ((rect->x + rect->width) - x < check) || ((y - rect->y) < check) || ((rect->y + rect->height) - y < check))
        return (2);
    return (1);
}

void get_draw(char **draw, t_rect *rect, t_zone *zone)
{
    int x, y;
    int rec;

    y = 0;
    while (y < zone->height)
    {
        x = 0;
        while (x < zone->width)
        {
            rec = is_rec(y, x, rect);
            if ((rect->type == 'r' && rec == 2) || (rect->type == 'R' && rec))
                (*draw)[(y * zone->width) + x] = rect->color;
            x++;
        }
        y++;
    }
}

int drawing(FILE *file, char **draw, t_zone *zone)
{
    t_rect	rect;
    int		count;

    while ((count = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color)) == 6)
    {
		if (!(rect.height > 0.00000000 && rect.width > 0.00000000) && (rect.type == 'r' || rect.type == 'R'))
            return (0);
        get_draw(draw, &rect, zone);
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

int main(int argc, char *argv[])
{
    FILE	*file;
    char	*draw;
    t_zone	zone;

    if (argc != 2)
        return (fail("Error: argument\n"));
    if (!(file = fopen(argv[1], "r")))
        return (fail("Error: Operation file corrupted\n"));
    if (!(draw = get_zone(file, &zone)))
        return (free_all(file, NULL) && fail("Error: Operation file corrupted\n"));
    if (!(drawing(file, &draw, &zone)))
        return (free_all(file, draw) && fail("Error: Operation file corrupted\n"));
    print_draw(draw, &zone);
    free_all(file, draw);
    return (0);
}
