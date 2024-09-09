// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherDrawing.h"
#include "TetherStatics.h"
#include "TetherMessaging.generated.h"

/*
 * Single entry of a debug text item to render. 
 *
 * @see UTetherStatics::DrawText(), UTetherStatics::ProcessText()
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

/** Severity level for logging */
UENUM()
enum class ETetherMessageLogSeverity : uint8
{
	Log,
	Warning,
	Error,
};

/** Whether to use FMessageLog or UE_LOG */
UENUM()
enum class ETetherMessageLogType : uint8
{
	FMessageLog,
	UE_LOG,
};

/**
 * A structure that holds information about a message log entry.
 * It contains the message, its severity, log type, and whether it's a unique message.
 */
USTRUCT()
struct TETHERPHYSICS_API FTetherMessageLogEntry
{
	GENERATED_BODY()

	/** 
	 * @param InMessage The message string.
	 * @param InSeverity The severity level of the message.
	 * @param InLogType The log type (FMessageLog or UE_LOG).
	 * @param bInUniqueMessage If true, avoids logging the same message repeatedly.
	 */
	FTetherMessageLogEntry(const FString& InMessage = "", ETetherMessageLogSeverity InSeverity = ETetherMessageLogSeverity::Log, ETetherMessageLogType InLogType = ETetherMessageLogType::FMessageLog, bool bInUniqueMessage = true)
		: Message(InMessage)
		, Severity(InSeverity)
		, LogType(InLogType)
		, bUniqueMessage(bInUniqueMessage)
	{}

	/** The message to be logged */
	UPROPERTY()
	FString Message;

	/** The severity of the message (Log, Warning, Error) */
	UPROPERTY()
	ETetherMessageLogSeverity Severity;

	/** The type of logging to use (FMessageLog, UE_LOG) */
	UPROPERTY()
	ETetherMessageLogType LogType;

	/** If true, ensures the message is logged only once */
	UPROPERTY()
	bool bUniqueMessage;

	bool operator==(const FTetherMessageLogEntry& Other) const
	{
		return !Message.IsEmpty() && Message.Equals(Other.Message) && Severity == Other.Severity;
	}
};

/**
 * Generates a hash value for the FTetherMessageLogEntry to allow it to be used as a key in TMap.
 * @param Entry The FTetherMessageLogEntry to hash.
 * @return The hash value of the message log entry.
 */
FORCEINLINE uint32 GetTypeHash(const FTetherMessageLogEntry& Entry)
{
	// Generate hash to allow FTetherMessageLogEntry as TMap Key
	return HashCombine(GetTypeHash(Entry.Message), GetTypeHash(static_cast<uint8>(Entry.Severity)));
}

/**
 * FMessageLog and UE_LOG handler.
 * This struct manages logging messages either via FMessageLog or UE_LOG.
 * Primarily exists to avoid printing unique messages repeatedly, e.g., on every tick.
 */
USTRUCT()
struct TETHERPHYSICS_API FTetherMessageLog
{
	GENERATED_BODY()

	FTetherMessageLog()
		: PendingMessageLogs({})
		, UniqueMessageLogs({})
	{}

	/**
	 * Adds a pending message to the message log.
	 * Avoids logging messages repeatedly if bUniqueMessage is true.
	 * @param LogEntry The message log entry to add.
	 * @param LogCategory The log category to associate with the entry (default: "PIE").
	 */
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

	/**
	 * Prints all pending messages to the log, using either FMessageLog or UE_LOG.
	 * Once printed, the pending message logs are cleared.
	 */
	void ProcessMessages()
	{
		// Iterate through the pending message logs
		for (auto& PendingLogs : GetPendingMessageLogs())
		{
			const FTetherMessageLogEntry& LogEntry = PendingLogs.Key;

			// Print the message based on its log type
			switch (LogEntry.LogType)
			{
			case ETetherMessageLogType::FMessageLog:
				{
					FMessageLog MessageLog { *PendingLogs.Value };
					FText MessageText = FText::FromString(PendingLogs.Key.Message);
					
					// Print the message based on its severity
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
					// Log the message using UE_LOG based on its severity
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

		// Reset the pending message logs after printing
		ResetPendingMessageLogs();
	}

	/** 
	 * Retrieves the map of pending message logs.
	 * @return A reference to the map of pending message logs.
	 */
	const TMap<FTetherMessageLogEntry, FString>& GetPendingMessageLogs() const { return PendingMessageLogs; }

	// Clear unique message logs, allowing them to be re-printed
	
	void ResetUniqueMessageLogs() { UniqueMessageLogs.Reset(); }
	void EmptyUniqueMessageLogs() { UniqueMessageLogs.Empty(); }

	// Clear pending message logs, preventing them from being processed (again, potentially)
	
	void ResetPendingMessageLogs() { PendingMessageLogs.Reset(); }
	void EmptyPendingMessageLogs() { PendingMessageLogs.Empty(); }

	// Clear all message logs for a blank slate
	
	void ResetMessageLogs() { ResetUniqueMessageLogs(); ResetPendingMessageLogs(); }
	void EmptyMessageLogs() { EmptyUniqueMessageLogs(); EmptyPendingMessageLogs(); }

private:
	/** Map that stores pending message logs, associating each entry with a log category */
	UPROPERTY()
	TMap<FTetherMessageLogEntry, FString> PendingMessageLogs;
	
	/** Array that holds unique messages to avoid logging the same message repeatedly */
	UPROPERTY()
	TArray<FString> UniqueMessageLogs;
};