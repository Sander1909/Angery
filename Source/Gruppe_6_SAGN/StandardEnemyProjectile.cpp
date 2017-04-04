// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "StandardEnemyProjectile.h"


// Sets default values
AStandardEnemyProjectile::AStandardEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStandardEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<USphereComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStandardEnemyProjectile::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StandardEnemyProjectile no collision box"));

	}

}

// Called every frame
void AStandardEnemyProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	SetProjectileLocation(DeltaTime);

	TimeToDestroy += DeltaTime;

	if (TimeToDestroy > 20.0f)
	{
		Destroy();
	}
}

void AStandardEnemyProjectile::SetProjectileLocation(float DeltaTime)
{
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * DeltaTime * Speed;

	SetActorLocation(NewLocation);
}

void AStandardEnemyProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit A Wall"));
	if (OtherActor->IsRootComponentStatic())
	{
		Destroy();
	}
}