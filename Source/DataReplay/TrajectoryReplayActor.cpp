// Fill out your copyright notice in the Description page of Project Settings.


#include "TrajectoryReplayActor.h"

// Sets default values
ATrajectoryReplayActor::ATrajectoryReplayActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrajectoryReplayActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrajectoryReplayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

