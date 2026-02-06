// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "DroneWaypointStruct.h"
#include "TrajectoryReplayActor.generated.h"

UCLASS()
class DATAREPLAY_API ATrajectoryReplayActor : public AActor
{
	GENERATED_BODY()

public:
	// Constructeur
	ATrajectoryReplayActor();

	// ========== CONFIGURATION DE LA TRAJECTOIRE ==========

	// DataTable contenant les points de la trajectoire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Data")
	UDataTable* TrajectoryData;

	// ========== VISUALISATION ==========

	// Mesh pour visualiser l'objet qui suit la trajectoire
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visualization")
	UStaticMeshComponent* VisualizationMesh;

	// ========== CONTRÔLES DE LECTURE ==========

	// Vitesse de lecture (1.0 = temps réel, 2.0 = 2x plus rapide, 0.5 = ralenti)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback Controls", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float PlaybackSpeed;

	// Lecture automatique au démarrage du jeu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback Controls")
	bool bAutoPlay;

	// État de lecture actuel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback Controls")
	bool bIsPlaying;

	// Répéter la trajectoire en boucle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback Controls")
	bool bLoopPlayback;

	// Lecture en sens inverse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback Controls")
	bool bReversePlayback;

	// ========== VISUALISATION DE LA TRAJECTOIRE ==========

	// Afficher la trajectoire complète
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	bool bShowTrajectory;

	// Couleur de la ligne de trajectoire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	FLinearColor TrajectoryColor;

	// Épaisseur de la ligne de trajectoire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization", meta = (ClampMin = "1.0", ClampMax = "20.0"))
	float TrajectoryThickness;

	// Afficher des sphères aux points de waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization")
	bool bShowWaypoints;

	// Rayon des sphères de waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trajectory Visualization", meta = (ClampMin = "1.0", ClampMax = "50.0"))
	float WaypointRadius;

	// ========== INFORMATIONS D'ÉTAT ==========

	// Temps actuel dans la trajectoire (lecture seule)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Playback Info")
	float CurrentPlaybackTime;

	// Durée totale de la trajectoire (lecture seule)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Playback Info")
	float TotalDuration;

	// Nombre de points dans la trajectoire (lecture seule)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Playback Info")
	int32 WaypointCount;

protected:
	// Appelé quand le jeu commence
	virtual void BeginPlay() override;

public:
	// Appelé à chaque frame
	virtual void Tick(float DeltaTime) override;

	// ========== FONCTIONS DE CONTRÔLE ==========

	// Démarrer ou reprendre la lecture de la trajectoire
	UFUNCTION(BlueprintCallable, Category = "Trajectory Playback")
	void Play();

	// Mettre en pause la lecture
	UFUNCTION(BlueprintCallable, Category = "Trajectory Playback")
	void Pause();

	// Arrêter et réinitialiser la trajectoire au début
	UFUNCTION(BlueprintCallable, Category = "Trajectory Playback")
	void Stop();

	// Se déplacer à un temps spécifique dans la trajectoire
	UFUNCTION(BlueprintCallable, Category = "Trajectory Playback")
	void SeekToTime(float TimeInSeconds);

	// Se déplacer à un pourcentage de la trajectoire (0.0 à 1.0)
	UFUNCTION(BlueprintCallable, Category = "Trajectory Playback")
	void SeekToProgress(float Progress);

	// Obtenir le pourcentage actuel de progression (0.0 à 1.0)
	UFUNCTION(BlueprintCallable, Category = "Trajectory Playback")
	float GetPlaybackProgress() const;

	// Recharger les données depuis le DataTable
	UFUNCTION(BlueprintCallable, Category = "Trajectory Playback")
	void ReloadTrajectoryData();

	// Afficher/Cacher la visualisation de la trajectoire
	UFUNCTION(BlueprintCallable, Category = "Trajectory Visualization")
	void ToggleTrajectoryVisualization();

	// Dessiner la trajectoire
	UFUNCTION(BlueprintCallable, Category = "Trajectory Visualization")
	void DrawTrajectoryVisualization();

private:
	// ========== DONNÉES INTERNES ==========

	// Tableau contenant tous les points de trajectoire
	TArray<FDroneWaypointRow*> TrajectoryPoints;

	// ========== FONCTIONS INTERNES ==========

	// Charger les points depuis le DataTable
	void LoadTrajectoryPoints();

	// Calculer la position interpolée à un temps donné
	FVector CalculatePositionAtTime(float Time) const;

	// Mettre à jour la position de l'acteur selon le temps actuel
	void UpdateActorPosition();

	// Valider et limiter le temps actuel dans les bornes valides
	void ClampCurrentTime();
};