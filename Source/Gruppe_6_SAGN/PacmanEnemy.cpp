// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "PacmanEnemy.h"
#include "StaticProjectile.h"
#include "PlayerMeleeAttack.h"
#include "PlayerProjectile.h"
#include "P_Up_BulletRain.h"
#include "P_Up_FullHealth.h"
#include "P_Up_FireRate.h"


// Sets default values
APacmanEnemy::APacmanEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacmanEnemy::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APacmanEnemy::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PacmanEnemy no collision box"));

	}

}

// Called every frame
void APacmanEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SwitchMode += DeltaTime;

	SpawnStaticProjectile(DeltaTime);

	//RotateToPlayer();

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
		switch (MovementMode)
		{
		case 1:

			if (GetActorLocation().X > 2000)
			{
				MovementMode = Mode1[rand() % 2];
				SwitchMode = 0.0f;
			}

			MoveUp();

			if (SwitchMode > 1.0f)
			{
				//Er Pacman ved høyrevegg? Gå til venstre.
				if (GetActorLocation().Y > 2000)
				{
					MovementMode = 3;
					SwitchMode = 0.0f;
				}

				//Er Pacman ved venstrevegg? Gå til høyre.
				else if (GetActorLocation().Y < -2000)
				{
					MovementMode = 4;
					SwitchMode = 0.0f;
				}

				else
				{
					MovementMode = Mode1[rand() % 2];
					SwitchMode = 0.0f;
				}
			}

			break;

		case 2:

			if (GetActorLocation().X < -2000)
			{
				MovementMode = Mode2[rand() % 2];
				SwitchMode = 0.0f;
			}

			MoveDown();

			if (SwitchMode > 1.0f)
			{
				//Er Pacman ved høyrevegg? Gå til venstre.
				if (GetActorLocation().Y > 2000)
				{
					MovementMode = 3;
					SwitchMode = 0.0f;
				}

				//Er Pacman ved venstrevegg? Gå til høyre.
				else if (GetActorLocation().Y < -2000)
				{
					MovementMode = 4;
					SwitchMode = 0.0f;
				}

				else
				{
					MovementMode = Mode2[rand() % 2];
					SwitchMode = 0.0f;
				}
			}
			break;

		case 3:

			if (GetActorLocation().Y < -2000)
			{
				MovementMode = Mode3[rand() % 2];
				SwitchMode = 0.0f;
			}

			MoveLeft();

			if (SwitchMode > 1.0f)
			{
				//Er Pacman ved øvre vegg? Gå ned.
				if (GetActorLocation().X > 2000)
				{
					MovementMode = 2;
					SwitchMode = 0.0f;
				}

				//Er Pacman ved nedre vegg? Gå opp.
				else if (GetActorLocation().X < -2000)
				{
					MovementMode = 1;
					SwitchMode = 0.0f;
				}

				else
				{
					MovementMode = Mode3[rand() % 2];
					SwitchMode = 0.0f;
				}
			}
			break;

		case 4:

			if (GetActorLocation().Y > 2000)
			{
				MovementMode = Mode4[rand() % 2];
				SwitchMode = 0.0f;
			}

			MoveRight();

			if (SwitchMode > 1.0f)
			{
				//Er Pacman ved øvre vegg? Gå ned.
				if (GetActorLocation().X > 2000)
				{
					MovementMode = 2;
					SwitchMode = 0.0f;
				}

				//Er Pacman ved nedre vegg? Gå opp.
				else if (GetActorLocation().X < -2000)
				{
					MovementMode = 1;
					SwitchMode = 0.0f;
				}

				else
				{
					MovementMode = Mode4[rand() % 2];
					SwitchMode = 0.0f;
				}
			}
			break;

		default:

			UE_LOG(LogTemp, Error, TEXT("No movement selected"));

			break;
		}

	}
	//If the enemy is hit by MeleeAttack, act accordingly.
	if (bHitByMelee)
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

void APacmanEnemy::MoveUp()
{
	FVector MoveUp = FVector(1.0f, 0.0f, 0.0f);
	//FRotator LookUp = FRotator(0.0f, 90.0f, 0.0f);
	if (!bHitByProjectile)
	{
		AddMovementInput(MoveUp, MovementValue);
		SetActorRotation(MoveUp.Rotation());
	}
	else
	{
		AddMovementInput(-GetActorForwardVector(), MovementValue);
	}
}

void APacmanEnemy::MoveDown()
{
	FVector MoveDown = FVector(-1.0f, 0.0f, 0.0f);

	if (!bHitByProjectile)
	{
		AddMovementInput(MoveDown, MovementValue);
		SetActorRotation(MoveDown.Rotation());
	}
	else
	{
		AddMovementInput(-GetActorForwardVector(), MovementValue);
	}
}

void APacmanEnemy::MoveLeft()
{
	FVector MoveLeft = FVector(0.0f, -1.0f, 0.0f);

	if (!bHitByProjectile)
	{
		AddMovementInput(MoveLeft, MovementValue);
		SetActorRotation(MoveLeft.Rotation());
	}
	else
	{
		AddMovementInput(-GetActorForwardVector(), MovementValue);
	}
}

void APacmanEnemy::MoveRight()
{
	FVector MoveRight = FVector(0.0f, 1.0f, 0.0f);

	if (!bHitByProjectile)
	{
		AddMovementInput(MoveRight, MovementValue);
		SetActorRotation(MoveRight.Rotation());
	}
	else
	{
		AddMovementInput(-GetActorForwardVector(), MovementValue);
	}

}

void APacmanEnemy::SpawnStaticProjectile(float DeltaTime)
{
	UWorld * World;

	World = GetWorld();

	SpawnTimer += DeltaTime;

	FVector Location = GetActorLocation();
	Location.Z = 30.0f;

	if (SpawnTimer > 0.3f)
	{
		World->SpawnActor<AStaticProjectile>(StaticProjectile_BP, Location, FRotator::ZeroRotator);
		SpawnTimer = 0.0f;
	}

}

void APacmanEnemy::SpawnPowerUp()
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
		MaxPowerUpTypes = rand() % 3 + 1;
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

		default:
			break;
		}
	}
}

void APacmanEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerProjectile::StaticClass()))
	{
		Health--;
		bHitByProjectile = true;
		if (Health < 1)
		{
			//SpawnPowerUp();
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

		//UE_LOG(LogTemp, Warning, TEXT("PacmanEnemy was hit by PlayerMeleeAttack"));
		bHitByMelee = true;
		HitByMeleeTimer = 0.0f;

	}
}