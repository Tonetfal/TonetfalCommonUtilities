// Author: Antonio Sidenko (Tonetfal), November 2024

#pragma once

#include "GameFramework/GameStateBase.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "TCU_Library.generated.h"

UCLASS()
class TONETFALCOMMONUTILITIES_API UTCU_Library
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
#pragma region Blueprints
#pragma region TypedGetters
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="ContextObject", DeterminesOutputType="Class"))
	static AGameStateBase* GetTypedGameState(const UObject* ContextObject, TSubclassOf<AGameStateBase> Class);

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="ContextObject", DeterminesOutputType="Class"))
	static AGameModeBase* GetTypedGameMode(const UObject* ContextObject, TSubclassOf<AGameModeBase> Class);

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="ContextObject", DeterminesOutputType="Class"))
	static AGameSession* GetTypedGameSession(const UObject* ContextObject, TSubclassOf<AGameSession> Class);

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="ContextObject", DeterminesOutputType="Class"))
	static APlayerController* GetTypedPlayerController(const UObject* ContextObject,
		TSubclassOf<APlayerController> Class, int32 PlayerIndex);

	UFUNCTION(BlueprintPure, Category="Game", meta=(DefaultToSelf="ContextObject", HidePin="ContextObject",
		DeterminesOutputType="SettingsClass"))
	static const AWorldSettings* GetWorldSettings(const UObject* ContextObject,
		const TSubclassOf<AWorldSettings> SettingsClass);

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="ContextObject", DeterminesOutputType="Class"))
	static ULocalPlayer* GetTypedLocalPlayer(const UObject* ContextObject,
		TSubclassOf<ULocalPlayer> Class, int32 PlayerIndex);

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="ContextObject", DeterminesOutputType="Class"))
	static UGameInstance* GetTypedGameInstance(const UObject* ContextObject, TSubclassOf<UGameInstance> Class);
#pragma endregion

#pragma region Player
	UFUNCTION(BlueprintPure="False", Category="Game|Player",
		meta=(DefaultToSelf="ContextObject", HidePin="ContextObject", DeterminesOutputType="Class"))
	static TArray<APlayerController*> GetPlayerControllers(const UObject* ContextObject, bool bLocalOnly,
		TSubclassOf<APlayerController> Class);

	UFUNCTION(BlueprintPure="False", Category="Game|Player",
		meta=(DefaultToSelf="ContextObject", HidePin="ContextObject", DeterminesOutputType="Class"))
	static TArray<APlayerState*> GetPlayerStates(const UObject* ContextObject, bool bLocalOnly,
		TSubclassOf<APlayerState> Class);

	UFUNCTION(BlueprintPure="False", Category="Game|Player",
		meta=(DefaultToSelf="ContextObject", HidePin="ContextObject", DeterminesOutputType="Class"))
	static TArray<APawn*> GetPlayerPawns(const UObject* ContextObject, bool bLocalOnly,
		TSubclassOf<APawn> Class);

	UFUNCTION(BlueprintPure, Category="Game|Player", meta=(DefaultToSelf="ContextObject", HidePin="ContextObject"))
	static int32 GetPlayersNumber(const UObject* ContextObject, bool bLocalOnly = false);

	UFUNCTION(BlueprintPure, Category="Game|Player")
	static int32 GetLocalPlayerIndex(const ULocalPlayer* LocalPlayer);

	UFUNCTION(BlueprintPure, Category="Game|Player")
	static int32 GetPlayerControllerIndex(const APlayerController* PlayerController);

	UFUNCTION(BlueprintPure, Category="Game|Player")
	static ULocalPlayer* RetrieveLocalPlayer(const APlayerController* PlayerController);

	UFUNCTION(BlueprintPure, Category="Game|Player")
	static APlayerController* RetrievePlayerController(const ULocalPlayer* LocalPlayer);
#pragma endregion

