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

	void SpawnStaticProjectile(float DeltaTime);
	void SpawnPowerUp();

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AStaticProjectile> StaticProjectile_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AP_Up_BulletRain> P_Up_BulletRain_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AP_Up_FullHealth> P_Up_FullHealth_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AP_Up_FireRate> P_Up_FireRate_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AP_Up_CurvingBullet> P_Up_CurvingBullet_BP;

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

	float SwitchTimer;
	float SpawnTimer;
	
	//Coordinates for spawning.
	float UpperX = 2000.0f;
	float LowerX = -2000.0f;
	float UpperY = 3800.0f;
	float LowerY = -3800.0f;

	int PowerUpRoll;
	int PowerUpProbability = 90;
	int MaxPowerUpTypes;

	//Used to decide new direction.
	int Mode1[3] = {1,3,4};
	int Mode2[3] = {2,3,4};
	int Mode3[3] = {3,1,2};
	int Mode4[3] = {4,1,2};
	
};
