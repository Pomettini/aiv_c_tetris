#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define TETRAMINO_OK 0
#define TETRAMINO_DEAD -1

struct tetramino
{
    int y;
};

struct tetris_map
{
    int height;
    int *cell;
};

typedef struct tetramino tetramino_t;
typedef struct tetris_map tetris_map_t;

int tetramino_move_down(struct tetramino *tetramino, struct tetris_map *tetris_map);

void tetramino_init(struct tetramino *tetramino);

void tetris_map_init(struct tetris_map *tetris_map, int height);

void tetramino_draw(tetramino_t *tetramino, SDL_Renderer *renderer, int size);
void tetris_map_draw(tetris_map_t *map, SDL_Renderer *renderer, int size);




