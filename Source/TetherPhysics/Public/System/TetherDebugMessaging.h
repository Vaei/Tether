// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherStatics.h"
#include "TetherDebugMessaging.generated.h"

/*
 * Single entry of a debug text item to render. 
 *
 * @see UTetherStatics::AddDebugText(), UTetherStatics::RemoveDebugText() and UTetherStatics::DrawDebugTextList()
 */
USTRUCT()
struct TETHERPHYSICS_API FTetherDebugText
{
	GENERATED_BODY()

	FTetherDebugText()
		: Shape(nullptr)
		, WorldLocation(FVector::ZeroVector)
		, Color(FColor::Transparent)
		, bDrawShadow(false)
		, Font(nullptr)
		, FontScale(1.f)
	{}

	const FTetherShape* Shape;

	/** Position in world */
	UPROPERTY()
	FVector WorldLocation;

	/** Text to display */
	UPROPERTY()
	FString Text;

	/** Text color */
	UPROPERTY()
	FColor Color;

	/** Whether to draw a shadow for the text */
	UPROPERTY()
	uint32 bDrawShadow:1;

	/** The Font which to display this as.  Will Default to GetSmallFont()**/
	UPROPERTY()
	TObjectPtr<UFont> Font;

	/** Scale to apply to font when rendering */
	UPROPERTY()
	float FontScale;
};

/**
 * Manages debug text drawing in both game and editor contexts.
 *
 * This is necessary because DrawDebugString() from DrawDebugHelpers cannot function without a PlayerController and HUD,
 * which means it cannot work when we're simulating in editor.
 */
USTRUCT()
struct TETHERPHYSICS_API FTetherDebugTextService
{
	GENERATED_BODY()

public:
	/**
	 * Pending debug text that will be drawn
	 * Added via UTetherStatics::DrawText()
	 * Processed via UTetherStatics::ProcessText()
	 */
	UPROPERTY()
	TArray<FTetherDebugText> PendingDebugText;

protected:
	TWeakObjectPtr<UObject> WorldContext;

	FDelegateHandle GDebugDrawHandleGame;
	FDelegateHandle GDebugDrawHandleEditor;

public:
	FTetherDebugTextService()
	{}

	UWorld* GetWorld() const;

	/** 
	 * Initializes the debug draw service for both game and editor worlds (if specified).
	 * Registers the appropriate debug draw delegate based on the context.
	 *
	 * Consider calling this from BeginPlay()
	 * 
	 * @param InWorldContext The context object that provides access to the world
	 * @param bDrawGame Whether to register the debug draw service for the game world
	 * @param bDrawEditor Whether to register the debug draw service for the editor world
	 */
	void Initialize(UObject* InWorldContext, bool bDrawGame = true, bool bDrawEditor = true);

	/** 
	 * Deinitializes the debug draw service, unregistering any registered delegates.
	 * This ensures that the debug draw services are cleaned up and no longer used.
	 *
	 * Consider calling this from EndPlay()
	 */
	void Deinitialize();
};

UENUM()
enum class ETetherMessageLogSeverity : uint8
{
	Log,
	Warning,
	Error,
};

UENUM()
enum class ETetherMessageLogType : uint8
{
	FMessageLog,
	UE_LOG,
};

USTRUCT()
struct TETHERPHYSICS_API FTetherMessageLogEntry
{
	GENERATED_BODY()

	FTetherMessageLogEntry(const FString& InMessage = "", ETetherMessageLogSeverity InSeverity = ETetherMessageLogSeverity::Log, ETetherMessageLogType InLogType = ETetherMessageLogType::FMessageLog, bool bInUniqueMessage = true)
		: Message(InMessage)
		, Severity(InSeverity)
		, LogType(InLogType)
		, bUniqueMessage(bInUniqueMessage)
	{}
	
	UPROPERTY()
	FString Message;

	UPROPERTY()
	ETetherMessageLogSeverity Severity;

