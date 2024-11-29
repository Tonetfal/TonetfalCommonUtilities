// Author: Antonio Sidenko (Tonetfal), November 2024

#include "TCU_Library.h"

#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

#pragma region TypedGetters
AGameStateBase* UTCU_Library::GetTypedGameState(const UObject* ContextObject,
	TSubclassOf<AGameStateBase> Class)
{
	AGameStateBase* GameState = UGameplayStatics::GetGameState(ContextObject);
	return IsValid(GameState) && GameState->IsA(Class) ? GameState : nullptr;
}

AGameModeBase* UTCU_Library::GetTypedGameMode(const UObject* ContextObject,
	TSubclassOf<AGameModeBase> Class)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(ContextObject);
	return IsValid(GameMode) && GameMode->IsA(Class) ? GameMode : nullptr;
}

AGameSession* UTCU_Library::GetTypedGameSession(const UObject* ContextObject, TSubclassOf<AGameSession> Class)
{
	const AGameModeBase* GameMode = UGameplayStatics::GetGameMode(ContextObject);
	if (!IsValid(GameMode))
	{
		return nullptr;
	}

	AGameSession* GameSession = GameMode->GameSession;
	return IsValid(GameSession) && GameSession->IsA(Class) ? GameSession : nullptr;
}

UGameInstance* UTCU_Library::GetTypedGameInstance(const UObject* ContextObject,
	TSubclassOf<UGameInstance> Class)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(ContextObject);
	return IsValid(GameInstance) && GameInstance->IsA(Class) ? GameInstance : nullptr;
}

const AWorldSettings* UTCU_Library::GetWorldSettings(const UObject* ContextObject,
	const TSubclassOf<AWorldSettings> SettingsClass)
{
	check(IsValid(ContextObject));
	check(IsValid(SettingsClass));

	const UWorld* World = ContextObject->GetWorld();
	check(IsValid(World));

	const AWorldSettings* WorldSettings = World->GetWorldSettings();
	check(IsValid(WorldSettings));

	check(WorldSettings->IsA(SettingsClass));
	return WorldSettings;
}

APlayerController* UTCU_Library::GetTypedPlayerController(const UObject* ContextObject,
	TSubclassOf<APlayerController> Class, int32 PlayerIndex)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex);
	return IsValid(Controller) && Controller->IsA(Class) ? Controller : nullptr;
}

ULocalPlayer* UTCU_Library::GetTypedLocalPlayer(const UObject* ContextObject, TSubclassOf<ULocalPlayer> Class,
	int32 PlayerIndex)
{
	const APlayerController* Controller = UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex);
	if (!IsValid(Controller))
	{
		return nullptr;
	}

	ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
	return IsValid(LocalPlayer) && LocalPlayer->IsA(Class) ? LocalPlayer : nullptr;
}

#pragma endregion

#pragma region Player
TArray<APlayerController*> UTCU_Library::GetPlayerControllers(const UObject* ContextObject, bool bLocalOnly,
	TSubclassOf<APlayerController> Class)
{
	TArray<APlayerController*> ReturnValue;

	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (!IsValid(PlayerController))
			{
				continue;
			}

			if (bLocalOnly)
			{
				if (!PlayerController->IsLocalController())
				{
					continue;
				}
			}

			if (!PlayerController->IsA(Class))
			{
				continue;
			}

			ReturnValue.Add(PlayerController);
		}
	}

	return ReturnValue;
}

TArray<APlayerState*> UTCU_Library::GetPlayerStates(const UObject* ContextObject, bool bLocalOnly,
	TSubclassOf<APlayerState> Class)
{
	TArray<APlayerState*> ReturnValue;

	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (IsValid(World))
	{
		const AGameStateBase* GameState = World->GetGameState();
		if (IsValid(GameState))
		{
			for (TObjectPtr<APlayerState> PlayerState : GameState->PlayerArray)
			{
				if (bLocalOnly)
				{
					const APlayerController* PlayerController = PlayerState->GetPlayerController();
					if (!IsValid(PlayerController))
					{
						continue;
					}

					if (!PlayerController->IsLocalController())
					{
						continue;
					}
				}

				if (!PlayerState->IsA(Class))
				{
					continue;
				}

				ReturnValue.Add(PlayerState);
			}
		}
	}

	return ReturnValue;
}

