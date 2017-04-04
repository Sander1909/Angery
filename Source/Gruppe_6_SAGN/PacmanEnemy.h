// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PacmanEnemy.generated.h"

UCLASS()
class GRUPPE_6_SAGN_API APacmanEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacmanEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void RotateToPlayer();
	void SpawnStaticProjectile(float DeltaTime);
	void SpawnPowerUp();

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AStaticProjectile> StaticProjectile_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AP_Up_BulletRain> P_Up_BulletRain_BP;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
		UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
		UShapeComponent * CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int Health = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float MaxHealth = 2;

private:

	bool bHitByMelee = false;
	bool bHitByProjectile = false;

	float HitByMeleeTimer;
	float HitByProjectileTimer;
	float MovementValue = 200.0f;

	int MovementMode = rand() %4+1;

	float SwitchMode;
	float SpawnTimer;
	
	int PowerUpRoll = 0;
	int PowerUpProbability = 70;
	int MaxPowerUpTypes = 3;

	int Mode1[2] = {3,4};
	int Mode2[2] = {3,4};
	int Mode3[2] = {1,2};
	int Mode4[2] = {1,2};
	
};
