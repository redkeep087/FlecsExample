#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommandInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FLECSEXAMPLE_API ICommandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CommandMove(FVector postion) = 0;
};