#pragma region Time
	UFUNCTION(BlueprintPure, Category="Game|Time", meta=(DefaultToSelf="ContextObject", HidePin="ContextObject",
		CompactNodeTitle="Get Time", BlueprintThreadSafe))
	static float GetTime(const UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category="Game|Time", meta=(DefaultToSelf="ContextObject", HidePin="ContextObject",
		CompactNodeTitle="Time Since", BlueprintThreadSafe))
	static float TimeSince(const UObject* ContextObject, float Time);

	UFUNCTION(BlueprintPure, Category="Game|Time", meta=(DefaultToSelf="ContextObject", HidePin="ContextObject",
		CompactNodeTitle="Get Time (Server)", BlueprintThreadSafe))
	static float GetTime_Server(const UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category="Game|Time", meta=(DefaultToSelf="ContextObject", HidePin="ContextObject",
		CompactNodeTitle="Time Since (Server)", BlueprintThreadSafe))
	static float TimeSince_Server(const UObject* ContextObject, float Time);
#pragma endregion

#pragma region Build
	UFUNCTION(BlueprintPure, Category="Game|Build")
	static int32 GetNetworkVersion();

	UFUNCTION(BlueprintPure, Category="Game|Build")
	static FString GetFormattedDate();

	UFUNCTION(BlueprintPure, Category="Game|Build")
	static FString GetFormattedTime();
#pragma endregion

#pragma region Misc
	UFUNCTION(BlueprintCallable, Category="Game|Misc", DisplayName="Stack Trace (C++)")
	static void CppStackTrace(FString Heading);

	UFUNCTION(BlueprintCallable, Category="Game|Misc")
	static void AllowAnyoneDestroyComponent(UActorComponent* ActorComponent, bool bAllow);

	UFUNCTION(BlueprintCallable, Category="Game|Misc", meta=(DevelopmentOnly))
	static void RerunConstructionScript(AActor* Target);

	UFUNCTION(BlueprintCallable, Category="Game|Misc")
	static void SetUnfocusedVolumeMultiplier(float InVolumeMultiplier);

	UFUNCTION(BlueprintCallable, Category="Game|Misc", meta=(DefaultToSelf="ContextObject"))
	static void CancelAllLatentActions(UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category="Game|Misc")
	static bool IsEditor();

	UFUNCTION(BlueprintPure="False", Category="Game|Misc", DisplayName="Is Editor",
		meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool K2_IsEditor();

	UFUNCTION(BlueprintPure, Category="Game|Misc", meta=(DefaultToSelf="ContextObject", HidePin="ContextObject"))
	static bool IsPreviewWorld(const UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category="Game|Misc")
	static APlayerStart* FindPlayerStart(const APlayerController* Controller, const FString& IncomingName,
		const TSubclassOf<APawn> PawnClass);

	UFUNCTION(BlueprintCallable, Category="Game|Misc", meta=(CompactNodeTitle="Do Nothing", DevelopmentOnly))
	static void DoNothing();
#pragma endregion

#pragma region Networking
	UFUNCTION(BlueprintCallable, Category="Game|Networking",
		meta=(WorldContext="WorldContextObject", ExpandBoolAsExecs="ReturnValue"))
	static bool IsDedicatedServer(const UObject* WorldContextObject);
#pragma endregion
#pragma endregion

#pragma region C++
#pragma region TypedGetters
	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameMode(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameState(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameSession(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameInstance(const UObject* ContextObject);

	template<typename UserClass>
	[[nodiscard]] static UserClass* GetWorldSettings(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetPlayerController(const UObject* ContextObject, int32 PlayerIndex);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetLocalPlayer(const UObject* ContextObject, int32 PlayerIndex);

#pragma region Checked
	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameMode_Checked(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameState_Checked(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameSession_Checked(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetGameInstance_Checked(const UObject* ContextObject);

	template<typename UserClass>
	[[nodiscard]] static UserClass* GetWorldSettings_Checked(const UObject* ContextObject);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetPlayerController_Checked(const UObject* ContextObject, int32 PlayerIndex);

	template <typename UserClass>
	[[nodiscard]] static UserClass* GetLocalPlayer_Checked(const UObject* ContextObject, int32 PlayerIndex);
#pragma endregion
#pragma endregion

#pragma region SaveGame
	template <typename UserClass>
	[[nodiscard]] static UserClass* CreateSaveGameObject();

	template <typename UserClass>
	[[nodiscard]] static UserClass* LoadGameFromSlot(const FString& SlotName, const int32 UserIndex);
#pragma endregion

#pragma region Misc
	template <typename UserClass>
	[[nodiscard]] static TArray<TWeakObjectPtr<UserClass>> ToWeakObjectPtrArray(const TArray<UserClass*>& Array);

	template <typename UserClass>
	[[nodiscard]] static TArray<TWeakObjectPtr<const UserClass>> ToWeakObjectPtrArray(
		const TArray<const UserClass*>& Array);

	template <typename UserClass>
	[[nodiscard]] static TArray<UserClass*> FromWeakObjectPtrArray(const TArray<TWeakObjectPtr<UserClass>>& Array);

	template <typename UserClass>
	[[nodiscard]] static TArray<const UserClass*> FromWeakObjectPtrArray(
		const TArray<TWeakObjectPtr<const UserClass>>& Array);

	template <typename UserClass>
	[[nodiscard]] static UserClass GetArrayElem(const TArray<UserClass>& Array, int32 Index, UserClass ReturnIfEmpty);

	template<typename UserClass>
	[[nodiscard]] static UserClass GetArrayElemUnsafe(const TArray<UserClass>& Array, int32 Index);

	template <typename UserClass>
	[[nodiscard]] static int32 GetHighestIndex(const TArray<UserClass>& Array, int32 Index);
#pragma endregion
#pragma endregion
};

#pragma region C++
#pragma region TypedGetters
template<typename UserClass>
UserClass* UTCU_Library::GetGameMode(const UObject* ContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return nullptr;
	}

	AGameModeBase* BaseGameMode = World->GetAuthGameMode();
	if (!IsValid(BaseGameMode))
	{
		return nullptr;
	}

	auto* TypedGameMode = Cast<UserClass>(BaseGameMode);
	return TypedGameMode;
}

template<typename UserClass>
UserClass* UTCU_Library::GetGameState(const UObject* ContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return nullptr;
	}

	auto* TypedGameState = World->GetGameState<UserClass>();
	return TypedGameState;
}

template<typename UserClass>
UserClass* UTCU_Library::GetGameSession(const UObject* ContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return nullptr;
	}

	AGameModeBase* BaseGameMode = World->GetAuthGameMode();
	if (!IsValid(BaseGameMode))
	{
		return nullptr;
	}

	auto* TypedGameSession = Cast<UserClass>(BaseGameMode->GameSession);
	return TypedGameSession;
}

template<typename UserClass>
UserClass* UTCU_Library::GetGameInstance(const UObject* ContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return nullptr;
	}

	auto* TypedGameInstance = World->GetGameInstance<UserClass>();
	return TypedGameInstance;
}

template<typename UserClass>
UserClass* UTCU_Library::GetWorldSettings(const UObject* ContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return nullptr;
	}

	const AWorldSettings* WorldSettings = World->GetWorldSettings();
	check(IsValid(WorldSettings));

	auto* TypedWorldSettings = Cast<UserClass>(WorldSettings);
	return TypedWorldSettings;
}

template<typename UserClass>
UserClass* UTCU_Library::GetPlayerController(const UObject* ContextObject, int32 PlayerIndex)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex);

	auto* TypedPlayerController = Cast<UserClass>(Controller);
	return TypedPlayerController;
}

