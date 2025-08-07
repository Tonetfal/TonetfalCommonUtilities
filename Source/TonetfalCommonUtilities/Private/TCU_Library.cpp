// Author: Antonio Sidenko (Tonetfal), November 2024

#include "TCU_Library.h"

#include "Blueprint/UserWidget.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "TonetfalCommonUtilities"

#pragma region Blueprints
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

AHUD* UTCU_Library::GetTypedHUD(const APlayerController* PlayerController, TSubclassOf<AHUD> Class)
{
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}

	AHUD* HUD = PlayerController->GetHUD();
	return IsValid(HUD) && HUD->IsA(Class) ? HUD : nullptr;
}

const AWorldSettings* UTCU_Library::GetWorldSettings(const UObject* ContextObject,
	const TSubclassOf<AWorldSettings> SettingsClass)
{
	if (!IsValid(SettingsClass))
	{
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return nullptr;
	}

	const AWorldSettings* WorldSettings = World->GetWorldSettings();
	check(WorldSettings->IsA(SettingsClass));

	return WorldSettings;
}

APlayerController* UTCU_Library::GetTypedPlayerController(const UObject* ContextObject,
	TSubclassOf<APlayerController> Class, int32 PlayerIndex, bool bLocalOnly)
{
	if (!bLocalOnly)
	{
		APlayerController* Controller = UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex);
		return IsValid(Controller) && Controller->IsA(Class) ? Controller : nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (IsValid(World))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (IsValid(PlayerController) && bLocalOnly && PlayerController->IsLocalController())
			{
				return PlayerController;
			}
		}
	}

	return nullptr;
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

#pragma region Actor
AActor* UTCU_Library::GetActorOfClassWithInterface(const UObject* WorldContextObject, TSubclassOf<UInterface> Interface)
{
	// QUICK_SCOPE_CYCLE_COUNTER(UGameplayStatics_GetActorOfClassWithInterface);

	// We do nothing if no interface provided, rather than giving ALL actors!
	if (!Interface)
	{
		return nullptr;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->GetClass()->ImplementsInterface(Interface))
			{
				return Actor;
			}
		}
	}

	return nullptr;
}

