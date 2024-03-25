#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "Interface/CommandInterface.h"
#include "MySpectatorPawn.generated.h"

/**
 *
 */
UCLASS()
class FLECSEXAMPLE_API AMySpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void SelectObject();
	void OrderObject();
	void ClearObjectSelection();

	ICommandInterface* SelectedPawn;
};
