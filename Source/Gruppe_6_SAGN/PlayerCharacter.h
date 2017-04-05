// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerHit);

UCLASS()
class GRUPPE_6_SAGN_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Shoot();
	void StopShoot();
	void Melee();

	void MoveX(float Value);
	void MoveY(float Value);

	void SetPlayerRotation();

	void SpawnBulletRain();
	void CharacterFullHealth();
	void BoostFireRate();

	UPROPERTY(BlueprintAssignable)
		FOnPlayerHit OnPlayerHit;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
		UShapeComponent * CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APlayerProjectile> PlayerProjectile_BP;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class APlayerMeleeAttack> PlayerMeleeAttack_BP;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraShake")
		void StartCameraShake();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraShake")
		void StartMinorCameraShake();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int Health = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float MaxHealth = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* OnPlayerShootSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* OnPlayerHitSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* OnPowerUpSound;

private:

	float MeleeDashTimer;
	float Speed = 1000.0f;
	float ShootTimer;
	float FireRate = 0.2f;
	float FireRateTimer;
	float IFrameTimer;

	int Width = 2450;
	int Heigth = 2050;

	bool bIsDead = false;
	bool bMeleeDash = false;
	bool bIsShooting = false;
	bool bIsFireRate = false;
	bool bIFrame = false;

};
