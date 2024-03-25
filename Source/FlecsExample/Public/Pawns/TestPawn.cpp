#include "TestPawn.h"
#include "FLECS/TestComponents.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"

ATestPawn::ATestPawn()
{
	flecsClientComp = CreateDefaultSubobject<UFlecsClientComponent>(TEXT("FlecsComp"));
}

void ATestPawn::CommandMove(FVector position) {
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (flecsClientComp) {
		flecs::entity e = flecsClientComp->GetEntityHandle().FlecsEntity;
		const FlecsFollowActorTarget* followActorTargetPtr = e.get<FlecsFollowActorTarget>();
		if (followActorTargetPtr == nullptr) {
			FActorSpawnParameters SpawnParams;
			AActor* targetActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), position, FRotator(0, GetActorRotation().Yaw, 0), SpawnParams);
			USceneComponent* scene = NewObject<USceneComponent>(targetActor);
			targetActor->SetRootComponent(scene);
			if (targetActor) {
				// We'll need a target with specific distance threshold to call the pathfinding on the system
				e.set<FlecsFollowActorTarget>({ targetActor, position + FVector::OneVector * 200 });
			}
			followActorTargetPtr = e.get<FlecsFollowActorTarget>();
		}
		if (followActorTargetPtr && followActorTargetPtr->actor) {
			followActorTargetPtr->actor->SetActorLocation(position);
		}

		UE_LOG(LogTemp, Warning, TEXT("Added command move"));
	}
}