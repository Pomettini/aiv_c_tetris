#include "tetris.h"
#include <stdio.h>

#define CELL tetris_map->cell
#define CELL_COLOR tetris_map->cell_color_id
#define WIDTH tetris_map->width
#define HEIGHT tetris_map->height
#define MAPSIZE tetris_map->width * tetris_map->height
#define ROTATION tetris_map->tetramino_current_rot
#define SHAPE_TYPE tetris_map->tetramino_type

const int tetramino_rotations[] = { 0, 2, 2, 4, 4 };
const int tetramini_positions[5][4][8] =
{
    // 0 - O SHAPE
    { 
        { 
            // X POSITION
            0, 1, 0, 1,
            // Y POSITION
            0, 0, -1, -1
        }
    },
    // 1 - S SHAPE
    { 
        // HORIZONTAL
        { 
            // X POSITION
            0, 1, 1, 2,
            // Y POSITION
            1, 1, 2, 2
        },
        // VERTICAL
        { 
            // X POSITION
            0, 0, 1, 1,
            // Y POSITION
            0, 1, 1, 2
        }
    },
    // 2 - I SHAPE
    { 
        // HORIZONTAL
        { 
            // X POSITION
            0, -1, 1, 2,
            // Y POSITION
            0, 0, 0, 0
        },
        // VERTICAL
        { 
            // X POSITION
            0, 0, 0, 0,
            // Y POSITION
            0, -2, -1, 1
        }
    },
    // 3 - L SHAPE
    { 
        { 
            // X POSITION
            0, -1, 1, 1,
            // Y POSITION
            0, 0, 0, 1
        },
        { 
            // X POSITION
            0, 0, 0, -1,
            // Y POSITION
            0, -1, 1, 1
        },
        { 
            // X POSITION
            0, -1, -1, 1,
            // Y POSITION
            0, 0, -1, 0
        },
        { 
            // X POSITION
            0, 0, 0, 1,
            // Y POSITION
            0, 1, -1, -1
        }
    },
    // 4 - T
    { 
        { 
            // X POSITION
            0, -1, 0, 1,
            // Y POSITION
            0, 0, -1, 0
        },
        { 
            // X POSITION
            0, 0, 0, 1,
            // Y POSITION
            0, -1, 1, 0
        },
        { 
            // X POSITION
            0, -1, 0, 1,
            // Y POSITION
            0, 0, 1, 0
        },
        { 
            // X POSITION
            0, 0, 0, -1,
            // Y POSITION
            0, -1, 1, 0
        }
    }
};

void tetramino_init(TETRAMINO_T, TETRIS_MAP_T)
{
    tetramino->x = tetris_map->width / 2;
    tetramino->y = -1;
}

void tetramino_random_shape_init(TETRAMINI_T, TETRIS_MAP_T)
{
    // 0 - OK
    // 1 - MMH
    // 2 - OK
    // 3 - OK
    // 4 - OK

    int random = rand() % 5;
    int half_screen = tetris_map->width / 2;

    SHAPE_TYPE = random;
    ROTATION = 0;

    for (int i = 0; i < TETRAMINI; i++)
    {
        tetramini[i].x = -1 + tetramini_positions[SHAPE_TYPE][ROTATION][i] + half_screen;
        tetramini[i].y = +2 + tetramini_positions[SHAPE_TYPE][ROTATION][i+4];
        tetramini[i].color_id = SHAPE_TYPE;
    }
}

void tetramino_o_shape_init(TETRAMINI_T, TETRIS_MAP_T)
{
    int half_screen = tetris_map->width / 2;

    tetramini[0].x = half_screen - 1;
    tetramini[0].y = 0;
    tetramini[0].color_id = 0;

    tetramini[1].x = half_screen;
    tetramini[1].y = 0;
    tetramini[1].color_id = 0;

    tetramini[2].x = half_screen - 1;
    tetramini[2].y = 1;
    tetramini[2].color_id = 0;

    tetramini[3].x = half_screen;
    tetramini[3].y = 1;
    tetramini[3].color_id = 0;

    tetris_map->tetramino_type = 0;
}