	UPROPERTY()
	ETetherMessageLogType LogType;
	
	UPROPERTY()
	bool bUniqueMessage;

	bool operator==(const FTetherMessageLogEntry& Other) const
	{
		return !Message.IsEmpty() && Message.Equals(Other.Message) && Severity == Other.Severity;
	}
};

FORCEINLINE uint32 GetTypeHash(const FTetherMessageLogEntry& Entry)
{
	// Generate hash to allow FTetherMessageLogEntry as TMap Key
	return HashCombine(GetTypeHash(Entry.Message), GetTypeHash(static_cast<uint8>(Entry.Severity)));
}

/**
 * FMessageLog and UE_LOG handler
 * Primarily exists to avoid printing unique messages repeatedly on tick
 */
USTRUCT()
struct TETHERPHYSICS_API FTetherMessageLog
{
	GENERATED_BODY()

	FTetherMessageLog()
		: PendingMessageLogs({})
		, UniqueMessageLogs({})
	{}
	
	void AddPendingMessage(const FTetherMessageLogEntry& LogEntry, const FString& LogCategory = "PIE")
	{
		if (!UniqueMessageLogs.Contains(LogEntry.Message))
		{
			PendingMessageLogs.Add(LogEntry, LogCategory);

			if (LogEntry.bUniqueMessage)
			{
				UniqueMessageLogs.Add(LogEntry.Message);
			}
		}
	}

	void PrintMessages()
	{
		for (auto& PendingLogs : GetPendingMessageLogs())
		{
			const FTetherMessageLogEntry& LogEntry = PendingLogs.Key;

			switch (LogEntry.LogType)
			{
			case ETetherMessageLogType::FMessageLog:
				{
					FMessageLog MessageLog { *PendingLogs.Value };
					FText MessageText = FText::FromString(PendingLogs.Key.Message);
					switch (LogEntry.Severity)
					{
					case ETetherMessageLogSeverity::Log:
						MessageLog.Info(MessageText);
						break;
					case ETetherMessageLogSeverity::Warning:
						MessageLog.Warning(MessageText);
						break;
					case ETetherMessageLogSeverity::Error:
						MessageLog.Error(MessageText);
						break;
					}
				}
				break;
			case ETetherMessageLogType::UE_LOG:
				{
					switch (LogEntry.Severity)
					{
					case ETetherMessageLogSeverity::Log:
						UE_LOG(LogTether, Log, TEXT("%s"), *LogEntry.Message);
						break;
					case ETetherMessageLogSeverity::Warning:
						UE_LOG(LogTether, Warning, TEXT("%s"), *LogEntry.Message);
						break;
					case ETetherMessageLogSeverity::Error:
						UE_LOG(LogTether, Error, TEXT("%s"), *LogEntry.Message);
						break;
					}
				}
				break;
			}
		}

		ResetPendingMessageLogs();
	}

	const TMap<FTetherMessageLogEntry, FString>& GetPendingMessageLogs() const { return PendingMessageLogs; }
	
	void ResetUniqueMessageLogs() { UniqueMessageLogs.Reset(); }
	void EmptyUniqueMessageLogs() { UniqueMessageLogs.Empty(); }
	
	void ResetPendingMessageLogs() { PendingMessageLogs.Reset(); }
	void EmptyPendingMessageLogs() { PendingMessageLogs.Empty(); }

	void ResetMessageLogs() { ResetUniqueMessageLogs(); ResetPendingMessageLogs(); }
	void EmptyMessageLogs() { EmptyUniqueMessageLogs(); EmptyPendingMessageLogs(); }

private:
	/**
	 * Key: MessageLogEntry
	 * Value: Log Category
	 */
	UPROPERTY()
	TMap<FTetherMessageLogEntry, FString> PendingMessageLogs;

	/** Avoid repeating already printed messages */
	UPROPERTY()
	TArray<FString> UniqueMessageLogs;
};