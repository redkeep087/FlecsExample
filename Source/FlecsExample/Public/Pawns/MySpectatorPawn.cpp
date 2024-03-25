#include "MySpectatorPawn.h"
#include "Controller/TestPlayerController.h"

void AMySpectatorPawn::SelectObject() {
	UE_LOG(LogTemp, Warning, TEXT("Select Object called"));

	ClearObjectSelection();

	TArray<FHitResult> HitResults;
	ATestPlayerController* playerController = Cast<ATestPlayerController>(GetController());
	if (playerController == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Please assign ATestPlayerController"));
		return;
	}

	playerController->GetHitResultsUnderCursor(ECC_EngineTraceChannel5, false, HitResults);
	for (FHitResult& HitResult : HitResults) {
		if (HitResult.GetActor() != nullptr)
		{
			//#ifndef UE_BUILD_SHIPPING 
			//UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *HitResult.GetActor()->GetActorLabel());
			////#endif
			//			// Object selected, you can perform actions on the selected object here
			//
			//			// First we have to check if there is a formation parent of selection
			//			// Second if there is no formation parent then our actor is the selection
			//IFormationClient* formationClient = Cast<IFormationClient>(HitResult.GetActor());
			//if (!formationClient) {
			//	for (auto c : HitResult.GetActor()->GetComponents()) {
			//		formationClient = Cast<IFormationClient>(c);
			//		if (formationClient) { break; }
			//	}
			//}
			//
			//UPoolingSubsystem* poolingSubsystem = GetGameInstance()->GetSubsystem<UPoolingSubsystem>();
			//ICommandInterface* pawn = Cast<ICommandInterface>(formationClient);
			//if (formationClient && poolingSubsystem) {
			//	AFormationParentPawn* formationParent = poolingSubsystem->GetFormationParentPawnFromClient(formationClient);
			//	if (formationParent) {
			//		pawn = Cast<ICommandInterface>(formationParent);
			//	}
			//}

			ICommandInterface* pawn = Cast<ICommandInterface>(HitResult.GetActor());

			if (pawn) {
				SelectedPawn = pawn;
				break;
			}
		}
	}
}


void AMySpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("SelectAction", IE_Pressed, this, &AMySpectatorPawn::SelectObject);
	PlayerInputComponent->BindAction("OrderAction", IE_Pressed, this, &AMySpectatorPawn::OrderObject);
}


void AMySpectatorPawn::OrderObject() {
	if (SelectedPawn == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Order Object called"));

	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController == nullptr) return;

	FHitResult HitResult;
	playerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	SelectedPawn->CommandMove(HitResult.ImpactPoint);

	//AAIController* aiController = Cast<AAIController>(SelectedPawn->GetController());
	//if (aiController) {
	//	aiController->MoveToLocation(HitResult.ImpactPoint);
	//}
}

void AMySpectatorPawn::ClearObjectSelection() {
	UE_LOG(LogTemp, Warning, TEXT("Clearing all selection"));
	SelectedPawn = nullptr;
}
