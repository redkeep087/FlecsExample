#include "Subsystems/TestFlecsSubsystem.h"

void UTestFlecsSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

void UTestFlecsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	testFlecsSetup = new TestFlecsSetup(&InWorld, GetEcsWorld());
	if (testFlecsSetup) {
		testFlecsSetup->PostInitialize();
	}
}

void UTestFlecsSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

FFlecsEntityHandle UTestFlecsSubsystem::RegisterEntity_Internal(IFlecsClient* client) {
	if (client && testFlecsSetup) {
		return testFlecsSetup->RegisterFlecsActor(client);
	}
	return FFlecsEntityHandle();
}

void UTestFlecsSubsystem::Deinitialize() {
	if (testFlecsSetup) {
		delete testFlecsSetup;
	}

	Super::Deinitialize();
}