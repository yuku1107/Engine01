/*==============================================================================

    ÉQÅ[ÉÄÉIÉuÉWÉFÉNÉgê∂ê¨ìoò^[gameObjectRegister.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/02/25
==============================================================================*/
#include "main.h"
#include "Engine/Core/ObjectFactory.h"
#include "gameObjectRegister.h"
#include "meshField.h"
#include "sky.h"
#include "wave.h"
#include "player.h"
#include "grass.h"
#include "tree.h"
#include "tree3D.h"
#include "anchor.h"
#include "targetMark.h"
#include "ground.h"
#include "groundMove.h"
#include "transferDevice.h"
#include "goal.h"
#include "enemy.h"
#include "tunnel.h"
#include "discoveryArea.h"
#include "discoveryWindow.h"
#include "Engine/Objects/UIObjects/ui.h"
#include "Engine/Objects/UIObjects/button.h"
#include "bulletCount.h"

void RegisterGameObjects()
{
    ObjectFactory::Register("MeshField", []() { return new MeshField(); }, 4);
    ObjectFactory::Register("Player", []() { return new Player(); }, 5);
    ObjectFactory::Register("Tree", []() { return new Tree(); }, 6);
    ObjectFactory::Register("Tree3D", []() { return new Tree3D(); }, 5);
    ObjectFactory::Register("Sky", []() { return new Sky(); }, 4);
    ObjectFactory::Register("Wave", []() { return new Wave(); }, 6);
    ObjectFactory::Register("Grass", []() { return new Grass(); }, 6);
    ObjectFactory::Register("Anchor", []() { return new Anchor(); }, 5);
    ObjectFactory::Register("TargetMark", []() { return new TargetMark(); }, 4);
    ObjectFactory::Register("Ground", []() { return new Ground(); }, 4);
    ObjectFactory::Register("GroundMove", []() { return new GroundMove(); }, 4);
    ObjectFactory::Register("TransferDevice", []() { return new TransferDevice(); }, 5);
    ObjectFactory::Register("Goal", []() { return new Goal(); }, 5);
    ObjectFactory::Register("Enemy", []() { return new Enemy(); }, 5);
    ObjectFactory::Register("Tunnel", []() { return new Tunnel(); }, 5);
    ObjectFactory::Register("DiscoveryArea", []() { return new DiscoveryArea(); }, 4);
    ObjectFactory::Register("DiscoveryWindow", []() { return new DiscoveryWindow(); }, 1);
    ObjectFactory::Register("BulletCountUI", []() { return new BulletCountUI(); }, 1);

    ObjectFactory::Register("TitleUI", []() { return new TitleUI(); }, 1);
    ObjectFactory::Register("StartButton", []() { return new StartButton(); }, 1);
    ObjectFactory::Register("ResultUI", []() { return new ResultUI(); }, 1);
    ObjectFactory::Register("EndButton", []() { return new EndButton(); }, 1);
    ObjectFactory::Register("ClearUI", []() { return new ClearUI(); }, 1);
    ObjectFactory::Register("ContinueButton", []() { return new ContinueButton(); }, 1);
    ObjectFactory::Register("RetryButton", []() { return new RetryButton(); }, 1);
    ObjectFactory::Register("MouseUI", []() { return new MouseUI(); }, 1);
}
