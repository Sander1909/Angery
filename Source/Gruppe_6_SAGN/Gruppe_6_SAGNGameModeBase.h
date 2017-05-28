// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BossEnemy.h"
#include "GameFramework/GameModeBase.h"
#include "Gruppe_6_SAGNGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GRUPPE_6_SAGN_API AGruppe_6_SAGNGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGruppe_6_SAGNGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void ShowVictoryHUD();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void NextWaveAnnouncement();

	class ABossEnemy * BossPtr = nullptr;


	UPROPERTY()
		UWorld * World;

	UFUNCTION(BlueprintCallable, Category = "testing")
		void debugFunction();

private:


	
};
