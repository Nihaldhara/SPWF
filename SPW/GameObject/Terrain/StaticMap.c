#include "StaticMap.h"
#include "../../Scene/LevelScene.h"
#include "../../Utils/Renderer.h"

// Object virtual methods
void StaticMap_VM_Destructor(void *self);

// GameObject virtual methods
void StaticMap_VM_Render(void *self);
void StaticMap_VM_Start(void *self);

static StaticMapClass _Class_StaticMap = { 0 };
const void *const Class_StaticMap = &_Class_StaticMap;

void Class_InitStaticMap()
{
    if (!Class_IsInitialized(Class_StaticMap))
    {
        Class_InitGameBody();

        void *self = (void *)Class_StaticMap;
        ClassCtorParams params = {
            .self = self,
            .super = Class_GameBody,
            .name = "StaticMap",
            .instanceSize = sizeof(StaticMap),
            .classSize = sizeof(StaticMapClass)
        };
        Class_Constructor(params, StaticMap_VM_Destructor);
        ((GameObjectClass *)self)->Render = StaticMap_VM_Render;
        ((GameObjectClass *)self)->Start = StaticMap_VM_Start;
    }
}

void StaticMap_Constructor(void *self, void *scene, int width, int height)
{
    GameBody_Constructor(self, scene, LAYER_TERRAIN);
    Object_SetClass(self, Class_StaticMap);

    StaticMap *map = Object_Cast(self, Class_StaticMap);
    map->m_width = width;
    map->m_height = height;

    map->m_tiles = (Tile **)calloc(width, sizeof(Tile *));
    AssertNew(map->m_tiles);

    for (int x = 0; x < width; x++)
    {
        map->m_tiles[x] = (Tile *)calloc(height, sizeof(Tile));
        AssertNew(map->m_tiles[x]);
    }

    AssetManager *assets = Scene_GetAssetManager(scene);
    RE_Atlas *atlas = AssetManager_GetTerrainAtlas(assets);

    map->m_woodPart = RE_Atlas_GetPart(atlas, "Wood");
    AssertNew(map->m_woodPart);

    map->m_oneWayPart = RE_Atlas_GetPart(atlas, "OneWay");
    AssertNew(map->m_oneWayPart);

    map->m_terrainPart = RE_Atlas_GetPart(atlas, "Terrain");
    AssertNew(map->m_terrainPart);

    map->m_spikePart = RE_Atlas_GetPart(atlas, "Spike");
    AssertNew(map->m_spikePart);

    SDL_Color color = { .r = 255, .g = 200, .b = 0, .a = 255 };
    GameBody_SetDebugColor(self, color);
}

void StaticMap_VM_Destructor(void *self)
{
    StaticMap *map = Object_Cast(self, Class_StaticMap);
    int width = map->m_width;

    for (int x = 0; x < width; x++)
    {
        if (map->m_tiles)
        {
            free(map->m_tiles[x]);
        }
    }
    free(map->m_tiles);

    // Destructeur de la classe mère
    Object_SuperDestroy(self, Class_StaticMap);
}

void Spike_OnCollisionStay(PE_Collision *collision)
{
    PE_Body *otherBody = PE_Collision_GetOtherBody(collision);
    GameBody *otherGameBody = GameBody_GetFromBody(otherBody);

    if (Object_IsA(otherGameBody, Class_Player))
    {
        Player *player = Object_Cast(otherGameBody, Class_Player);
        Player_Bounce(player);
        Player_Damage(player);
    }
}

void StaticMap_SetTile(void *self, int x, int y, int type)
{
    StaticMap *map = Object_Cast(self, Class_StaticMap);

    assert((0 <= x) && (x < map->m_width));
    assert((0 <= y) && (y < map->m_height));

    Tile *tile = &(map->m_tiles[x][y]);
    tile->m_partIdx = 0;
    tile->m_type = type;
}

