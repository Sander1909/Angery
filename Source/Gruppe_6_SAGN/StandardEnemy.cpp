// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "StandardEnemy.h"
#include "StandardEnemyProjectile.h"
#include "PlayerProjectile.h"
#include "PlayerMeleeAttack.h"
#include "P_Up_BulletRain.h"
#include "P_Up_FullHealth.h"

AStandardEnemy::AStandardEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AStandardEnemy::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStandardEnemy::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StandardEnemy no collision box"));

	}
}

void AStandardEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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

	if (!bHitByMelee)
	{
		switch (EnemyMode)
		{
		case 1:
			MoveForward(DeltaTime);
			RotateToPlayer();
			break;
		case 2:
			SpawnProjectile(DeltaTime);
			RotateToPlayer();
			break;
		}
	}
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

void AStandardEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AStandardEnemy::MoveForward(float DeltaTime)
{
	FVector ForwardVector = GetActorForwardVector() * DeltaTime;

	if (!bHitByProjectile)
	{
		AddMovementInput(ForwardVector, MovementValue);
	}
	else
	{
		AddMovementInput(-ForwardVector, MovementValue);
	}
}

void AStandardEnemy::RotateToPlayer()
{
	
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - GetActorLocation();

	SetActorRotation(NewDirection.Rotation());

	if (NewDirection.Size() <= ShootRange)
	{
		EnemyMode = 2;
	}

}

void AStandardEnemy::SpawnProjectile(float DeltaTime)
{
	UWorld * World;

	World = GetWorld();

	FVector Location = GetActorLocation();
	Location.Z = 10.0f;

	ShootTimer += DeltaTime;

	if (ShootTimer >= 1.5f)
	{
		World->SpawnActor<AStandardEnemyProjectile>(StandardEnemyProjectile_BP, Location, GetActorRotation());
		ShootTimer = 0.0f;
	}


	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - GetActorLocation();

	if (NewDirection.Size() > ShootRange)
	{
		EnemyMode = 1;
	}

}

void AStandardEnemy::SpawnPowerUp()
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
		MaxPowerUpTypes = rand() % 2 + 1;
		switch (MaxPowerUpTypes)
		{
		case 1:

			World->SpawnActor<AP_Up_BulletRain>(P_Up_BulletRain_BP, Location, FRotator::ZeroRotator);
			break;
			
		case 2:

			World->SpawnActor<AP_Up_FullHealth>(P_Up_FullHealth_BP, Location, FRotator::ZeroRotator);
			break;

		case 3:

			//Spawn neste PowerUp.
			break;

		default:
			break;
		}
	}
}

void AStandardEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
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

		//UE_LOG(LogTemp, Warning, TEXT("StandardEnemy health is: %i"), Health);
	}
	else if (OtherActor->IsA(APlayerMeleeAttack::StaticClass()))
	{
		//Health--;
		if (Health < 1)
		{
			Destroy();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Standard Enemy was hit by PlayerMeleeAttack"));
		bHitByMelee = true;
		HitByMeleeTimer = 0.0f;

	}
}