int tetramino_group_rotate(TETRAMINI_T, TETRIS_MAP_T)
{
    if (ROTATION >= tetramino_rotations[SHAPE_TYPE] - 1)
        ROTATION = 0;
    else
        ROTATION++;

    int half_screen = tetris_map->width / 2;

    // SDL_Log("%d, %d, %d", SHAPE_TYPE, ROTATION, tetramino_rotations[SHAPE_TYPE]);

    int previous_x = tetramini[0].x;
    int previous_y = tetramini[0].y;

    for (int i = 0; i < 4; i++)
    {
        tetramini[i].x = previous_x + (int)tetramini_positions[SHAPE_TYPE][ROTATION][i];
        tetramini[i].y = previous_y + (int)tetramini_positions[SHAPE_TYPE][ROTATION][i + 4];
    }
}

int tetramino_group_init(TETRAMINI_T, TETRIS_MAP_T)
{
    
}

int tetramino_group_move_down(TETRAMINI_T, TETRIS_MAP_T)
{
    for (int i = 0; i < TETRAMINI; i++)
    {
        if (tetramino_move_down_check(&tetramini[i], tetris_map) == TETRAMINO_DEAD)
        {
            tetramini_to_map(tetramini, tetris_map);
            return TETRAMINO_DEAD;
        }
    }

    for (int i = 0; i < TETRAMINI; i++)
        tetramino_move_down_act(&tetramini[i], tetris_map);

    return TETRAMINO_OK;
}

int tetramino_group_move_right(TETRAMINI_T, TETRIS_MAP_T)
{
    int current_index;

    for (int i = 0; i < TETRAMINI; i++)
    {
        if (tetramino_move_right_check(&tetramini[i], tetris_map) == TETRAMINO_DEAD)
            return TETRAMINO_DEAD;

        current_index = tetris_map->width * tetramini[i].y + tetramini[i].x;

        if (CELL[current_index + 1] == 1)
            return TETRAMINO_OK;
    }

    for (int i = 0; i < TETRAMINI; i++)
        tetramino_move_right_act(&tetramini[i], tetris_map);

    return TETRAMINO_OK;
}

int tetramino_group_move_left(TETRAMINI_T, TETRIS_MAP_T)
{
    int current_index;

    for (int i = 0; i < TETRAMINI; i++)
    {
        if (tetramino_move_left_check(&tetramini[i], tetris_map) == TETRAMINO_DEAD)
            return TETRAMINO_DEAD;

        current_index = tetris_map->width * tetramini[i].y + tetramini[i].x;

        if (CELL[current_index - 1] == 1)
            return TETRAMINO_OK;
    }

    for (int i = 0; i < TETRAMINI; i++)
        tetramino_move_left_act(&tetramini[i], tetris_map);

    return TETRAMINO_OK;
}

int tetramino_move_down(TETRAMINO_T, TETRIS_MAP_T)
{
    int current_index = tetris_map->width * tetramino->y + tetramino->x;
    int next_index = tetris_map->width * (tetramino->y + 1) + tetramino->x;

    if (tetramino->y + 1 >= tetris_map->height)
    {
        CELL[current_index] = 1;
        return TETRAMINO_DEAD;
    }

    if (CELL[next_index] == 0)
    {
        tetramino->y += 1;
    }
    else
    {
        CELL[current_index] = 1;
        return TETRAMINO_DEAD;
    }

    tetris_row_check_fill(tetris_map);

    return TETRAMINO_OK;
}

int tetramino_move_down_check(TETRAMINO_T, TETRIS_MAP_T)
{
    int current_index = tetris_map->width * tetramino->y + tetramino->x;
    int next_index = tetris_map->width * (tetramino->y + 1) + tetramino->x;

    if (tetramino->y + 1 >= tetris_map->height)
    {
        CELL[current_index] = 1;
        return TETRAMINO_DEAD;
    }

    if (CELL[next_index] == 1)
        return TETRAMINO_DEAD;

    return TETRAMINO_OK;
}

