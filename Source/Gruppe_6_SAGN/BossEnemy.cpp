// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "BossEnemy.h"
#include "PlayerProjectile.h"
#include "CurvingBossBullet.h"
#include "StandardEnemyProjectile.h"
#include "SpinningMeleeEnemyAttack.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Add overlap function to class collision box.
	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BossEnemy no collision box"));
	}

}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 50)
	{
		bIsEnraged = true;
	}

	//Calling standard movement functions.
	MoveForward(DeltaTime);
	RotateToPlayer();

	if (bIsEnraged)
	{
		EnrageAttack1 += DeltaTime;
		EnrageAttack2 += DeltaTime;

		if (EnrageAttack1 >= 3.0f)
		{
			FRotator ProjectileRotation = GetActorRotation();
			FRotator AddYaw = FRotator(0.0f, 30.0f, 0.0f);
			FVector Location = GetActorLocation();
			Location.Z = 10.0f;

			for (int i = 0; i < 12; i++)
			{

				GetWorld()->SpawnActor<ACurvingBossBullet>(CurvingBossBullet_BP, Location, ProjectileRotation);

				ProjectileRotation = ProjectileRotation + AddYaw;
			}
			EnrageAttack1 = 0.0f;

		}

		if (EnrageAttack2 >= 4.0f)
		{

			for (int i = 2000; i > -1800; i -= 350)
			{
				GetWorld()->SpawnActor<ASpinningMeleeEnemyAttack>(SpinningMeleeEnemyAttack_BP, FVector(i, 3600.0f, 10.0f), FRotator::ZeroRotator);
			}
			for (int i = 2000; i > -1800; i -= 350)
			{
				GetWorld()->SpawnActor<ASpinningMeleeEnemyAttack>(SpinningMeleeEnemyAttack_BP, FVector(i, -3600.0f, 10.0f), FRotator::ZeroRotator);
			}
			EnrageAttack2 = 0.0f;
		}
	}

	//Boss acts differently in every mode.
	switch (BossMode)
	{

		//Calls circular attack.
	case 1:
		SpawnBulletWave(DeltaTime);

		break;

		//Pause between attacks.
	case 2:
		NewMode += DeltaTime;

		//Sets new attack mode.
		if (NewMode > 0.5f)
		{
			BossMode = NewModeArray[rand() % 2];
			NewMode = 0.0f;
		}
		break;

		//Calls stream attack.
	case 3:
		SpawnBulletStream(DeltaTime);

		break;

	default:

		break;

	}
}

// Called to bind functionality to input
void ABossEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossEnemy::MoveForward(float DeltaTime)
{
	FVector ForwardVector = GetActorForwardVector() * DeltaTime;

	AddMovementInput(ForwardVector, MovementValue);

}

void ABossEnemy::RotateToPlayer()
{
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - GetActorLocation();

	NewDirection.Z = 0.0f;

	SetActorRotation(NewDirection.Rotation());
}

void ABossEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerProjectile::StaticClass()))
	{
		Health--;
		if (Health < 0)
		{
			IsBossDead = 1;
			Destroy();
		}
		OtherActor->Destroy();
	}
}

void ABossEnemy::SpawnBulletWave(float DeltaTime)
{
	UWorld * World;

	World = GetWorld();

	FRotator CurrentRotation = GetActorRotation();

	FRotator AddYaw = FRotator(0.0f, 10.0f, 0.0f);

	FRotator NewRotation = CurrentRotation;

	FVector Location = GetActorLocation();

	Location.Z = 100.0f;

	BulletWaveTimer += DeltaTime;

	//Every second the boss fires 10 projectiles in a circle.
	if (BulletWaveTimer > 1.0f)
	{
		for (int i = 0; i < 36; i++)
		{
			World->SpawnActor<ACurvingBossBullet>(CurvingBossBullet_BP, Location, NewRotation);
			NewRotation = NewRotation + AddYaw;
		}

		BulletWaveTimer = 0.0f;
		WavesSpawned++;
	}

	//Next mode.
	if (WavesSpawned > 5)
	{
		BossMode = 2;
		WavesSpawned = 0;
	}
}

void ABossEnemy::SpawnBulletStream(float DeltaTime)
{
	StopStream = StopStream + DeltaTime;
	StreamDelay = StreamDelay + DeltaTime;

	UWorld * World;

	World = GetWorld();

	FVector Location = GetActorLocation();
	Location.Z = 100.0f;

	//Every 0.15 seconds the boss fires 4 bullets in a circular curve.
	if (StreamDelay > 0.15f)
	{
		AddYawToStream = AddYawToStream + 10.0f;
		AddInvertedYawToStream = AddInvertedYawToStream + 10.0f;

		FRotator NewRotationOne = FRotator(0.0, AddYawToStream, 0.0);
		FRotator NewRotationTwo = FRotator(0.0, AddInvertedYawToStream + 180, 0.0);


		World->SpawnActor<ACurvingBossBullet>(CurvingBossBullet_BP, Location, NewRotationOne);
		World->SpawnActor<ACurvingBossBullet>(CurvingBossBullet_BP, Location, NewRotationOne.GetInverse());
		World->SpawnActor<ACurvingBossBullet>(CurvingBossBullet_BP, Location, NewRotationTwo);
		World->SpawnActor<ACurvingBossBullet>(CurvingBossBullet_BP, Location, NewRotationTwo.GetInverse());

		StreamDelay = 0.0f;
	}

	//Next mode.
	if (StopStream > 4.0f)
	{
		BossMode = 2;
		StopStream = 0.0f;
	}

}

void ABossEnemy::IsBossDeadFunction(int &number)
{
	number = IsBossDead;
}