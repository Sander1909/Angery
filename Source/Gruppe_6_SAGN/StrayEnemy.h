// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "StrayEnemy.generated.h"

UCLASS()
class GRUPPE_6_SAGN_API AStrayEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStrayEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void RotateToPlayer();
	void MoveForward(float DeltaTime);
	void SpawnProjectile();

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AStandardEnemyProjectile> StandardEnemyProjectile_BP;


	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
		UShapeComponent * CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int Health = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float MaxHealth = 4;

private:

	bool bHitByMelee = false;
	bool bHitByProjectile = false;

	float UpperX = 2000.0f;
	float LowerX = -2000.0f;
	float UpperY = 3800.0f;
	float LowerY = -3800.0f;

	float HitByMeleeTimer;
	float HitByProjectileTimer;
	float MovementValue = 600.0f;
	float ShootTimer;

	int PowerUpRoll = 0;
	int PowerUpProbability = 90;
	int MaxPowerUpTypes;

	
	
};
