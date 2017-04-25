// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "PlayerCharacter.h"
#include "StandardEnemyProjectile.h"
#include "PlayerProjectile.h"
#include "PlayerMeleeAttack.h"
#include "CurvingBossBullet.h"
#include "StaticProjectile.h"
#include "SpinningMeleeEnemyAttack.h"
#include "P_Up_Bulletrain.h"
#include "P_Up_FullHealth.h"
#include "P_Up_FireRate.h"
#include "P_Up_CurvingBullet.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character no collision box"));

	}

	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = true;

	GetCharacterMovement()->MaxWalkSpeed = Speed;

	//OnPlayerHit.Broadcast();
	
}

// Called every frame
void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	SetPlayerRotation();

	if (bIFrame)
	{
		IFrameTimer += DeltaTime;
		if (IFrameTimer >= 1.0f)
		{
			bIFrame = false;
			IFrameTimer = 0.0f;
		}
	}

	if (bCurvingBullet)
	{
		CurvingBulletTimer += DeltaTime;

		if (PlayerProjectilePtr)
		{
			PlayerProjectilePtr->SetCurvingBullet();
		}

		if (CurvingBulletTimer > 10.0f)
		{
			bCurvingBullet = false;
			CurvingBulletTimer = 0.0f;
		}
	}

	if (bIsShooting)
	{
		ShootTimer += DeltaTime;

		if (ShootTimer >= FireRate)
		{
			Shoot();
			ShootTimer = 0.0f;
		}
	}

	if (bIsFireRate)
	{
		FireRateTimer += DeltaTime;
		if (FireRateTimer >= 10.0f)
		{
			bIsFireRate = false;
			FireRate = 0.2f;
			FireRateTimer = 0.0f;
		}
	}

	if (bMeleeDash)
	{
		MeleeDashTimer += DeltaTime;
		GetCharacterMovement()->MaxWalkSpeed = 15000.0f;
		if (MeleeDashTimer > 0.3f)
		{
			bMeleeDash = false;
			MeleeDashTimer = 0.0f;
			GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}

	if (bMeleeDelay)
	{
		MeleeDelayTimer += DeltaTime;

		if (MeleeDelayTimer > 2.0f)
		{
			MeleeDelayTimer = 0.0f;
			bMeleeDelay = false;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Player Health is %i"), Health);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

		InputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerCharacter::Shoot);
		InputComponent->BindAction("Shoot", IE_Released, this, &APlayerCharacter::StopShoot);
		InputComponent->BindAction("Melee", IE_Pressed, this, &APlayerCharacter::Melee);

		InputComponent->BindAxis("MoveX", this, &APlayerCharacter::MoveX);
		InputComponent->BindAxis("MoveY", this, &APlayerCharacter::MoveY);
}

void APlayerCharacter::Shoot()
{
	if (!bIsDead)
	{
		UWorld * World;

		World = GetWorld();

		FVector Location = GetActorLocation() + GetActorForwardVector();
		Location.Z = 10.0f;

		if (World)
		{
			bIsShooting = true;
			UGameplayStatics::PlaySound2D(World, OnPlayerShootSound, 0.05f, 1.0f, 0.0f);
			StartMinorCameraShake();

			PlayerProjectilePtr = World->SpawnActor<APlayerProjectile>(PlayerProjectile_BP, Location, GetActorRotation());
		}
	}
}

void APlayerCharacter::StopShoot()
{
	bIsShooting = false;
}

void APlayerCharacter::Melee()
{
	if (!bIsDead && !bMeleeDelay)
	{
		UWorld * World;

		World = GetWorld();

		if (World)
		{
			World->SpawnActor<APlayerMeleeAttack>(PlayerMeleeAttack_BP, GetActorLocation(), FRotator::ZeroRotator);
			bMeleeDash = true;
			bMeleeDelay = true;
		}
	}
}

void APlayerCharacter::MoveX(float Value)
{
	if (!bIsDead)
	{
		FVector MoveX = FVector(1.0f, 0.0f, 0.0f);
		AddMovementInput(MoveX, Value);
	}
}

void APlayerCharacter::MoveY(float Value)
{
	if (!bIsDead)
	{
		FVector MoveY = FVector(0.0f, 1.0f, 0.0f);
		AddMovementInput(MoveY, Value);
	}
}

void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{

	//TODO Putte inn invulnerabilityframes
	//SetVisibility = true/false;

	if (OtherActor->IsA(AStandardEnemyProjectile::StaticClass()))
	{
		if (!bMeleeDash && !bIFrame)
		{
			bIFrame = true;
			Health--;
			UGameplayStatics::PlaySound2D(GetWorld(), OnPlayerHitSound, 0.5f, 1.0f, 0.0f);
			StartCameraShake();
		}
		OtherActor->Destroy();
	}

	if (OtherActor->IsA(ASpinningMeleeEnemyAttack::StaticClass()))
	{
		if (!bMeleeDash && !bIFrame)
		{
			bIFrame = true;
			Health--;
			UGameplayStatics::PlaySound2D(GetWorld(), OnPlayerHitSound, 0.5f, 1.0f, 0.0f);
			StartCameraShake();
		}
		OtherActor->Destroy();
	}

	else if (OtherActor->IsA(ACurvingBossBullet::StaticClass()))
	{
		if (!bMeleeDash && !bIFrame)
		{
			bIFrame = true;
			Health--;
			UGameplayStatics::PlaySound2D(GetWorld(), OnPlayerHitSound, 0.5f, 1.0f, 0.0f);
			StartCameraShake();
		}
		OtherActor->Destroy();
	}

	else if (OtherActor->IsA(AStaticProjectile::StaticClass()))
	{
		if (!bMeleeDash && !bIFrame)
		{
			bIFrame = true;
			Health--;
			UGameplayStatics::PlaySound2D(GetWorld(), OnPlayerHitSound, 0.5f, 1.0f, 0.0f);
			StartCameraShake();
		}
		OtherActor->Destroy();
	}

	else if (OtherActor->IsA(AP_Up_BulletRain::StaticClass()))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OnPowerUpSound, 0.5f, 1.0f, 0.0f);
		SpawnBulletRain();
		OtherActor->Destroy();
	}

	else if (OtherActor->IsA(AP_Up_FullHealth::StaticClass()))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OnPowerUpSound, 0.5f, 1.0f, 0.0f);
		CharacterFullHealth();
		OtherActor->Destroy();
	}

	else if (OtherActor->IsA(AP_Up_FireRate::StaticClass()))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), OnPowerUpSound, 0.5f, 1.0f, 0.0f);
		BoostFireRate();
		OtherActor->Destroy();
	}

	else if (OtherActor->IsA(AP_Up_CurvingBullet::StaticClass()))
	{
		bCurvingBullet = true;
		UGameplayStatics::PlaySound2D(GetWorld(), OnPowerUpSound, 0.5f, 1.0f, 0.0f);
		OtherActor->Destroy();
	}

	if (Health < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has died."));
		//UGameplayStatics::SetGamePaused(GetWorld(), true);
		bIsDead = true;
	}
}

