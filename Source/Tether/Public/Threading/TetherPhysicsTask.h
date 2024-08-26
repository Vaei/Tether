// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTask.h"

#include "TetherPhysicsTask.generated.h"

class UTetherActorComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTetherPhysicsTaskUpdate, float, DeltaTime, bool, bTemp);

/**
 * @TODO
 * Updates on GameThread
 */
UCLASS()
class TETHER_API UTetherPhysicsTask : public UGameplayTask
{
	GENERATED_BODY()

public:
	UTetherPhysicsTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * @param TaskPriority Controls how this task will be treated in relation to other, already running tasks, provided GameplayTasksComponent is configured to care about priorities (the default behavior)
	 */
	UFUNCTION(BlueprintCallable, Category=Tether, meta=(DefaultToSelf="InTaskOwner", BlueprintInternalUseOnly="TRUE", DisplayName="Tether Physics Update"))
	static UTetherPhysicsTask* TetherPhysicsTask(UTetherActorComponent* TaskOwner, FGameplayTag LinearSolver, FGameplayTag AngularSolver, uint8 TaskPriority = 200);

	UFUNCTION(BlueprintCallable, Category=Tether)
	void SetLinearSolver(FGameplayTag LinearSolver);

	UFUNCTION(BlueprintCallable, Category=Tether)
	void SetAngularSolver(FGameplayTag AngularSolver);

	UFUNCTION(BlueprintCallable, Category=Tether)
	void PausePhysics() { Pause(); }

	UFUNCTION(BlueprintCallable, Category=Tether)
	void ResumePhysics() { Resume(); }

	// virtual void Activate() override;
	
protected:
	FGameplayTag LinearSolverTag;
	FGameplayTag AngularSolverTag;

protected:
	UPROPERTY(BlueprintAssignable)
	FTetherPhysicsTaskUpdate PhysicsUpdate;
};