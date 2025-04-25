// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMissile.h"
#include "Enemy.h"

// Sets default values
AEnemyMissile::AEnemyMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	collisionComponent->InitBoxExtent(FVector(50, 16, 16));
	RootComponent = collisionComponent;

	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMissile::OnOverlapBegin);

	// Projectile Movement Properties---------------------------------
	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	projectileMovementComponent->SetUpdatedComponent(collisionComponent);
	projectileMovementComponent->InitialSpeed = 1000.0f;
	projectileMovementComponent->MaxSpeed = 1000.0f;
	projectileMovementComponent->bRotationFollowsVelocity = true;
	projectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AEnemyMissile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyMissile::FireInDirection(const FVector& fireDirection)
{
	projectileMovementComponent->Velocity = fireDirection * projectileMovementComponent->InitialSpeed;
}

void AEnemyMissile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AEnemy::StaticClass()))
	{
		Destroy();
	}
}

