#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "../Settings.h"
#include "../GameObject/Camera/Camera.h"

#define CIRCLE_SEG_COUNT 64

void Renderer_DrawPoint(SDL_Renderer *renderer, Camera *camera, PE_Vec2 point);
void Renderer_DrawLine(SDL_Renderer *renderer, Camera *camera, PE_Vec2 p0, PE_Vec2 p1);

void Renderer_DrawVector(SDL_Renderer *renderer, Camera *camera, PE_Vec2 origin, PE_Vec2 v);

void Renderer_DrawBody(SDL_Renderer *renderer, Camera *camera, PE_Body *body, float alpha);

int Renderer_DrawCircle(
    SDL_Renderer *renderer, Camera *camera, PE_CircleShape *circle, PE_Vec2 transform);

void Renderer_DrawPolygon(
    SDL_Renderer *renderer, Camera *camera, PE_PolygonShape *polygon, PE_Vec2 transform);

void Renderer_DrawShape(
    SDL_Renderer *renderer, Camera *camera, PE_Shape *shape, PE_Vec2 transform);

void Renderer_DrawAABB(
    SDL_Renderer *renderer, Camera *camera, PE_AABB *aabb);

#endif