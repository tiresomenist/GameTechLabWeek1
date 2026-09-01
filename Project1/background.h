#pragma once
#include "FVertexSimple.h"

FVertexSimple bgQuad[] = {
    { -1.0f,  1.0f, 0.0f,   1,1,1,1,   0.0f, 0.0f },  // 좌상
    {  1.0f,  1.0f, 0.0f,   1,1,1,1,   1.0f, 0.0f },  // 우상
    { -1.0f, -1.0f, 0.0f,   1,1,1,1,   0.0f, 1.0f },  // 좌하
    {  1.0f, -1.0f, 0.0f,   1,1,1,1,   1.0f, 1.0f },  // 우하
};