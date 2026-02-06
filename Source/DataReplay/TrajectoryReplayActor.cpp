// Fill out your copyright notice in the Description page of Project Settings.

#include "TrajectoryReplayActor.h"
#include "Components/StaticMeshComponent.h"

// ========== CONSTRUCTEUR ==========

ATrajectoryReplayActor::ATrajectoryReplayActor()
{
	// Activer le Tick pour mettre à jour chaque frame
	PrimaryActorTick.bCanEverTick = true;

	// Créer le mesh component pour la visualisation
	VisualizationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualizationMesh"));
	RootComponent = VisualizationMesh;

	// Valeurs par défaut des paramètres
	PlaybackSpeed = 1.0f;
	bAutoPlay = false;
	bIsPlaying = false;
	bLoopPlayback = false;
	bReversePlayback = false;
	CurrentPlaybackTime = 0.0f;
	TotalDuration = 0.0f;
	WaypointCount = 0;

	// Valeurs par défaut de la visualisation
	bShowTrajectory = false;
	TrajectoryColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Vert par défaut
	TrajectoryThickness = 5.0f;
	bShowWaypoints = false;
	WaypointRadius = 10.0f;
}

// ========== ÉVÉNEMENTS DU CYCLE DE VIE ==========

void ATrajectoryReplayActor::BeginPlay()
{
	Super::BeginPlay();

	// Charger les points de trajectoire depuis le DataTable
	LoadTrajectoryPoints();

	// Log d'information
	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Actor initialized with %d waypoints, Total Duration: %.2f seconds"),
		WaypointCount, TotalDuration);

	// Démarrage automatique si activé
	if (bAutoPlay && WaypointCount > 0)
	{
		Play();
	}
}

void ATrajectoryReplayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Dessiner la trajectoire si activée
	if (bShowTrajectory)
	{
		DrawTrajectoryVisualization();
	}

	// Vérifier si la lecture est active
	if (bIsPlaying && WaypointCount > 0)
	{
		// Calculer l'incrément de temps selon la vitesse et la direction
		float TimeIncrement = DeltaTime * PlaybackSpeed;
		if (bReversePlayback)
		{
			TimeIncrement = -TimeIncrement;
		}

		// Mettre à jour le temps actuel
		CurrentPlaybackTime += TimeIncrement;

		// Gérer les conditions de boucle et de fin
		if (CurrentPlaybackTime > TotalDuration)
		{
			if (bLoopPlayback)
			{
				// Recommencer depuis le début
				CurrentPlaybackTime = 0.0f;
			}
			else
			{
				// Arrêter à la fin
				CurrentPlaybackTime = TotalDuration;
				Pause();
				UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Playback completed"));
			}
		}
		else if (CurrentPlaybackTime < 0.0f)
		{
			if (bLoopPlayback)
			{
				// Recommencer depuis la fin (lecture inverse)
				CurrentPlaybackTime = TotalDuration;
			}
			else
			{
				// Arrêter au début
				CurrentPlaybackTime = 0.0f;
				Pause();
				UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Reverse playback completed"));
			}
		}

		// Mettre à jour la position de l'acteur
		UpdateActorPosition();
	}
}

// ========== CHARGEMENT DES DONNÉES ==========

void ATrajectoryReplayActor::LoadTrajectoryPoints()
{
	// Réinitialiser les données
	TrajectoryPoints.Empty();
	TotalDuration = 0.0f;
	WaypointCount = 0;

	// Vérifier que le DataTable est assigné
	if (TrajectoryData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[TrajectoryReplay] TrajectoryData is NULL! Please assign a DataTable in the editor."));
		return;
	}

	// Récupérer tous les noms de lignes du DataTable
	TArray<FName> RowNames = TrajectoryData->GetRowNames();

	// Charger chaque ligne
	for (const FName& RowName : RowNames)
	{
		FDroneWaypointRow* Row = TrajectoryData->FindRow<FDroneWaypointRow>(RowName, TEXT(""));
		if (Row)
		{
			TrajectoryPoints.Add(Row);
		}
	}

	// Trier les points par temps (croissant)
	TrajectoryPoints.Sort([](const FDroneWaypointRow& A, const FDroneWaypointRow& B)
		{
			return A.Time < B.Time;
		});

	// Calculer les statistiques
	WaypointCount = TrajectoryPoints.Num();
	if (WaypointCount > 0)
	{
		TotalDuration = TrajectoryPoints.Last()->Time;
	}

	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Loaded %d waypoints from DataTable"), WaypointCount);
}

void ATrajectoryReplayActor::ReloadTrajectoryData()
{
	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Reloading trajectory data..."));

	bool bWasPlaying = bIsPlaying;
	Stop();

	LoadTrajectoryPoints();

	if (bWasPlaying && bAutoPlay)
	{
		Play();
	}
}

// ========== CALCUL DE POSITION ==========

