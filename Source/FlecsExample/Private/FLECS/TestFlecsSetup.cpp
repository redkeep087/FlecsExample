// TODO: Find a better way to setup constants

#include "FLECS/TestFlecsSetup.h"
#include "FLECS/TestComponents.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Math/UnrealMathUtility.h"
#include "Subsystems/TestFlecsSubsystem.h"
#include "UObject/UObjectGlobals.h"

TestFlecsSetup::TestFlecsSetup(UWorld* _world, flecs::world* _ecs) : UFlecsSetup(_world, _ecs) {
	_ecs->component<FlecsPosition>().member<double>("X").member<double>("Y").member<double>("Z");

	// TODO: Find a better organization
	auto update_position_actor = _ecs->system<FlecsPosition&, FlecsCommunicator&>("Update position Actor")
		.multi_threaded()
		.each([](FlecsPosition& p, FlecsCommunicator& a) {
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("FLECS Update position"); {
			FVector location = a.ptr->GetActor()->GetActorLocation();
			p.value = location;
		}
			});
	update_position_actor.disable();

	auto update_position_actor_flecs = _ecs->system<FlecsPosition&, FlecsCommunicator&>("Update flecs position Actor")
		.multi_threaded()
		.kind<FlecsFixedUpdate>()
		.each([](FlecsPosition& p, FlecsCommunicator& a) {
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("FLECS Update position"); {
			a.ptr->GetActor()->SetActorLocation(p.value);
		}
			});
	update_position_actor_flecs.enable();

	auto update_follow_actor = _ecs->system<FlecsNavigationSingleton&, FlecsPosition&, FlecsFollowActorTarget&>("Update Follow Actor")
		.multi_threaded()
		.term_at(1).singleton()
		.kind<FlecsFixedUpdate>()
		.each([](flecs::entity e, FlecsNavigationSingleton& navSingleton, FlecsPosition& p, FlecsFollowActorTarget& at) {
		FVector targetLocation = at.actor->GetActorLocation();
		FVector diff = targetLocation - at.targetLocation;
		float sqrDist = diff.SquaredLength();
		const float compareDist = 20;
		const float sqrCompareDist = compareDist * compareDist;
		if (sqrDist > compareDist) {
			at.targetLocation = targetLocation;
			// TODO make it Asynchronous
			UNavigationPath* NavPath = navSingleton.NavSystem->FindPathToLocationSynchronously(navSingleton.NavSystem->GetWorld(), p.value, targetLocation);
			if (NavPath && NavPath->IsValid())
			{
				e.set<FlecsNavigationPath>({ NavPath->PathPoints });
				e.set<FlecsFollowTarget>({ NavPath->PathPoints[0] });
			}
		}
		});

	auto update_track_path = _ecs->system<FlecsPosition&, FlecsFollowTarget&, FlecsNavigationPath&>("Update follow path")
		.multi_threaded()
		.kind<FlecsFixedUpdate>()
		.each([](flecs::entity e, FlecsPosition& p, FlecsFollowTarget& t, FlecsNavigationPath& paths) {
		FVector dir = t.location - p.value;
		float diff = dir.Length();
		const float FIXED_TIME = (float)1 / 25;
		if (diff < 100 * FIXED_TIME * 1.05) {
			if (paths.points.Num() != 0) {
				paths.points.RemoveAt(0);
				if (paths.points.Num() > 0) {
					t.location = paths.points[0];
				}
			}
		}
		if (diff < 5 && paths.points.IsEmpty()) {
			e.remove<FlecsFollowTarget>();
		}
		});

	auto update_follow_path = _ecs->system<FlecsNavigationSingleton&, FlecsPosition&, FlecsFollowTarget&>("Update follow target")
		.multi_threaded()
		.term_at(1).singleton()
		.kind<FlecsFixedUpdate>()
		.each([](FlecsNavigationSingleton& navSingleton, FlecsPosition& p, FlecsFollowTarget& t) {
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("FLECS Update follow target"); {
			const float groundTraceLength = 500;
			FNavLocation newLocation;
			if (navSingleton.NavSystem->ProjectPointToNavigation(p.value, newLocation, FVector::OneVector * 500)) {
				FHitResult GroundHit(1.f);
				FVector pos = p.value;
				navSingleton.NavSystem->GetWorld()->LineTraceSingleByChannel(
					GroundHit,
					pos + FVector::UpVector * groundTraceLength * 0.25,
					pos + FVector::DownVector * groundTraceLength * 1.25,
					ECollisionChannel::ECC_Visibility);
				if (GroundHit.IsValidBlockingHit()) {
					p.value = GroundHit.ImpactPoint;
				}
			}

			if (navSingleton.NavSystem->ProjectPointToNavigation(p.value, newLocation, FVector::OneVector * 500)) {
				FHitResult GroundHit(1.f);
				FVector pos = t.location;
				navSingleton.NavSystem->GetWorld()->LineTraceSingleByChannel(
					GroundHit,
					pos + FVector::UpVector * groundTraceLength * 0.25,
					pos + FVector::DownVector * groundTraceLength * 1.25,
					ECollisionChannel::ECC_Visibility);

				if (GroundHit.IsValidBlockingHit()) {
					t.location = GroundHit.ImpactPoint;
				}
			}

			// Calculate the direction to move towards
			//float distanceBetweenPositions = FVector::Dist(p.value, t.location);
			FVector Direction = (t.location - p.value).GetSafeNormal();

			const float FIXED_TIME = (float)1 / 25;
			// Calculate the distance to move in this frame based on the Speed
			float DistanceThisFrame = 100 * FIXED_TIME;// it.delta_time();

			float DistBetweenPoints = FVector::Dist(p.value, t.location);

			p.value += Direction * FMath::Min(DistanceThisFrame, DistBetweenPoints);

			UE_LOG(LogTemp, Warning, TEXT("MOVED IN %f about %f"), FIXED_TIME, DistBetweenPoints);
		}
		});
}

void TestFlecsSetup::PostInitialize()
{
	UAvoidanceManager* AvoidanceManager = NewObject<UAvoidanceManager>(world, GEngine->AvoidanceManagerClass);
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(world);
	if (navSystem) {
		ecs->set<FlecsNavigationSingleton>({ navSystem, AvoidanceManager });
	}
}

TestFlecsSetup::~TestFlecsSetup() {
}

FFlecsEntityHandle TestFlecsSetup::RegisterFlecsActor(IFlecsClient* actor) {
	auto entity = ecs->entity();
	entity.set<FlecsTransform>({ actor->GetActor()->GetTransform() });
	entity.set<FlecsPosition>({ actor->GetActor()->GetActorLocation() });
	entity.set<FlecsCommunicator>({ actor });
	return FFlecsEntityHandle({ entity });
}