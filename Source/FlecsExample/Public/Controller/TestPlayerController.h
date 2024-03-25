#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FLECSEXAMPLE_API ATestPlayerController : public APlayerController {
	GENERATED_BODY()
public:

	virtual ASpectatorPawn* SpawnSpectatorPawn() override;

	virtual void BeginPlayingState() override;

	virtual void BeginSpectatingState() override;

	bool GetHitResultsAtScreenPosition(const FVector2D ScreenPosition, const ECollisionChannel TraceChannel, const FCollisionQueryParams& CollisionQueryParams, TArray<FHitResult>& HitResults) const;

	bool GetHitResultsUnderCursor(ECollisionChannel TraceChannel, bool bTraceComplex, TArray<FHitResult>& HitResults);
};