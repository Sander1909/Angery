// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "PacmanEnemy.h"
#include "StaticProjectile.h"
#include "PlayerMeleeAttack.h"
#include "PlayerProjectile.h"
#include "P_Up_BulletRain.h"
#include "P_Up_FullHealth.h"
#include "P_Up_FireRate.h"
#include "P_Up_CurvingBullet.h"


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

	//Add overlap function to class collision box.
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

	SwitchTimer += DeltaTime;

	//Calling standard attack function.
	SpawnStaticProjectile(DeltaTime);

	//Hit state duration.
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

			//Moves up
		case 1:

			//Prevents walking into wall.
			if (GetActorLocation().X > UpperX)
			{
				//Sets new movement mode. (Right or left from current direction)
				MovementMode = Mode1[rand() % 2];
				SwitchTimer = 0.0f;
			}

			MoveUp();

			if (SwitchTimer > 1.0f)
			{
				//If Pacman is by right wall, move left.
				if (GetActorLocation().Y > UpperY)
				{
					MovementMode = 3;
					SwitchTimer = 0.0f;
				}

				//If Pacman is by left wall, move right.
				else if (GetActorLocation().Y < LowerY)
				{
					MovementMode = 4;
					SwitchTimer = 0.0f;
				}

				//Moves normally.
				else
				{
					MovementMode = Mode1[rand() % 3];
					SwitchTimer = 0.0f;
				}
			}

			break;


			//Moves down.
		case 2:

			//Prevents walking into wall.
			if (GetActorLocation().X < LowerX)
			{
				//Sets new movement mode. (Right or left from current direction)
				MovementMode = Mode2[rand() % 2];
				SwitchTimer = 0.0f;
			}

			MoveDown();

			if (SwitchTimer > 1.0f)
			{
				//If Pacman is by right wall, move left.
				if (GetActorLocation().Y > UpperY)
				{
					MovementMode = 3;
					SwitchTimer = 0.0f;
				}

				//If Pacman is by left wall, move right.
				else if (GetActorLocation().Y < LowerY)
				{
					MovementMode = 4;
					SwitchTimer = 0.0f;
				}

				//Move normally.
				else
				{
					MovementMode = Mode2[rand() % 3];
					SwitchTimer = 0.0f;
				}
			}
			break;


			//Moves left.
		case 3:

			//Prevents walking into wall.
			if (GetActorLocation().Y < LowerY)
			{
				//Sets new movement mode. (Right or left from current direction)
				MovementMode = Mode3[rand() % 2];
				SwitchTimer = 0.0f;
			}

			MoveLeft();

			if (SwitchTimer > 1.0f)
			{
				//If Pacman is by upper wall, move down.
				if (GetActorLocation().X > UpperX)
				{
					MovementMode = 2;
					SwitchTimer = 0.0f;
				}

				//If Pacman is by lower wall, move up.
				else if (GetActorLocation().X < LowerX)
				{
					MovementMode = 1;
					SwitchTimer = 0.0f;
				}


				//Move normally.
				else
				{
					MovementMode = Mode3[rand() % 3];
					SwitchTimer = 0.0f;
				}
			}
			break;


			//Moves right.
		case 4:

			//Prevents walking into wall.
			if (GetActorLocation().Y > UpperY)
			{
				//Sets new movement mode. (Right or left from current direction)
				MovementMode = Mode4[rand() % 2];
				SwitchTimer = 0.0f;
			}

			MoveRight();

			if (SwitchTimer > 1.0f)
			{
				//If Pacman is by upper wall, go down.
				if (GetActorLocation().X > UpperX)
				{
					MovementMode = 2;
					SwitchTimer = 0.0f;
				}

				//If Pacman is by lower wall, go up.
				else if (GetActorLocation().X < LowerX)
				{
					MovementMode = 1;
					SwitchTimer = 0.0f;
				}

				//Move normally.
				else
				{
					MovementMode = Mode4[rand() % 3];
					SwitchTimer = 0.0f;
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

	//Rolls for powerup.
	PowerUpRoll = rand() % 100 + 1;
	UE_LOG(LogTemp, Warning, TEXT("Powerup roll is: %i"), PowerUpRoll);
	
	//If the roll is higher than the threshold, spawn a random powerup.
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
			SpawnPowerUp();
			Destroy();
		}

		bHitByMelee = true;
		HitByMeleeTimer = 0.0f;

	}
}