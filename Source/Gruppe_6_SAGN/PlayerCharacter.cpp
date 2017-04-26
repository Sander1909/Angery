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

	//Add overlap function to class collision box.
	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character no collision box"));

	}
	
	//Possess player character.
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	//Show cursor in game.
	MyController->bShowMouseCursor = true;

	//Set maximum movementspeed to Speed variable.
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	
}

// Called every frame
void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Calling rotation function.
	SetPlayerRotation();

	//Invincibility frame duration.
	if (bIFrame)
	{
		IFrameTimer += DeltaTime;

		if (IFrameTimer >= 1.0f)
		{
			bIFrame = false;
			IFrameTimer = 0.0f;
		}
	}

	//Curving bullets duration.
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

	//Run shoot function.
	if (bIsShooting)
	{
		ShootTimer += DeltaTime;

		if (ShootTimer >= FireRate)
		{
			Shoot();
			ShootTimer = 0.0f;
		}
	}

	//Increased fire rate duration.
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

	//Melee dash duration and sets movement speed.
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

	//Creates cooldown timer for melee dash ability.
	if (bMeleeDelay)
	{
		MeleeDelayTimer += DeltaTime;

		if (MeleeDelayTimer > 2.0f)
		{
			MeleeDelayTimer = 0.0f;
			bMeleeDelay = false;
		}
	}
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
	//Can't shoot if you're dead.
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
	//Can't use melee dash if you're dead or on cooldown.
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
	//Can't move if you're dead.
	if (!bIsDead)
	{
		FVector MoveX = FVector(1.0f, 0.0f, 0.0f);
		AddMovementInput(MoveX, Value);
	}
}

void APlayerCharacter::MoveY(float Value)
{
	//Can't move if you're dead.
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
	//You won't get hurt if you are in melee dash or during invincibility frames.
	//ENEMY ATTACKS

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

	//POWERUPS

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

	//Death event.

	if (Health < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has died."));
		ShowDeadHUD();
		bIsDead = true;
	}
}

void APlayerCharacter::SetPlayerRotation()
{

	//Won't rotate if you're dead.
	if (!bIsDead)
	{

		//Gets cursor location, and rotates player towards it. 
		FHitResult Hit;
		bool HitResult = false;
		HitResult = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, Hit);

		if (HitResult)
		{
			FVector CursorLocation = Hit.Location;
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
	//This one doesn't need content, but won't work without the function.
}

void APlayerCharacter::StartMinorCameraShake_Implementation()
{
	//Same.
}

void APlayerCharacter::ShowDeadHUD_Implementation()
{
	//Same.
}

//Spawns player bullets in a grid on the top of the map and run them down the map. 
void APlayerCharacter::SpawnBulletRain()
{

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