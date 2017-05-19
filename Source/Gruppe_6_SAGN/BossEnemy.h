// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BossEnemy.generated.h"

UCLASS()
class GRUPPE_6_SAGN_API ABossEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float DeltaTime);
	void RotateToPlayer();

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
		UShapeComponent * CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACurvingBossBullet> CurvingBossBullet_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AStandardEnemyProjectile> StandardEnemyProjectile_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ASpinningMeleeEnemyAttack> SpinningMeleeEnemyAttack_BP;

	void SpawnBulletWave(float DeltaTime);
	void SpawnBulletStream(float DeltaTime);
	void IsBossDeadFunction(int &number);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float MaxHealth = 100;

private:

	float MovementValue = 20.0f;
	float BulletWaveTimer;
	float NewMode;
	float StreamDelay;
	float AddYawToStream;
	float AddInvertedYawToStream;
	float StopStream;
	float EnrageAttack1;
	float EnrageAttack2;

	int BossMode = 1;
	int WavesSpawned;
	int NewModeArray[2] = { 1,3 };
	int IsBossDead = 0;

	bool bIsEnraged = false;
};