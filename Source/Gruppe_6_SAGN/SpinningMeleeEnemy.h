// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SpinningMeleeEnemy.generated.h"

UCLASS()
class GRUPPE_6_SAGN_API ASpinningMeleeEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpinningMeleeEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
		UShapeComponent * CollisionBox = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASpinningMeleeEnemyAttack> SpinningMeleeEnemyAttack_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AP_Up_BulletRain> P_Up_BulletRain_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AP_Up_FullHealth> P_Up_FullHealth_BP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int Health = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float MaxHealth = 4;

	void MoveForward(float DeltaTime);
	void SetEnemyRotation();
	void SpawnAttack(float DeltaTime);
	void SpawnPowerUp();


private:

	bool bHitByProjectile = false;
	bool bHitByMelee = false;
	float HitByMeleeTimer;
	float HitByProjectileTimer;
	float MovementValue = 20.0f;
	float SwitchModeTimer;
	float SpawnAttackTimer;

	int EnemyMode = 1;
	int PowerUpRoll = 0;
	int PowerUpProbability = 85;
	int MaxPowerUpTypes = 3;

};