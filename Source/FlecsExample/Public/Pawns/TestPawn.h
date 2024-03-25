#pragma once

#include "GameFramework/Pawn.h"
#include "Interface/CommandInterface.h"
#include "FLECS/Common/FlecsClient.h"
#include "TestPawn.generated.h"

UCLASS()
class FLECSEXAMPLE_API ATestPawn : public APawn, public ICommandInterface {
	GENERATED_BODY()
public:
	ATestPawn();

	UFUNCTION()
	virtual void CommandMove(FVector position) override;
	
	UPROPERTY()
		UFlecsClientComponent* flecsClientComp = nullptr;
};