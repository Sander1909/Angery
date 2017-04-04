// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "StrayEnemy.h"
#include "StandardEnemyProjectile.h"
#include "PlayerMeleeAttack.h"
#include "PlayerProjectile.h"
#include "P_Up_BulletRain.h"
#include "P_Up_FullHealth.h"


// Sets default values
AStrayEnemy::AStrayEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStrayEnemy::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStrayEnemy::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StrayEnemy no collision box"));

	}
	
}

// Called every frame
void AStrayEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bHitByProjectile)
	{
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
		ShootTimer += DeltaTime;

		MoveForward(DeltaTime);

		if (ShootTimer > 3.5f)
		{
			SpawnProjectile();

			RotateToPlayer();

			ShootTimer = 0.0;
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

void AStrayEnemy::RotateToPlayer()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - GetActorLocation();

	SetActorRotation(NewDirection.Rotation());
}

void AStrayEnemy::MoveForward(float DeltaTime)
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - GetActorLocation();

	FVector ForwardVector = GetActorForwardVector() * DeltaTime;
	if (!bHitByProjectile)
	{
		AddMovementInput(ForwardVector, MovementValue);
	}
	else
	{
		AddMovementInput(-NewDirection, MovementValue);
	}
}

void AStrayEnemy::SpawnProjectile()
{
	FVector Location = GetActorLocation();
	Location.Z = 10.0f;
	FRotator ProjectileRotation = GetActorRotation();
	FRotator AddYaw = FRotator(0.0f, 30.0f, 0.0f);

	UWorld * World;

	World = GetWorld();

	for (int i = 0; i < 12; i++)
	{
		World->SpawnActor<AStandardEnemyProjectile>(StandardEnemyProjectile_BP, Location, ProjectileRotation);
		ProjectileRotation = ProjectileRotation + AddYaw;
	}

}

void AStrayEnemy::SpawnPowerUp()
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

void AStrayEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
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
	else if (OtherActor->IsA(APlayerMeleeAttack::StaticClass()))
	{
		//Health--;
		if (Health < 1)
		{
			Destroy();
		}

		//UE_LOG(LogTemp, Warning, TEXT("StrayEnemy was hit by PlayerMeleeAttack"));
		bHitByMelee = true;
		HitByMeleeTimer = 0.0f;

	}
}
