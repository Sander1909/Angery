// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AStandardEnemy> StandardEnemy_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ASpinningMeleeEnemy> SpinningMeleeEnemy_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APacmanEnemy> PacmanEnemy_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ARandomEnemy> RandomEnemy_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AStrayEnemy> StrayEnemy_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ABossEnemy> BossEnemy_BP;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* OnNextWaveSound;

	void SpawnStandardEnemy();
	void SpawnPacmanEnemy();
	void SpawnRandomEnemy();
	void SpawnStrayEnemy();
	void SpawnBossEnemy();

	UPROPERTY()
		UWorld * World;

private:

	float SpawnValues[2]{ 2000.0f, -2000.0f };
	float PacmanSpawnValues[2]{ 1000.0f, -1000.0f };

	bool WaveIntermission = false;

	int WaveNumber = 1;
	int EnemiesSpawned = 0;

	float WaveTimer;
	float StandardSpawnTimer;
	float StraySpawnTimer;
	float PacmanSpawnTimer;
	float SpinningMeleeSpawnTimer;
	
	
};
