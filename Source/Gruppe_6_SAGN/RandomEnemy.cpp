// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "RandomEnemy.h"
#include "StandardEnemyProjectile.h"
#include "PlayerProjectile.h"


// Sets default values
ARandomEnemy::ARandomEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARandomEnemy::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARandomEnemy::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RandomEnemy no collision box"));
	}


}

// Called every frame
void ARandomEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	MoveForward(DeltaTime);
	SetRandomRotation(DeltaTime);
	SpawnProjectile(DeltaTime);

}

// Called to bind functionality to input
void ARandomEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARandomEnemy::MoveForward(float DeltaTime)
{
	FVector ForwardVector = GetActorForwardVector() * DeltaTime;

	AddMovementInput(ForwardVector, MovementValue);
}

void ARandomEnemy::SetRandomRotation(float DeltaTime)
{
	RandomTimer += DeltaTime;

	FRotator RandomRotation;

	if (RandomTimer > 0.8f)
	{
		RandomRotation = FRotator(0.0f, rand() % 360 + 1, 0.0f);

		SetActorRotation(RandomRotation);

		RandomTimer = 0.0f;
	}

}

void ARandomEnemy::SpawnProjectile(float DeltaTime)
{
	ProjectileDelay += DeltaTime;
	
	UWorld * World;
	
	World = GetWorld();

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - GetActorLocation();


	if (ProjectileDelay > 0.5f)
	{
		World->SpawnActor<AStandardEnemyProjectile>(StandardEnemyProjectile_BP, GetActorLocation(), NewDirection.Rotation());
		ProjectileDelay = 0.0f;	
		
	}
}

void ARandomEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(APlayerProjectile::StaticClass()))
	{
		Health--;
		if (Health < 1)
		{
			Destroy();
		}
		OtherActor->Destroy();
	}
}
