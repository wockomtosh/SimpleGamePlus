// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PlayerMissile.h"
#include "EnemyMissile.h"
#include "EnemyGroup.h"
#include "Laser.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	collisionComponent->InitBoxExtent(FVector(80, 80, 80));
	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	RootComponent = collisionComponent;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + (velocity * DeltaTime));

}

void AEnemy::FireMissile()
{
	if (!missileClass)
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ERROR: No enemy missile class found"));
		return;
	}

	FVector missileLocation = GetActorLocation() + missileLaunchOffset;
	FVector missileDirection = FVector(-1, 0, 0);

	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		AEnemyMissile* newMissile = world->SpawnActor<AEnemyMissile>(missileClass, missileLocation, FRotator::ZeroRotator, spawnParams);
		if (newMissile)
		{
			newMissile->FireInDirection(missileDirection);
		}
	}
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Enemy Overlap started"));
	if (OtherActor->IsA(APlayerMissile::StaticClass()) || OtherActor->IsA(ALaser::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Enemy Overlapped with missile"));
		if (enemyGroup)
		{
			enemyGroup->OnEnemyDestroyed(this);
		}
		Destroy();
	}
	else if (!OtherActor->IsA(AEnemyMissile::StaticClass()))
	{
		if (enemyGroup)
		{
			enemyGroup->OnEnemyCollidedWithBoundary();
		}
	}
}

