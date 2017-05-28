// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "SpinningMeleeEnemy.h"
#include "PlayerMeleeAttack.h"
#include "PlayerProjectile.h"
#include "SpinningMeleeEnemyAttack.h"



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

	//Add overlap function to class collision box.
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

	//Keeps track of when to attack and when to move.
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

	//Does a neat spin.
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