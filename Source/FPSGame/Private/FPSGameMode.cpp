// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	//指定使用的是哪一种GameState
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn *InstigatorPawn,bool bMissionSuccess)
{
	

	if (SpectatingViewPointClass)
	{
		TArray<AActor*> ReturnedActors;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewPointClass, ReturnedActors);
			
		//改变视角，如果找到了valid actor
		if (ReturnedActors.Num() > 0)
		{
			AActor * NewViewTarget = ReturnedActors[0];

			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController * PC = It->Get();
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 2.0f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
			
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpectatingViewPointClass is nuller.Please update GameMode class with valid subclass.Cannot change spectating view to target"));
	}
	
	AFPSGameState * GS =GetGameState<AFPSGameState>();
	if (GS)
	{
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}

	OnMissionComplete(InstigatorPawn, bMissionSuccess);
	
}