TArray<APawn*> UTCU_Library::GetPlayerPawns(const UObject* ContextObject, bool bLocalOnly,
	TSubclassOf<APawn> Class)
{
	TArray<APawn*> ReturnValue;

	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (IsValid(World))
	{
		const AGameStateBase* GameState = World->GetGameState();
		if (IsValid(GameState))
		{
			for (TObjectPtr<APlayerState> PlayerState : GameState->PlayerArray)
			{
				if (bLocalOnly)
				{
					const APlayerController* PlayerController = PlayerState->GetPlayerController();
					if (!IsValid(PlayerController))
					{
						continue;
					}

					if (!PlayerController->IsLocalController())
					{
						continue;
					}
				}

				APawn* PlayerPawn = PlayerState->GetPawn();
				if (!IsValid(PlayerPawn))
				{
					continue;
				}

				if (!PlayerPawn->IsA(Class))
				{
					continue;
				}

				ReturnValue.Add(PlayerPawn);
			}
		}
	}

	return ReturnValue;
}

int32 UTCU_Library::GetPlayersNumber(const UObject* ContextObject, bool bLocalOnly)
{
	int32 Count = 0;

	const auto* GameState = GetGameState<AGameStateBase>(ContextObject);
	if (!IsValid(GameState))
	{
		return Count;
	}

	if (!bLocalOnly)
	{
		Count = GameState->PlayerArray.Num();
	}
	else
	{
		for (const TObjectPtr<APlayerState> PlayerState : GameState->PlayerArray)
		{
			if (PlayerState->HasLocalNetOwner())
			{
				Count++;
			}
		}
	}

	return Count;
}

int32 UTCU_Library::GetLocalPlayerIndex(const ULocalPlayer* LocalPlayer)
{
	if (IsValid(LocalPlayer))
	{
		const int32 Index = LocalPlayer->GetLocalPlayerIndex();
		return Index;
	}

	return INDEX_NONE;
}

int32 UTCU_Library::GetPlayerControllerIndex(const APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		const int32 Index = GetPlayerControllerIndex(PlayerController);
		return Index;
	}

	return INDEX_NONE;
}

ULocalPlayer* UTCU_Library::RetrieveLocalPlayer(const APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		return LocalPlayer;
	}

	return nullptr;
}

APlayerController* UTCU_Library::RetrievePlayerController(const ULocalPlayer* LocalPlayer)
{
	if (IsValid(LocalPlayer))
	{
		const UWorld* World = GEngine->GetWorldFromContextObject(LocalPlayer, EGetWorldErrorMode::LogAndReturnNull);
		APlayerController* PlayerController = LocalPlayer->GetPlayerController(World);
		return PlayerController;
	}

	return nullptr;
}
#pragma endregion

#pragma region Time
float UTCU_Library::GetTime(const UObject* ContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	const float Time = IsValid(World) ? World->GetTimeSeconds() : 0.f;
	return Time;
}

float UTCU_Library::TimeSince(const UObject* ContextObject, float Time)
{
	const float TimeSince = GetTime(ContextObject) - Time;
	return TimeSince;
}

float UTCU_Library::GetTime_Server(const UObject* ContextObject)
{
	const AGameStateBase* GameState = UGameplayStatics::GetGameState(ContextObject);
	if (IsValid(GameState))
	{
		const float ReturnValue = GameState->GetServerWorldTimeSeconds();
		return ReturnValue;
	}

	return 0.f;
}

float UTCU_Library::TimeSince_Server(const UObject* ContextObject, float Time)
{
	const float TimeSince = GetTime_Server(ContextObject) - Time;
	return TimeSince;
}
#pragma endregion

#pragma region Build
int32 UTCU_Library::GetNetworkVersion()
{
	return FNetworkVersion::GetNetworkCompatibleChangelist();
}

FString UTCU_Library::GetFormattedDate()
{
	return UTF8_TO_TCHAR(__DATE__);
}

FString UTCU_Library::GetFormattedTime()
{
	return UTF8_TO_TCHAR(__TIME__);
}
#pragma endregion

#pragma region Misc
void UTCU_Library::CppStackTrace(FString Heading)
{
	FDebug::DumpStackTraceToLog(*Heading, ELogVerbosity::Type::Log);
}