void StaticMap_VM_Start(void *self)
{
    StaticMap *map = Object_Cast(self, Class_StaticMap);
    Scene *scene = GameObject_GetScene((GameObject *)map);

    PE_World *world = Scene_GetWorld(scene);
    PE_BodyDef bodyDef = { 0 };
    PE_Body *body = NULL;

    // Crée le corps
    PE_BodyDef_SetDefault(&bodyDef);
    bodyDef.type = PE_STATIC_BODY;
    bodyDef.position = PE_Vec2_Zero;
    bodyDef.name = "StaticMap";

    body = PE_World_CreateBody(world, &bodyDef);
    AssertNew(body);

    GameBody_SetBody(self, body);

    // Crée le collider
    Tile **tiles = map->m_tiles;
    int width = map->m_width;
    int height = map->m_height;
    PE_Vec2 vertices[3] = { 0 };

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            Tile *tile = &(tiles[x][y]);
            if (tile->m_type == TILE_EMPTY)
            {
                continue;
            }

            bool newCollider = true;

            PE_ColliderDef colliderDef = { 0 };
            PE_ColliderDef_SetDefault(&colliderDef);
            colliderDef.friction = 0.5f;
            colliderDef.filter.categoryBits = FILTER_TERRAIN;
            colliderDef.filter.maskBits = ~0;

            switch (tile->m_type)
            {
            case TILE_ONE_WAY:
                colliderDef.isOneWay = true;
                PE_Shape_SetAsBox(
                    &colliderDef.shape,
                    (float)x, (float)y + 0.5f, (float)x + 1.0f, (float)y + 1.0f);
                break;

            case TILE_GROUND:
            case TILE_WOOD:
                PE_Shape_SetAsBox(
                    &colliderDef.shape,
                    (float)x, (float)y, (float)x + 1.0f, (float)y + 1.0f);
                break;

            case TILE_SPIKE:
                vertices[0] = PE_Vec2_Set((float)x + 0.1f, (float)y);
                vertices[1] = PE_Vec2_Set((float)x + 0.9f, (float)y);
                vertices[2] = PE_Vec2_Set((float)x + 0.5f, (float)y + 0.8f);

                PE_Shape_SetAsPolygon(
                    &colliderDef.shape, vertices, 3);
                break;

            default:
                newCollider = false;
                break;
            }

            if (newCollider)
            {
                tile->m_collider = PE_Body_CreateCollider(body, &colliderDef);
                AssertNew(tile->m_collider);

                if (tile->m_type == TILE_SPIKE)
                {
                    PE_Collider_SetOnCollisionStay(tile->m_collider, Spike_OnCollisionStay);
                }
            }
            else
            {
                tile->m_collider = NULL;
            }
        }
    }

    // Limite à gauche du monde
    PE_ColliderDef colliderDef = { 0 };
    PE_ColliderDef_SetDefault(&colliderDef);
    colliderDef.friction = 0.0f;
    colliderDef.filter.categoryBits = FILTER_TERRAIN;
    colliderDef.filter.maskBits = ~0;
    PE_Shape_SetAsBox(
        &colliderDef.shape,
        -1.0f, -2.0f, 0.0f, (float)height + 10.0f
    );

    PE_Body_CreateCollider(body, &colliderDef);

    // Limite à droite du monde
    PE_ColliderDef_SetDefault(&colliderDef);
    colliderDef.friction = 0.0f;
    colliderDef.filter.categoryBits = FILTER_TERRAIN;
    colliderDef.filter.maskBits = ~0;
    PE_Shape_SetAsBox(
        &colliderDef.shape,
        (float)width, -2.0f, (float)width + 1.0f, (float)height + 10.0f
    );

    PE_Body_CreateCollider(body, &colliderDef);
}

int StaticMap_GetTileType(StaticMap *map, int x, int y)
{
    int width = map->m_width;
    int height = map->m_height;
    Tile **tiles = map->m_tiles;

    if ((x < 0) || (x >= width) || (y < 0))
        return TILE_GROUND;
    else if (y >= height)
        return TILE_EMPTY;
    else
        return tiles[x][y].m_type;
}


