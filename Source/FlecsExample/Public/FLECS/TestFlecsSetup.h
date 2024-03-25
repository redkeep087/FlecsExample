#pragma once

#include "FLECS/FlecsSetupClass.h"
#include "FLECS/Common/FlecsClient.h"

class UWorld;

class TestFlecsSetup : public UFlecsSetup
{
public:
    TestFlecsSetup(UWorld* _world, flecs::world* _ecs);
    void PostInitialize();
    ~TestFlecsSetup();

    virtual FFlecsEntityHandle RegisterFlecsActor(IFlecsClient* actor) override;
};