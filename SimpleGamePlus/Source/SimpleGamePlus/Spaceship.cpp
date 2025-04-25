// Fill out your copyright notice in the Description page of Project Settings.


#include "Spaceship.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "Enemy.h"
#include "EnemyGroup.h"
#include "EnemyMissile.h"
#include "Laser.h"

// Sets default values
ASpaceship::ASpaceship()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	shield->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	shield->OnComponentBeginOverlap.AddDynamic(this, &ASpaceship::OnShieldOverlapBegin);
	shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	shield->SetHiddenInGame(true);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASpaceship::OnCapsuleOverlapBegin);
}

// Called when the game starts or when spawned
void ASpaceship::BeginPlay()
{
	Super::BeginPlay();

	//check(GEngine != nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Beginning spaceship."));

	//Add Input Mapping Context to controller
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultInputMappingContext, 0);
		}
	}

	APlayerMissile::numActiveMissiles = 0;
}

// Called every frame
void ASpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	enhancedInputComponent->BindAction(input_fire, ETriggerEvent::Triggered, this, &ASpaceship::FireWeapon);
	enhancedInputComponent->BindAction(input_shield, ETriggerEvent::Triggered, this, &ASpaceship::RaiseShield);
	enhancedInputComponent->BindAction(input_move, ETriggerEvent::Triggered, this, &ASpaceship::MoveRight);
}

void ASpaceship::MoveRight(const FInputActionValue& value)
{
	const float moveValue = value.Get<float>();
	AddMovementInput(FVector(0, 1, 0), moveValue);

}

void ASpaceship::FireWeapon()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(AEnemyGroup::numEnemiesDestroyedForLaser));
	if (AEnemyGroup::numEnemiesDestroyedForLaser >= enemiesToDefeatUntilPower)
	{
		if (!laserClass)
		{
			check(GEngine != nullptr);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ERROR: No laser class found"));
			return;
		}

		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();

			ALaser* newLaser = world->SpawnActor<ALaser>(laserClass, shield->GetComponentTransform().GetLocation(), FRotator::ZeroRotator, spawnParams);
			if (newLaser)
			{
				FAttachmentTransformRules attachmentRules(
					EAttachmentRule::SnapToTarget,
					EAttachmentRule::KeepWorld,
					EAttachmentRule::KeepWorld,
					false);
				newLaser->AttachToActor(this, attachmentRules);

				// Set timer to reset enemies and destroy laser
				FTimerHandle laserHandle;
				GetWorldTimerManager().SetTimer(laserHandle, newLaser, &ALaser::DestroyLaser, laserDuration, false);
			}
		}

		return;
	}

	if (!missileClass)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ERROR: No missile class found"));
		return;
	}
		
	//check(GEngine != nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(APlayerMissile::numActiveMissiles));

	if (APlayerMissile::numActiveMissiles < numAllowedActiveMissiles)
	{
		FVector missileLocation = shield->GetComponentTransform().GetLocation();
		FVector missileDirection = FVector(1, 0, 0);

		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();

			APlayerMissile* newMissile = world->SpawnActor<APlayerMissile>(missileClass, missileLocation, FRotator::ZeroRotator, spawnParams);
			if (newMissile)
			{
				newMissile->FireInDirection(missileDirection);
				APlayerMissile::numActiveMissiles++;
			}
		}
	}
}

void ASpaceship::RaiseShield()
{
	shield->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	shield->SetHiddenInGame(false);

	// Wait shieldDuration seconds and disable collision again
	FTimerHandle ShieldHandle;
	GetWorldTimerManager().SetTimer(ShieldHandle, this, &ASpaceship::LowerShield, shieldDuration, false);
}

void ASpaceship::LowerShield()
{
	shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	shield->SetHiddenInGame(true);
}

void ASpaceship::OnShieldOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemy::StaticClass()) || OtherActor->IsA(AEnemyMissile::StaticClass()))
	{
		OtherActor->Destroy();
	}
}

void ASpaceship::OnCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemy::StaticClass()) || OtherActor->IsA(AEnemyMissile::StaticClass()))
	{
		OnPlayerHit();
	}
}

void ASpaceship::OnPlayerHit()
{
	// Blow up player, tell game manager that the player has died?
	Destroy();
}

