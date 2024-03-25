#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "AI/Navigation/AvoidanceManager.h"
#include "Math/GenericOctree.h"

struct FlecsFollowTarget {
    FVector location;
};

struct FlecsFollowActorTarget {
    AActor* actor;
    FVector targetLocation;
};

struct FlecsNavigationSingleton {
    UNavigationSystemV1* NavSystem;
    UAvoidanceManager* AvoidanceManager;
};

struct FlecsNavigationPath {
    TArray<FVector> points;
};

struct FlecsRVO_Instance {
    int32 AvoidanceUID;
};

struct FlecsPosition {
    FVector value;
};

struct FlecsRotationZ {
    float value;
};

struct FlecsTransform {
    FTransform value;
};

struct FlecsRVO_Data {

};

struct FlecsSpatialGrid {
    //TOctree2<flecs::entity, FVector> gridTree;
};

struct FlecsSpatialGridClient {

};