template<typename UserClass>
UserClass* UTCU_Library::GetLocalPlayer(const UObject* ContextObject, int32 PlayerIndex)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(ContextObject, PlayerIndex);

	if (!IsValid(Controller))
	{
		return nullptr;
	}

	ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();

	auto* TypedLocalPlayer = Cast<UserClass>(LocalPlayer);
	return TypedLocalPlayer;
}

#pragma region Checked
template<typename UserClass>
UserClass* UTCU_Library::GetGameMode_Checked(const UObject* ContextObject)
{
	auto* TypedGameMode = GetGameMode<UserClass>(ContextObject);
	check(IsValid(TypedGameMode));

	return TypedGameMode;
}

template<typename UserClass>
UserClass* UTCU_Library::GetGameState_Checked(const UObject* ContextObject)
{
	auto* TypedGameState = GetGameState<UserClass>(ContextObject);
	check(IsValid(TypedGameState));

	return TypedGameState;
}

template<typename UserClass>
UserClass* UTCU_Library::GetGameSession_Checked(const UObject* ContextObject)
{
	auto* TypedGameSession = GetGameSession<UserClass>(ContextObject);
	check(IsValid(TypedGameSession));

	return TypedGameSession;
}

template<typename UserClass>
UserClass* UTCU_Library::GetGameInstance_Checked(const UObject* ContextObject)
{
	auto* TypedGameInstance = GetGameInstance<UserClass>(ContextObject);
	check(IsValid(TypedGameInstance));

	return TypedGameInstance;
}