void UTCU_Library::AllowAnyoneDestroyComponent(UActorComponent* ActorComponent, bool bAllow)
{
	if (IsValid(ActorComponent))
	{
		ActorComponent->bAllowAnyoneToDestroyMe = bAllow;
	}
}

void UTCU_Library::RerunConstructionScript(AActor* Target)
{
#if WITH_EDITOR
	if (IsValid(Target))
	{
		Target->RerunConstructionScripts();
	}
#endif
}

void UTCU_Library::SetUnfocusedVolumeMultiplier(float InVolumeMultiplier)
{
	FApp::SetUnfocusedVolumeMultiplier(InVolumeMultiplier);
}

void UTCU_Library::CancelAllLatentActions(UObject* ContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		auto* ObjectActionList = LatentActionManager.ObjectToActionListMap.Find(ContextObject);
		if (ObjectActionList && ObjectActionList->IsValid())
		{
			for (auto [UUID, LatentAction] : (*ObjectActionList)->ActionList)
			{
				LatentAction->NotifyObjectDestroyed();
				delete LatentAction;
			}

			(*ObjectActionList)->ActionList.Reset();
		}
	}
}

bool UTCU_Library::IsEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

bool UTCU_Library::K2_IsEditor()
{
	return IsEditor();
}

bool UTCU_Library::IsPreviewWorld(const UObject* ContextObject)
{
	return IsValid(ContextObject) ? ContextObject->GetWorld()->IsPreviewWorld() : false;
}

APlayerStart* UTCU_Library::FindPlayerStart(const APlayerController* Controller, const FString& IncomingName,
	const TSubclassOf<APawn> PawnClass)
{
	if (!IsValid(Controller))
	{
		return nullptr;
	}

	UWorld* World = Controller->GetWorld();

	if (!IncomingName.IsEmpty())
	{
		const APawn* PawnToFit = IsValid(PawnClass) ? PawnClass->GetDefaultObject<APawn>() : nullptr;

		const FName IncomingPlayerStartTag = FName(*IncomingName);
		TArray<APlayerStart*> MatchingPlayerStarts;
		for (TActorIterator<APlayerStart> It(World); It; ++It)
		{
			APlayerStart* PlayerStart = *It;
			if (PlayerStart && PlayerStart->PlayerStartTag == IncomingPlayerStartTag)
			{
				const FVector ActorLocation = PlayerStart->GetActorLocation();
				const FRotator ActorRotation = PlayerStart->GetActorRotation();
				const bool bCanFit = !World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation);
				if (bCanFit)
				{
					MatchingPlayerStarts.Add(PlayerStart);
				}
				else
				{
					UE_LOG(LogGameMode, Log, TEXT("Skipping player start at [%s] because player can't fit in"),
						*ActorLocation.ToString());
				}
			}
		}

		if (!MatchingPlayerStarts.IsEmpty())
		{
			// Get a random player start
			const int32 RandIndex = FMath::RandRange(0, MatchingPlayerStarts.Num() - 1);
			return MatchingPlayerStarts[RandIndex];
		}
	}

	// Choose a player start
	APlayerStart* FoundPlayerStart = nullptr;
	APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	TArray<APlayerStart*> OccupiedStartPoints;
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (PlayerStart->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			return FoundPlayerStart;
		}

		FVector ActorLocation = PlayerStart->GetActorLocation();
		const FRotator ActorRotation = PlayerStart->GetActorRotation();
		if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
		{
			UnOccupiedStartPoints.Add(PlayerStart);
		}
		else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
		{
			OccupiedStartPoints.Add(PlayerStart);
		}
	}

	if (!IsValid(FoundPlayerStart))
	{
		if (!UnOccupiedStartPoints.IsEmpty())
		{
			FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (!OccupiedStartPoints.IsEmpty())
		{
			FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}

	return FoundPlayerStart;
}

void UTCU_Library::DoNothing()
{
	// https://landelare.github.io/2022/04/30/uses-of-a-useless-node.html
}
#pragma endregion Misc

#pragma region Networking
bool UTCU_Library::IsDedicatedServer(const UObject* WorldContextObject)
{
	return UKismetSystemLibrary::IsDedicatedServer(WorldContextObject);
}
#pragma endregion
