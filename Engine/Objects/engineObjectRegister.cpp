/*==============================================================================

    ÉGÉìÉWÉìÉIÉuÉWÉFÉNÉgê∂ê¨ìoò^[engineObjectRegister.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/02/25
==============================================================================*/
#include "main.h"
#include "engineObjectRegister.h"

#include "Engine/Core/ObjectFactory.h"

#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Objects/SceneObjects/light.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/SceneObjects/coordinate.h"
#include "Engine/Objects/SceneObjects/coordinate2D.h"
#include "Engine/Objects/SceneObjects/cameraUI.h"

void RegisterEngineObjects()
{
    ObjectFactory::Register("ScenePolygon2D", []() { return new ScenePolygon2D(); }, 0);
    ObjectFactory::Register("Light", []() { return new Light(); }, 3);
    ObjectFactory::Register("Camera", []() { return new Camera(); }, 4);
    ObjectFactory::Register("Coordinate", []() { return new Coordinate(); }, 3);
    ObjectFactory::Register("Coordinate2D", []() { return new Coordinate2D(); }, 1);
    ObjectFactory::Register("CameraUI", []() { return new CameraUI(); }, 2);
}
