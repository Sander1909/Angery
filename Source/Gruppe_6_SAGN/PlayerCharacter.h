// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerProjectile.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

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

	//See Category for what UPROPERTY is used for.

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

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AIFrameShield> IFrameShield_BP;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraShake")
		void StartCameraShake();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CameraShake")
		void StartMinorCameraShake();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void FireRateAnnouncement();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void CurvingBulletAnnouncement();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void BulletStormAnnouncement();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void FullRageAnnouncement();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void ShowDeadHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		float CurvingBulletTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		float FireRateTimer;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* OnPlayerShootSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* OnPlayerHitSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* OnPowerUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
		bool bMeleeDash = false;

	class APlayerProjectile* PlayerProjectilePtr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
		bool bIsShooting = false;

private:

	float MeleeDashTimer;
	float MeleeDelayTimer;
	float Speed = 1000.0f;
	float ShootTimer;
	float FireRate = 0.3f;
	float IFrameTimer;

	int Width = 3900;
	int Heigth = 2050;

	bool bIsDead = false;
	bool bMeleeDelay = false;

	bool bIsFireRate = false;
	bool bIFrame = false;
	bool bCurvingBullet = false;

};
