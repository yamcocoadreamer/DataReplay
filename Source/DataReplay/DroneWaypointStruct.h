// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DroneWaypointStruct.generated.h"

/**
 * Structure pour stocker un point de la trajectoire du drone
 * Cette structure sera utilisée pour importer les données CSV
 */
USTRUCT(BlueprintType)
struct FDroneWaypointRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Temps en secondes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	float Time;

	// Position X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	float X;

	// Position Y
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	float Y;

	// Position Z (altitude)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	float Z;

	// Constructeur par défaut
	FDroneWaypointRow()
		: Time(0.f)
		, X(0.f)
		, Y(0.f)
		, Z(0.f)
	{
	}
};