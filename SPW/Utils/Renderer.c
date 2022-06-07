#include "Renderer.h"

void Renderer_DrawVector(SDL_Renderer *renderer, Camera *camera, PE_Vec2 origin, PE_Vec2 v)
{
    float x0, x1, y0, y1;

    v = PE_Vec2_Add(v, origin);

    Camera_WorldToView(camera, origin, &x0, &y0);
    Camera_WorldToView(camera, v, &x1, &y1);

    SDL_RenderDrawLine(
        renderer, (int)roundf(x0), (int)roundf(y0),
        (int)roundf(x1), (int)roundf(y1));
}

void Renderer_DrawPoint(SDL_Renderer *renderer, Camera *camera, PE_Vec2 point)
{
    float x, y;
    int w = 3;
    Camera_WorldToView(camera, point, &x, &y);

    SDL_RenderDrawLine(
        renderer, (int)roundf(x) - w, (int)roundf(y) - w,
        (int)roundf(x) + w, (int)roundf(y) + w);
    SDL_RenderDrawLine(
        renderer, (int)roundf(x) + w, (int)roundf(y) - w,
        (int)roundf(x) - w, (int)roundf(y) + w);
}

void Renderer_DrawLine(SDL_Renderer *renderer, Camera *camera, PE_Vec2 p0, PE_Vec2 p1)
{
    float x0, y0;
    float x1, y1;
    Camera_WorldToView(camera, p0, &x0, &y0);
    Camera_WorldToView(camera, p1, &x1, &y1);

    SDL_RenderDrawLine(
        renderer, (int)roundf(x0), (int)roundf(y0),
        (int)roundf(x1), (int)roundf(y1));
}


void Renderer_DrawBody(SDL_Renderer *renderer, Camera *camera, PE_Body *body, float alpha)
{
    PE_Vec2 transform = PE_Body_GetInterpolation(body, alpha);

    Renderer_DrawPoint(renderer, camera, transform);

    if (body->m_parent)
    {
        PE_Vec2 parentTransform = PE_Body_GetInterpolation(body->m_parent, alpha);
        Renderer_DrawLine(renderer, camera, transform, parentTransform);
    }

    if (PE_Body_IsEnabled(body))
    {
        PE_ColliderIterator it;
        PE_Body_GetColliderIterator(body, &it);
        while (PE_ColliderIterator_MoveNext(&it))
        {
            PE_Collider *collider = PE_ColliderIterator_Current(&it);
            PE_Shape *shape = PE_Collider_GetShape(collider);
            Renderer_DrawShape(renderer, camera, shape, transform);
        }
    }
}

