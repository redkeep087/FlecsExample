#pragma once

#include "CoreMinimal.h"
#include "Subsystems/FlecsSubsystem.h"
#include "FLECS/TestFlecsSetup.h"
#include "TestFlecsSubsystem.generated.h"

UCLASS()
class FLECSEXAMPLE_API UTestFlecsSubsystem : public UFlecsSubsystem {
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void PostInitialize();
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	virtual FFlecsEntityHandle RegisterEntity_Internal(IFlecsClient* client) override;
public:
	TestFlecsSetup* testFlecsSetup = nullptr;
};