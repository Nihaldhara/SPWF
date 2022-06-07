#ifndef _STATIC_MAP_H_
#define _STATIC_MAP_H_

#include "../GameBody.h"

void Class_InitStaticMap();

/// @brief Description de la classe "StaticMap".
extern const void *const Class_StaticMap;

typedef struct StaticMapClass_s {
    const GameBodyClass base;
} StaticMapClass;

typedef enum TileType_e
{
    TILE_EMPTY,
    TILE_GROUND,
    TILE_WOOD,
    TILE_ONE_WAY,
    TILE_SPIKE,
    TILE_STEEP_SLOPE_L,
    TILE_STEEP_SLOPE_R,
    TILE_GENTLE_SLOPE_L1,
    TILE_GENTLE_SLOPE_L2,
    TILE_GENTLE_SLOPE_R1,
    TILE_GENTLE_SLOPE_R2,
} TileType;

typedef struct Tile_s
{
    TileType m_type;
    int m_partIdx;
    PE_Collider *m_collider;
} Tile;

typedef struct StaticMap_s
{
    struct {
        GameBody base;
    } m_super;

    RE_AtlasPart *m_woodPart;
    RE_AtlasPart *m_oneWayPart;
    RE_AtlasPart *m_terrainPart;
    RE_AtlasPart *m_spikePart;

    Tile **m_tiles;
    int m_width;
    int m_height;
} StaticMap;

void StaticMap_Constructor(void *self, void *scene, int width, int height);

void StaticMap_SetTile(void *self, int x, int y, int type);
void StaticMap_InitTiles(void *self);

#endif