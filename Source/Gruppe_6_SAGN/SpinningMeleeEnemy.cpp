// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "SpinningMeleeEnemy.h"
#include "PlayerMeleeAttack.h"
#include "PlayerProjectile.h"
#include "SpinningMeleeEnemyAttack.h"
#include "P_Up_BulletRain.h"
#include "P_Up_FullHealth.h"
#include "P_Up_FireRate.h"
#include "P_Up_CurvingBullet.h"


// Sets default values
ASpinningMeleeEnemy::ASpinningMeleeEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpinningMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpinningMeleeEnemy::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpinningMeleeEnemy no collision box"));

	}

}

// Called every frame
void ASpinningMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Move is %f"), MovementValue);


	SwitchModeTimer += DeltaTime;

	if (bHitByProjectile)
	{
		MoveForward(DeltaTime);
		HitByProjectileTimer += DeltaTime;
		if (HitByProjectileTimer > 0.3f)
		{
			HitByProjectileTimer = 0.0f;
			bHitByProjectile = false;
		}
	}

	//Default mode guard.
	if (!bHitByMelee)
	{
		switch (EnemyMode)
		{
		case 1:

			MoveForward(DeltaTime);
			SetEnemyRotation();

			if (SwitchModeTimer > 3.5f)
			{
				EnemyMode = 2;
			}

			break;

		case 2:

			SpawnAttack(DeltaTime);

		}

	}
	//If the enemy is hit by MeleeAttack, act accordingly.
	else if (bHitByMelee)
	{
		HitByMeleeTimer += DeltaTime;

		if (HitByMeleeTimer < 0.5f)
		{
			AddMovementInput(GetActorLocation() - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), MovementValue);
		}
		else if (HitByMeleeTimer >= 0.5f)
		{
			if (HitByMeleeTimer > 1.5f)
			{
				HitByMeleeTimer = 0.0f;
				bHitByMelee = false;
			}
		}
	}

}

void ASpinningMeleeEnemy::MoveForward(float DeltaTime)
{
	FVector ForwardVector = GetActorForwardVector() * DeltaTime;
	if (!bHitByProjectile)
	{
		AddMovementInput(ForwardVector, MovementValue);
	}
	else
	{
		FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		FVector NewDirection = PlayerLocation - GetActorLocation();

		AddMovementInput(-NewDirection, 300.0f);
	}
}

void ASpinningMeleeEnemy::SetEnemyRotation()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - GetActorLocation();

	NewDirection.Z = 0.0f;

	SetActorRotation(NewDirection.Rotation());
}

void ASpinningMeleeEnemy::SpawnAttack(float DeltaTime)
{

	SwitchModeTimer += DeltaTime;
	SpawnAttackTimer += DeltaTime;

	UWorld * World;
	World = GetWorld();

	FRotator ActorRotation = GetActorRotation() + FRotator(0.0f, 3.2f, 0.0f);
	SetActorRotation(ActorRotation);

	FVector Location = GetActorLocation();
	Location.Z = 100.0f;

	if (SpawnAttackTimer > 0.3f)
	{
		SpawnAttackTimer = 0.0f;

		World->SpawnActor<ASpinningMeleeEnemyAttack>(SpinningMeleeEnemyAttack_BP, Location + GetActorForwardVector() * 300.0f, FRotator::ZeroRotator);
	}


	if (SwitchModeTimer > 7.0f)
	{
		SwitchModeTimer = 0.0f;
		EnemyMode = 1;
	}
}

void ASpinningMeleeEnemy::SpawnPowerUp()
{
	UWorld * World;

	World = GetWorld();

	FVector Location = GetActorLocation();
	Location.Z = 100.0f;

	//FRotator P_Up_Rotation = FRotator(45.0f, 45.0f, 45.0f);

	PowerUpRoll = rand() % 100 + 1;
	UE_LOG(LogTemp, Warning, TEXT("Powerup roll is: %i"), PowerUpRoll);
	if (PowerUpRoll > PowerUpProbability)
	{
		MaxPowerUpTypes = rand() % 4 + 1;
		switch (MaxPowerUpTypes)
		{
		case 1:

			World->SpawnActor<AP_Up_BulletRain>(P_Up_BulletRain_BP, Location, FRotator::ZeroRotator);
			break;

		case 2:

			World->SpawnActor<AP_Up_FullHealth>(P_Up_FullHealth_BP, Location, FRotator::ZeroRotator);
			break;

		case 3:

			World->SpawnActor<AP_Up_FireRate>(P_Up_FireRate_BP, Location, FRotator::ZeroRotator);
			break;

		case 4:

			World->SpawnActor<AP_Up_CurvingBullet>(P_Up_CurvingBullet_BP, Location, FRotator::ZeroRotator);
			break;

		default:
			break;
		}
	}
}

void ASpinningMeleeEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerProjectile::StaticClass()))
	{
		Health--;
		bHitByProjectile = true;
		if (Health < 1)
		{
			SpawnPowerUp();
			Destroy();
		}
		OtherActor->Destroy();
	}

	else if (OtherActor->IsA(APlayerMeleeAttack::StaticClass()) && !bHitByMelee)
	{
		Health--;
		if (Health < 1)
		{
			Destroy();
		}

		bHitByMelee = true;
		HitByMeleeTimer = 0.0f;

	}
}