int Renderer_DrawCircle(
    SDL_Renderer *renderer, Camera *camera,
    PE_CircleShape *circle, PE_Vec2 transform)
{
    PE_Vec2 center = PE_Vec2_Add(circle->m_center, transform);
    PE_Vec2 right = PE_Vec2_Add(center, PE_Vec2_Set(circle->m_radius, 0.0f));

    float x, y, xRight, yRight;
    Camera_WorldToView(camera, center, &x, &y);
    Camera_WorldToView(camera, right, &xRight, &yRight);
    
    int xCenter = (int)roundf(x);
    int yCenter = (int)roundf(y);
    int radius  = (int)roundf(xRight - x);

    int offsetx = 0;
    int offsety = radius;
    int d = radius - 1;
    int status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, xCenter + offsetx, yCenter + offsety);
        status += SDL_RenderDrawPoint(renderer, xCenter + offsety, yCenter + offsetx);
        status += SDL_RenderDrawPoint(renderer, xCenter - offsetx, yCenter + offsety);
        status += SDL_RenderDrawPoint(renderer, xCenter - offsety, yCenter + offsetx);
        status += SDL_RenderDrawPoint(renderer, xCenter + offsetx, yCenter - offsety);
        status += SDL_RenderDrawPoint(renderer, xCenter + offsety, yCenter - offsetx);
        status += SDL_RenderDrawPoint(renderer, xCenter - offsetx, yCenter - offsety);
        status += SDL_RenderDrawPoint(renderer, xCenter - offsety, yCenter - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void Renderer_DrawPolygon(
    SDL_Renderer *renderer, Camera *camera,
    PE_PolygonShape *polygon, PE_Vec2 transform)
{
    int count = polygon->m_vertexCount;
    float xVertices[PE_MAX_POLYGON_VERTICES] = { 0 };
    float yVertices[PE_MAX_POLYGON_VERTICES] = { 0 };

    assert(2 <= count && count < PE_MAX_POLYGON_VERTICES);

    for (int i = 0; i < count; i++)
    {
        PE_Vec2 vertex = PE_Vec2_Add(polygon->m_vertices[i], transform);
        Camera_WorldToView(camera, vertex, xVertices + i, yVertices + i);
    }
    for (int i = 0; i < count - 1; i++)
    {
        SDL_RenderDrawLine(
            renderer, (int)roundf(xVertices[i]), (int)roundf(yVertices[i]),
            (int)roundf(xVertices[i + 1]), (int)roundf(yVertices[i + 1]));
    }
    SDL_RenderDrawLine(
        renderer, (int)roundf(xVertices[0]), (int)roundf(yVertices[0]),
        (int)roundf(xVertices[count - 1]), (int)roundf(yVertices[count - 1]));
}

void Renderer_DrawCapsule(
    SDL_Renderer *renderer, Camera *camera,
    PE_CapsuleShape *capsule, PE_Vec2 transform)
{
    PE_Vec2 vertexA = PE_Vec2_Add(capsule->m_vertices[0], transform);
    PE_Vec2 vertexB = PE_Vec2_Add(capsule->m_vertices[1], transform);
    PE_Vec2 skewA;
    PE_Vec2 right = { .x = capsule->m_radius, .y = 0.0f };

    right = PE_Vec2_Add(right, vertexA);
    skewA = PE_Vec2_Sub(vertexB, vertexA);
    skewA = PE_Vec2_Scale(skewA, capsule->m_radius / PE_Vec2_Length(skewA));
    skewA = PE_Vec2_Add(PE_Vec2_Perp(skewA), vertexA);

    float xA, yA, xB, yB, x0, y0, x1, y1, xS, yS;
    Camera_WorldToView(camera, vertexA, &xA, &yA);
    Camera_WorldToView(camera, vertexB, &xB, &yB);
    Camera_WorldToView(camera, right, &x0, &y0);
    Camera_WorldToView(camera, skewA, &xS, &yS);

    float radius = x0 - xA;
    float inc = 2.0f * 3.14159265f / (float)CIRCLE_SEG_COUNT;
    float theta = inc;

    x0 -= xA;
    y0 -= yA;
    for (int i = 0; i < CIRCLE_SEG_COUNT; ++i, theta += inc)
    {
        x1 = radius * cosf(theta);
        y1 = radius * sinf(theta);

        SDL_RenderDrawLine(
            renderer,
            (int)roundf(xA + x0), (int)roundf(yA + y0),
            (int)roundf(xA + x1), (int)roundf(yA + y1));
        SDL_RenderDrawLine(
            renderer,
            (int)(xB + x0), (int)(yB + y0),
            (int)(xB + x1), (int)(yB + y1));

        x0 = x1;
        y0 = y1;
    }

    xS -= xA;
    yS -= yA;
    SDL_RenderDrawLine(
        renderer,
        (int)roundf(xA + xS), (int)roundf(yA + yS),
        (int)roundf(xB + xS), (int)roundf(yB + yS));
    SDL_RenderDrawLine(
        renderer,
        (int)roundf(xA - xS), (int)roundf(yA - yS),
        (int)roundf(xB - xS), (int)roundf(yB - yS));
}


void Renderer_DrawShape(
    SDL_Renderer *renderer, Camera *camera,
    PE_Shape *shape, PE_Vec2 transform)
{
    switch (shape->m_type)
    {
    case PE_SHAPE_POLYGON:
    case PE_SHAPE_POLYGON_BOX:
        Renderer_DrawPolygon(renderer, camera, &shape->m_data.polygon, transform);
        break;

    case PE_SHAPE_CIRCLE:
        Renderer_DrawCircle(renderer, camera, &shape->m_data.circle, transform);
        break;

    case PE_SHAPE_CAPSULE:
        Renderer_DrawCapsule(renderer, camera, &shape->m_data.capsule, transform);
        break;
    default:
        break;
    }
}

void Renderer_DrawAABB(SDL_Renderer *renderer, Camera *camera, PE_AABB *aabb)
{
    float x0, x1, y0, y1;

    Camera_WorldToView(camera, aabb->lower, &x0, &y0);
    Camera_WorldToView(camera, aabb->upper, &x1, &y1);

    SDL_FRect rect;
    rect.x = x0;
    rect.y = y1;
    rect.w = fabsf(x1 - x0);
    rect.h = fabsf(y1 - y0);

    SDL_RenderDrawRectF(renderer, &rect);
}
