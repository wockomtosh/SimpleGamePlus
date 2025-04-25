// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMissile.h"
#include "Spaceship.h"

int APlayerMissile::numActiveMissiles = 0;

// Sets default values
APlayerMissile::APlayerMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));

	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	collisionComponent->InitBoxExtent(FVector(50, 16, 16));
	RootComponent = collisionComponent;

	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerMissile::OnOverlapBegin);

	// Projectile Movement Properties---------------------------------
	// Use this component to drive this projectile's movement.
	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	projectileMovementComponent->SetUpdatedComponent(collisionComponent);
	projectileMovementComponent->InitialSpeed = 3000.0f;
	projectileMovementComponent->MaxSpeed = 3000.0f;
	projectileMovementComponent->bRotationFollowsVelocity = true;
	projectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void APlayerMissile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerMissile::FireInDirection(const FVector& fireDirection)
{
	projectileMovementComponent->Velocity = fireDirection * projectileMovementComponent->InitialSpeed;
}

void APlayerMissile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//check(GEngine != nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Missile Overlap started"));
	if (!OtherActor->IsA(ASpaceship::StaticClass()))
	{
		APlayerMissile::numActiveMissiles--;
		Destroy();
	}
}