FVector ATrajectoryReplayActor::CalculatePositionAtTime(float Time) const
{
	// Si aucun point n'est chargé
	if (WaypointCount == 0)
	{
		return FVector::ZeroVector;
	}

	// Si un seul point
	if (WaypointCount == 1)
	{
		return FVector(TrajectoryPoints[0]->X, TrajectoryPoints[0]->Y, TrajectoryPoints[0]->Z);
	}

	// Si le temps est avant le premier point
	if (Time <= TrajectoryPoints[0]->Time)
	{
		return FVector(TrajectoryPoints[0]->X, TrajectoryPoints[0]->Y, TrajectoryPoints[0]->Z);
	}

	// Si le temps est après le dernier point
	if (Time >= TrajectoryPoints.Last()->Time)
	{
		FDroneWaypointRow* LastPoint = TrajectoryPoints.Last();
		return FVector(LastPoint->X, LastPoint->Y, LastPoint->Z);
	}

	// Trouver les deux points entre lesquels interpoler
	for (int32 i = 0; i < WaypointCount - 1; i++)
	{
		FDroneWaypointRow* CurrentPoint = TrajectoryPoints[i];
		FDroneWaypointRow* NextPoint = TrajectoryPoints[i + 1];

		if (Time >= CurrentPoint->Time && Time <= NextPoint->Time)
		{
			// Calculer le facteur d'interpolation linéaire (0.0 à 1.0)
			float TimeDelta = NextPoint->Time - CurrentPoint->Time;
			float Alpha = (TimeDelta > 0.0f) ? (Time - CurrentPoint->Time) / TimeDelta : 0.0f;

			// Positions des deux points
			FVector PosA(CurrentPoint->X, CurrentPoint->Y, CurrentPoint->Z);
			FVector PosB(NextPoint->X, NextPoint->Y, NextPoint->Z);

			// Interpolation linéaire
			return FMath::Lerp(PosA, PosB, Alpha);
		}
	}

	// Cas par défaut (ne devrait pas arriver)
	return FVector::ZeroVector;
}

void ATrajectoryReplayActor::UpdateActorPosition()
{
	FVector NewPosition = CalculatePositionAtTime(CurrentPlaybackTime);
	SetActorLocation(NewPosition);
}

// ========== CONTRÔLES DE LECTURE ==========

void ATrajectoryReplayActor::Play()
{
	if (WaypointCount == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[TrajectoryReplay] Cannot play - no trajectory data loaded"));
		return;
	}

	bIsPlaying = true;
	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Playback started at time %.2f"), CurrentPlaybackTime);
}

void ATrajectoryReplayActor::Pause()
{
	bIsPlaying = false;
	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Playback paused at time %.2f"), CurrentPlaybackTime);
}

void ATrajectoryReplayActor::Stop()
{
	bIsPlaying = false;
	CurrentPlaybackTime = 0.0f;

	if (WaypointCount > 0)
	{
		UpdateActorPosition();
	}

	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Playback stopped and reset"));
}

void ATrajectoryReplayActor::SeekToTime(float TimeInSeconds)
{
	CurrentPlaybackTime = FMath::Clamp(TimeInSeconds, 0.0f, TotalDuration);
	UpdateActorPosition();

	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Seeked to time %.2f seconds"), CurrentPlaybackTime);
}

void ATrajectoryReplayActor::SeekToProgress(float Progress)
{
	Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
	CurrentPlaybackTime = Progress * TotalDuration;
	UpdateActorPosition();

	UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Seeked to %.1f%% progress"), Progress * 100.0f);
}

float ATrajectoryReplayActor::GetPlaybackProgress() const
{
	if (TotalDuration > 0.0f)
	{
		return CurrentPlaybackTime / TotalDuration;
	}
	return 0.0f;
}

void ATrajectoryReplayActor::ClampCurrentTime()
{
	CurrentPlaybackTime = FMath::Clamp(CurrentPlaybackTime, 0.0f, TotalDuration);
}

// ========== VISUALISATION DE LA TRAJECTOIRE ==========

void ATrajectoryReplayActor::ToggleTrajectoryVisualization()
{
	bShowTrajectory = !bShowTrajectory;

	if (bShowTrajectory)
	{
		UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Trajectory visualization enabled"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[TrajectoryReplay] Trajectory visualization disabled"));
	}
}

void ATrajectoryReplayActor::DrawTrajectoryVisualization()
{
	if (WaypointCount < 2 || !GetWorld())
	{
		return;
	}

	// Dessiner des lignes entre chaque paire de waypoints
	for (int32 i = 0; i < WaypointCount - 1; i++)
	{
		FDroneWaypointRow* CurrentPoint = TrajectoryPoints[i];
		FDroneWaypointRow* NextPoint = TrajectoryPoints[i + 1];

		FVector StartPos(CurrentPoint->X, CurrentPoint->Y, CurrentPoint->Z);
		FVector EndPos(NextPoint->X, NextPoint->Y, NextPoint->Z);

		// Dessiner la ligne
		DrawDebugLine(
			GetWorld(),
			StartPos,
			EndPos,
			TrajectoryColor.ToFColor(true),
			false,  // Persistent (false = une frame seulement)
			-1.0f,  // Lifetime
			0,      // Depth priority
			TrajectoryThickness
		);
	}

	// Dessiner des sphères aux waypoints si activé
	if (bShowWaypoints)
	{
		for (int32 i = 0; i < WaypointCount; i++)
		{
			FDroneWaypointRow* Point = TrajectoryPoints[i];
			FVector Position(Point->X, Point->Y, Point->Z);

			DrawDebugSphere(
				GetWorld(),
				Position,
				WaypointRadius,
				12,  // Segments
				TrajectoryColor.ToFColor(true),
				false,
				-1.0f,
				0,
				1.0f
			);
		}
	}
}