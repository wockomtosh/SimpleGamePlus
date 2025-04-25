// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"
#include "Enemy.h"
#include "EnemyGroup.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	collisionComponent->InitBoxExtent(FVector(500, 32, 32));
	RootComponent = collisionComponent;
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaser::DestroyLaser()
{
	// Reset enemies at the end so you don't constantly chain the laser
	AEnemyGroup::numEnemiesDestroyedForLaser = 0;

	Destroy();
}

