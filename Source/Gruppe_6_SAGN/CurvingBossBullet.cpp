// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "CurvingBossBullet.h"


// Sets default values
ACurvingBossBullet::ACurvingBossBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACurvingBossBullet::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = this->FindComponentByClass<USphereComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACurvingBossBullet::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurvingBossProjectile no collision box"));

	}
}

// Called every frame
void ACurvingBossBullet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	TimeToDestroy += DeltaTime;

	MoveForward(DeltaTime);
	CurveLeft();

	if (TimeToDestroy > 8.0f)
	{
		Destroy();
	}

}

void ACurvingBossBullet::MoveForward(float DeltaTime)
{
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * DeltaTime * Speed;

	SetActorLocation(NewLocation);
}

void ACurvingBossBullet::CurveLeft()
{
	FRotator NewRotation = GetActorRotation() + FRotator(0.0f, 0.25f, 0.0f);

	SetActorRotation(NewRotation);
}

void ACurvingBossBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit A Wall"));
	if (OtherActor->IsRootComponentStatic())
	{
		Destroy();
	}
}