template<typename UserClass>
UserClass* UTCU_Library::GetWorldSettings_Checked(const UObject* ContextObject)
{
	auto* TypedWorldSettings = GetWorldSettings<UserClass>(ContextObject);
	check(IsValid(TypedWorldSettings));

	return TypedWorldSettings;
}

template<typename UserClass>
UserClass* UTCU_Library::GetPlayerController_Checked(const UObject* ContextObject, int32 PlayerIndex)
{
	auto* TypedPlayerController = GetPlayerController<UserClass>(ContextObject);
	check(IsValid(TypedPlayerController));

	return TypedPlayerController;
}

template<typename UserClass>
UserClass* UTCU_Library::GetLocalPlayer_Checked(const UObject* ContextObject, int32 PlayerIndex)
{
	auto* TypedLocalPlayer = GetLocalPlayer<UserClass>(ContextObject);
	check(IsValid(TypedLocalPlayer));

	return TypedLocalPlayer;
}
#pragma endregion
#pragma endregion

#pragma region SaveGame
template<typename UserClass>
UserClass* UTCU_Library::CreateSaveGameObject()
{
	auto* SaveGame = NewObject<UserClass>(GetTransientPackage(), UserClass::StaticClass());
	return SaveGame;
}

template<typename UserClass>
UserClass* UTCU_Library::LoadGameFromSlot(const FString& SlotName, const int32 UserIndex)
{
	USaveGame* GameSlot = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	if (!IsValid(GameSlot))
	{
		return nullptr;
	}

	auto* TypedGameSlot = CastChecked<UserClass>(GameSlot);
	return TypedGameSlot;
}

template<typename UserClass>
TArray<TWeakObjectPtr<UserClass>> UTCU_Library::ToWeakObjectPtrArray(const TArray<UserClass*>& Array)
{
	TArray<TWeakObjectPtr<UserClass>> ReturnValue;
	ReturnValue.Reserve(Array.Num());

	for (UserClass* Element : Array)
	{
		ReturnValue.Add(Element);
	}

	return ReturnValue;
}

template<typename UserClass>
TArray<TWeakObjectPtr<const UserClass>> UTCU_Library::ToWeakObjectPtrArray(const TArray<const UserClass*>& Array)
{
	TArray<TWeakObjectPtr<const UserClass>> ReturnValue;
	ReturnValue.Reserve(Array.Num());

	for (UserClass* Element : Array)
	{
		ReturnValue.Add(Element);
	}

	return ReturnValue;
}

template<typename UserClass>
TArray<UserClass*> UTCU_Library::FromWeakObjectPtrArray(const TArray<TWeakObjectPtr<UserClass>>& Array)
{
	TArray<UserClass*> ReturnValue;
	ReturnValue.Reserve(Array.Num());

	for (TWeakObjectPtr<UserClass> Element : Array)
	{
		ReturnValue.Add(Element.Get());
	}

	return ReturnValue;
}

template<typename UserClass>
TArray<const UserClass*> UTCU_Library::FromWeakObjectPtrArray(const TArray<TWeakObjectPtr<const UserClass>>& Array)
{
	TArray<const UserClass*> ReturnValue;
	ReturnValue.Reserve(Array.Num());

	for (TWeakObjectPtr<const UserClass> Element : Array)
	{
		ReturnValue.Add(Element.Get());
	}

	return ReturnValue;
}

template<typename UserClass>
UserClass UTCU_Library::GetArrayElem(const TArray<UserClass>& Array, int32 Index, UserClass ReturnIfEmpty)
{
	return !Array.IsEmpty() ? GetArrayElemUnsafe(Array, Index) : ReturnIfEmpty;
}

template<typename UserClass>
UserClass UTCU_Library::GetArrayElemUnsafe(const TArray<UserClass>& Array, int32 Index)
{
	return Array[GetHighestIndex(Array, Index)];
}

template<typename UserClass>
int32 UTCU_Library::GetHighestIndex(const TArray<UserClass>& Array, int32 Index)
{
	return FMath::Clamp(Index, 0, Array.Num() - 1);
}
#pragma endregion
#pragma endregion