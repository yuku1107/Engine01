/*==============================================================================

    ƒC[ƒWƒ“ƒOŠÖ”[easing.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2024/01/14
==============================================================================*/
#include "easing.h"
#include "main.h"

float easeInOutSine(float t)
{
    return 0.5f * (1.0f + sinf(XM_PI * (t - 0.5f)));
}

float easeInOutQuad(float t)
{
    return t < 0.5f ? 2.0f * t * t : t * (4.0f - 2.0f * t) - 1.0f;
}

float easeInOutCubic(float t)
{
    return t < 0.5f ? 4.0f * t * t * t : 1.0f + (--t) * (2.0f * (--t)) * (2.0f * t);
}

float easeInOutQuart(float t)
{
    if (t < 0.5f) 
    {
        t *= t;
        return 8.0f * t * t;
    }
    else 
    {
        t = (--t) * t;
        return 1.0f - 8.0f * t * t;
    }
}

float easeInOutQuint(float t)
{
    float t2;
    if (t < 0.5f) 
    {
        t2 = t * t;
        return 16.0f * t * t2 * t2;
    }
    else 
    {
        t2 = (--t) * t;
        return 1.0f + 16.0f * t * t2 * t2;
    }
}

float easeInOutExpo(float t)
{
    if (t < 0.5f) 
    {
        return (powf(2.0f, 16.0f * t) - 1.0f) / 510.0f;
    }
    else 
    {
        return 1.0f - 0.5f * powf(2.0f, -16.0f * (t - 0.5f));
    }
}

float easeInOutCirc(float t)
{
    if (t < 0.5f)
    {
        return (1.0f - (float)sqrt(1.0f - 2.0f * t)) * 0.5f;
    }
    else 
    {
        return (1.0f + (float)sqrt(2.0f * t - 1.0f)) * 0.5f;
    }
}

float easeInOutBack(float t)
{
    if (t < 0.5f)
    {
        return t * t * (7.0f * t - 2.5f) * 2.0f;
    }
    else 
    {
        return 1.0f + (--t) * t * 2.0f * (7.0f * t + 2.5f);
    }
}

float easeInOutElastic(float t)
{
    float t2;
    if (t < 0.45f) 
    {
        t2 = t * t;
        return 8.0f * t2 * t2 * sinf(t * XM_PI * 9.0f);
    }
    else if (t < 0.55f) 
    {
        return 0.5f + 0.75f * sinf(t * XM_PI * 4.0f);
    }
    else 
    {
        t2 = (t - 1.0f) * (t - 1.0f);
        return 1.0f - 8.0f * t2 * t2 * sinf(t * XM_PI * 9.0f);
    }
}

float easeInOutBounce(float t)
{
    if (t < 0.5f) 
    {
        return 8.0f * powf(2.0f, 8.0f * (t - 1.0f)) * abs(sinf(t * XM_PI * 7.0f));
    }
    else 
    {
        return 1.0f - 8.0f * powf(2.0f, -8.0f * t) * abs(sinf(t * XM_PI * 7.0f));
    }
}