bool StaticMap_IsGround(StaticMap *map, int x, int y)
{
    switch (StaticMap_GetTileType(map, x, y))
    {
    case TILE_GROUND:
    case TILE_GENTLE_SLOPE_L1:
    case TILE_GENTLE_SLOPE_L2:
    case TILE_GENTLE_SLOPE_R1:
    case TILE_GENTLE_SLOPE_R2:
    case TILE_STEEP_SLOPE_L:
    case TILE_STEEP_SLOPE_R:
        return true;

    default:
        return false;
    }
}

void StaticMap_InitTiles(void *self)
{
    StaticMap *map = Object_Cast(self, Class_StaticMap);
    Tile **tiles = map->m_tiles;
    int width = map->m_width;
    int height = map->m_height;

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (StaticMap_GetTileType(map, x, y) == TILE_GROUND)
            {
                if (StaticMap_IsGround(map, x, y + 1) == false)
                {
                    tiles[x][y].m_partIdx = 1;
                }
                else
                {
                    tiles[x][y].m_partIdx = 4;
                }
            }
        }
    }
}

void StaticMap_VM_Render(void *self)
{
    StaticMap *map = Object_Cast(self, Class_StaticMap);
    Scene *scene = GameObject_GetScene(self);
    SDL_Renderer *renderer = scene->m_renderer;
    Camera *camera = Scene_GetActiveCamera(scene);

    Tile **tiles = map->m_tiles;
    int width = map->m_width;
    int height = map->m_height;

    PE_AABB view = Camera_GetWorldView(camera);
    int x0 = (int)view.lower.x - 1;
    int y0 = (int)view.lower.y - 1;
    int x1 = (int)view.upper.x + 2;
    int y1 = (int)view.upper.y + 2;
    
    x0 = Int_Max(x0, 0);
    y0 = Int_Max(y0, 0);
    x1 = Int_Min(x1, width);
    y1 = Int_Min(y1, height);

    for (int x = x0; x < x1; ++x)
    {
        for (int y = y0; y < y1; ++y)
        {
            PE_Collider *collider = tiles[x][y].m_collider;

            PE_Vec2 position = PE_Vec2_Set((float)x, (float)y);
            SDL_FRect dst = { 0 };

            Camera_WorldToView(camera, position, &(dst.x), &(dst.y));
            float scale = Camera_GetWorldToViewScale(camera);
            dst.w = scale * 1.0f;
            dst.h = scale * 1.0f;

            switch ((map->m_tiles[x][y]).m_type)
            {
            case TILE_GROUND:
            case TILE_STEEP_SLOPE_L:
            case TILE_STEEP_SLOPE_R:
            case TILE_GENTLE_SLOPE_L1:
            case TILE_GENTLE_SLOPE_L2:
            case TILE_GENTLE_SLOPE_R1:
            case TILE_GENTLE_SLOPE_R2:
            {
                int idx = (map->m_tiles[x][y]).m_partIdx;

                RE_AtlasPart_RenderCopyF(
                    map->m_terrainPart, idx, renderer, &dst, RE_ANCHOR_LEFT | RE_ANCHOR_BOTTOM
                );
            }   break;

            case TILE_WOOD:
                RE_AtlasPart_RenderCopyF(
                    map->m_woodPart, 0, renderer, &dst, RE_ANCHOR_LEFT | RE_ANCHOR_BOTTOM
                );
                break;

            case TILE_ONE_WAY:
                RE_AtlasPart_RenderCopyF(
                    map->m_oneWayPart, 0, renderer, &dst, RE_ANCHOR_LEFT | RE_ANCHOR_BOTTOM
                );
                break;

            case TILE_SPIKE:
                RE_AtlasPart_RenderCopyF(
                    map->m_spikePart, 0, renderer, &dst, RE_ANCHOR_LEFT | RE_ANCHOR_BOTTOM
                );
                break;

            default:
            {
                if (!collider)
                {
                    continue;
                }
                if (PE_Collider_IsOneWay(collider))
                {
                    SDL_SetRenderDrawColor(renderer, 255, 160, 200, 255);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 255, 200, 120, 255);
                }

                PE_Shape *shape = PE_Collider_GetShape(collider);
                Renderer_DrawShape(renderer, camera, shape, PE_Vec2_Zero);
            }   break;
            }

        }
    }
}