AActor* UTCU_Library::GetActorOfClassWithTag(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass,
	FName Tag)
{
	// QUICK_SCOPE_CYCLE_COUNTER(UGameplayStatics_GetActorOfClassWithTag);

	// We do nothing if no tag is provided, rather than giving ALL actors!
	if (Tag.IsNone())
	{
		return nullptr;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (TActorIterator It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;
			if (IsValid(Actor) && Actor->ActorHasTag(Tag))
			{
				return Actor;
			}
		}
	}

	return nullptr;
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

#pragma region Player
APlayerController* UTCU_Library::GetTypedOwningPlayer(UUserWidget* Widget, TSubclassOf<APlayerController> Class)
{
	if (IsValid(Widget))
	{
		APlayerController* PlayerController = Widget->GetOwningPlayer();
		if (IsValid(PlayerController) && PlayerController->IsA(Class))
		{
			return PlayerController;
		}
	}

	return nullptr;
}

APawn* UTCU_Library::GetTypedOwningPlayerPawn(UUserWidget* Widget, TSubclassOf<APawn> Class)
{
	if (IsValid(Widget))
	{
		APawn* Pawn = Widget->GetOwningPlayerPawn();
		if (IsValid(Pawn) && Pawn->IsA(Class))
		{
			return Pawn;
		}
	}

	return nullptr;
}

APlayerState* UTCU_Library::GetTypedOwningPlayerState(UUserWidget* Widget, TSubclassOf<APlayerState> Class)
{
	if (IsValid(Widget))
	{
		APlayerController* PlayerController = Widget->GetOwningPlayer();
		if (IsValid(PlayerController))
		{
			auto* PlayerState = PlayerController->GetPlayerState<APlayerState>();
			if (IsValid(PlayerState) && PlayerState->IsA(Class))
			{
				return PlayerState;
			}
		}
	}

	return nullptr;
}

bool UTCU_Library::IsHandled(FEventReply Reply)
{
	return Reply.NativeReply.IsEventHandled();
}

bool UTCU_Library::IsDesignTime(const UUserWidget* Widget)
{
	return IsValid(Widget) ? Widget->IsDesignTime() : false;
}

bool UTCU_Library::K2_IsDesignTime(const UUserWidget* Widget)
{
	return IsDesignTime(Widget);
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

float UTCU_Library::GetTime_Explicit(const UObject* ContextObject)
{
	return GetTime(ContextObject);
}

float UTCU_Library::TimeSince_Explicit(const UObject* ContextObject, float Time)
{
	return TimeSince(ContextObject, Time);
}

float UTCU_Library::GetTime_Server_Explicit(const UObject* ContextObject)
{
	return GetTime_Server(ContextObject);
}

float UTCU_Library::TimeSince_Server_Explicit(const UObject* ContextObject, float Time)
{
	return TimeSince_Server(ContextObject, Time);
}
#pragma endregion

#pragma region Gameplay Tags
FGameplayTagContainer UTCU_Library::RemoveTags(const FGameplayTagContainer& Target, const FGameplayTagContainer& Filter)
{
	FGameplayTagContainer ResultContainer = Target;
	ResultContainer.RemoveTags(Filter);
	return ResultContainer;
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
TArray<UObject*> UTCU_Library::CastArray(TArray<UObject*> Array, TSubclassOf<UObject> Class)
{
	return Array;
}

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
		LatentActionManager.RemoveActionsForObject(ContextObject);
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

bool UTCU_Library::IsWorldTearingDown(const UObject* ContextObject)
{
	return IsValid(ContextObject) ? ContextObject->GetWorld()->bIsTearingDown : false;
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

void UTCU_Library::ClipboardCopy(const FString& String)
{
	FPlatformApplicationMisc::ClipboardCopy(*String);
}

void UTCU_Library::DoNothing()
{
	// https://landelare.github.io/2022/04/30/uses-of-a-useless-node.html
}

void UTCU_Library::SetActorLabel(AActor* Target, const FString& NewActorLabel, bool bMarkDirty)
{
#if WITH_EDITOR
	if (IsValid(Target))
	{
		Target->SetActorLabel(NewActorLabel, bMarkDirty);
	}
#endif
}
#pragma endregion Misc

#pragma region Networking
bool UTCU_Library::IsDedicatedServer(const UObject* WorldContextObject)
{
	return UKismetSystemLibrary::IsDedicatedServer(WorldContextObject);
}

FString UTCU_Library::ToString(FUniqueNetIdRepl UniqueNetId)
{
	const FString ReturnValue = UniqueNetId.ToString();
	return ReturnValue;
}

FUniqueNetIdRepl UTCU_Library::ToNetId(const FString& String)
{
	FUniqueNetIdRepl ReturnValue;
	ReturnValue.FromJson(String);
	return ReturnValue;
}

bool UTCU_Library::IsNetIdValid(FUniqueNetIdRepl UniqueNetId)
{
	return UniqueNetId.IsValid();
}

FUniqueNetIdRepl UTCU_Library::GetNetIdFromController(const APlayerController* Controller)
{
	if (IsValid(Controller) && IsValid(Controller->PlayerState))
	{
		return Controller->PlayerState->GetUniqueId();
	}

	return FUniqueNetIdRepl();
}
#pragma endregion

#pragma region String
static bool IsSpace(FString::ElementType Character)
{
	return Character == ' ' || Character == '\n' || Character == '\r';
}

FString UTCU_Library::TrimLeadingSpaces(FString InString, bool& bOutHasTrimmed)
{
	const int32 Count = InString.Len();
	for (int32 i = 0; i < Count; i++)
	{
		const FString::ElementType Character = InString[0];
		if (IsSpace(Character))
		{
			InString.RemoveAt(0);
			bOutHasTrimmed = true;
		}
		else
		{
			break;
		}
	}

	return InString;
}

FString UTCU_Library::TrimTrailingSpaces(FString InString, bool& bOutHasTrimmed)
{
	const int32 Count = InString.Len();
	for (int32 i = Count - 1; i >= 0; i--)
	{
		const FString::ElementType Character = InString[i];
		if (IsSpace(Character))
		{
			InString.RemoveAt(i);
			bOutHasTrimmed = true;
		}
		else
		{
			break;
		}
	}

	return InString;
}

FString UTCU_Library::TrimSurroundingSpaces(FString InString, bool& bOutHasTrimmed)
{
	bool bTrimmed = false;

	InString = TrimLeadingSpaces(InString, bTrimmed);
	bOutHasTrimmed |= bTrimmed;

	InString = TrimTrailingSpaces(InString, bTrimmed);
	bOutHasTrimmed |= bTrimmed;

	return InString;
}

FString UTCU_Library::LimitString(FString InString, int32 Limit, bool& bOutLimited)
{
	if (Limit >= InString.Len())
	{
		bOutLimited = false;
		return InString;
	}

	FString ReturnValue = InString;
	ReturnValue.LeftInline(Limit);

	bOutLimited = true;
	return ReturnValue;
}

FString UTCU_Library::Repeat(FString String, int32 Count)
{
	FString ReturnValue;
	for (int32 i = 0; i < Count; ++i)
	{
		ReturnValue += String;
	}

	return ReturnValue;
}
#pragma endregion
#pragma endregion

#pragma region C++
#pragma region Misc
bool UTCU_Library::IsWorldType(const UObject* ContextObject, EWorldType::Type Type)
{
	return IsValid(ContextObject) ? ContextObject->GetWorld()->WorldType == Type : false;
}
#pragma endregion
#pragma endregion

UTCU_Settings::UTCU_Settings()
{
	CategoryName = "Plugins";
	SectionName = "Tonetfal's Common Utilities";
}

#if WITH_EDITOR
FText UTCU_Settings::GetSectionText() const
{
	return LOCTEXT("SettingsDisplayName", "Tonetfal's Common Utilities");
}
#endif

float UTCU_Settings::GetMaxWaitingTime()
{
	const auto* This = GetDefault<ThisClass>();
	return This->MaxWaitingTime;
}

#undef LOCTEXT_NAMESPACE