int tetramino_move_down_act(TETRAMINO_T, TETRIS_MAP_T)
{
    int current_index = tetris_map->width * tetramino->y + tetramino->x;
    int next_index = tetris_map->width * (tetramino->y + 1) + tetramino->x;

    if (CELL[next_index] == 0)
    {
        tetramino->y += 1;
    }
    else
    {
        CELL[current_index] = 1;
        return TETRAMINO_DEAD;
    }

    tetris_row_check_fill(tetris_map);

    return TETRAMINO_OK;
}

int tetramini_to_map(TETRAMINI_T, TETRIS_MAP_T)
{
    int current_index;
    int next_index;

    for (int i = 0; i < TETRAMINI; i++)
    {
        current_index = tetris_map->width * tetramini[i].y + tetramini[i].x;
        next_index = tetris_map->width * (tetramini[i].y + 1) + tetramini[i].x;
        CELL[current_index] = 1;
        CELL_COLOR[current_index] = tetramini[i].color_id;
    }

    return TETRAMINO_DEAD;
}

int tetramino_move_right(TETRAMINO_T, TETRIS_MAP_T)
{
    if (tetramino->x >= tetris_map->width - 1)
    {
        return TETRAMINO_OK;
    }

    int right_index = tetris_map->width * tetramino->y + (tetramino->x + 1);
    if (!CELL[right_index])
    {
        tetramino->x++;
    }

    return TETRAMINO_OK;
}

int tetramino_move_right_check(TETRAMINO_T, TETRIS_MAP_T)
{
    if (tetramino->x >= tetris_map->width - 1)
        return TETRAMINO_DEAD;

    return TETRAMINO_OK;
}

int tetramino_move_right_act(TETRAMINO_T, TETRIS_MAP_T)
{
    tetramino->x++;
    return TETRAMINO_OK;
}

int tetramino_move_left(TETRAMINO_T, TETRIS_MAP_T)
{
    if (tetramino->x <= 0)
    {
        return TETRAMINO_OK;
    }

    tetramino->x--;

    return TETRAMINO_OK;
}

int tetramino_move_left_check(TETRAMINO_T, TETRIS_MAP_T)
{
    if (tetramino->x <= 0)
        return TETRAMINO_DEAD;

    return TETRAMINO_OK;
}

int tetramino_move_left_act(TETRAMINO_T, TETRIS_MAP_T)
{
    tetramino->x--;
    return TETRAMINO_OK;
}

void tetris_map_init(TETRIS_MAP_T, int width, int height)
{
    int size = sizeof(int) * width * height;

    CELL = malloc(size);
    memset(CELL, 0, size);

    CELL_COLOR = malloc(size);
    memset(CELL_COLOR, 0, size);

    tetris_map->width = width;
    tetris_map->height = height;
}

void tetris_row_check_fill(TETRIS_MAP_T)
{
    int row;
    int column;

    // For each row of the Tetris map
    for (row = HEIGHT; row >= 0; row--)
    {
        int tetramini = 0;
        for (column = 0; column < tetris_map->width; column++)
        {
            // We count how many tetramini are in the row
            if (CELL[(row * tetris_map->width) + column] == 1)
                tetramini += 1;
        }

        if (tetramini >= tetris_map->width)
            tetris_row_destroy(tetris_map, row);
    }
}

void tetris_row_destroy(TETRIS_MAP_T, int row)
{
    // Move down all the cells
    memmove
    (
        CELL + WIDTH, 
        CELL, 
        sizeof(int) * (WIDTH * row)
    );
    memset(CELL + WIDTH, 0, sizeof(int) * WIDTH);
    // Move down all the color cells
    memmove
    (
        CELL_COLOR + WIDTH, 
        CELL_COLOR, 
        sizeof(int) * (WIDTH * row)
    );
    memset(CELL_COLOR + WIDTH, 0, sizeof(int) * WIDTH);
}