void APlayerCharacter::SetPlayerRotation()
{
	if (!bIsDead)
	{
		FHitResult Hit;
		bool HitResult = false;
		HitResult = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, Hit);

		if (HitResult)
		{
			FVector CursorLocation = Hit.Location;

			//      UE_LOG(LogTemp, Warning, TEXT("Cursor location %s!"), *CursorLocation.ToString());

			FVector TempLocation = FVector(CursorLocation.X, CursorLocation.Y, 30.f);

			FVector NewDirection = TempLocation - GetActorLocation();
			NewDirection.Z = 0.0f;
			NewDirection.Normalize();
			SetActorRotation(NewDirection.Rotation());
		}
	}
}

void APlayerCharacter::StartCameraShake_Implementation()
{
	//Skriver noe her så Sander ikke blir lei seg siden funksjonen er ensom, tom og irritert på innsiden.
}

void APlayerCharacter::StartMinorCameraShake_Implementation()
{
	// :)
}

void APlayerCharacter::SpawnBulletRain()
{
	//UE_LOG(LogTemp, Warning, TEXT("Bullet Rain"));
	UWorld * World;

	World = GetWorld();

	for (int y = -Width; y < Width; y += 100)
	{
		for(int x = Heigth; x > 1350; x -= 100)
		{
			World->SpawnActor<APlayerProjectile>(PlayerProjectile_BP, FVector(x, y, 30.0f), FVector(-1.0f, 0.0f, 0.0f).Rotation());
		}
	}
}

void APlayerCharacter::CharacterFullHealth()
{
	Health = MaxHealth;
}

void APlayerCharacter::BoostFireRate()
{
	bIsFireRate = true;
	FireRate = 0.1;
}