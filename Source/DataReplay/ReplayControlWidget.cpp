// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplayControlWidget.h"
#include "TrajectoryReplayActor.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void UReplayControlWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsUpdatingSlider = false;

	// Vérifier que tous les widgets sont bien liés
	if (!Btn_Play || !Btn_Pause || !Btn_Stop || !Btn_ShowTrajectory || !Slider_Timeline || !Text_TimeDisplay)
	{
		UE_LOG(LogTemp, Error, TEXT("[ReplayControlWidget] One or more widgets are not bound! Make sure all widgets are named correctly in the UMG Blueprint."));
		return;
	}

	// Lier les événements des boutons
	Btn_Play->OnClicked.AddDynamic(this, &UReplayControlWidget::OnPlayButtonClicked);
	Btn_Pause->OnClicked.AddDynamic(this, &UReplayControlWidget::OnPauseButtonClicked);
	Btn_Stop->OnClicked.AddDynamic(this, &UReplayControlWidget::OnStopButtonClicked);
	Btn_ShowTrajectory->OnClicked.AddDynamic(this, &UReplayControlWidget::OnShowTrajectoryButtonClicked);

	// Lier l'événement du slider
	Slider_Timeline->OnValueChanged.AddDynamic(this, &UReplayControlWidget::OnTimelineSliderValueChanged);

	UE_LOG(LogTemp, Log, TEXT("[ReplayControlWidget] UI Controls initialized successfully"));
}

void UReplayControlWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Mettre à jour l'affichage et le slider si un acteur est assigné
	if (TargetReplayActor)
	{
		// Mettre à jour le slider pour refléter la position actuelle
		if (!bIsUpdatingSlider && Slider_Timeline)
		{
			float Progress = TargetReplayActor->GetPlaybackProgress();
			Slider_Timeline->SetValue(Progress);
		}

		// Mettre à jour l'affichage du temps
		UpdateTimeDisplay();

		// Mettre à jour le texte du bouton Show Trajectory
		if (Btn_ShowTrajectory)
		{
			UTextBlock* ButtonText = Cast<UTextBlock>(Btn_ShowTrajectory->GetChildAt(0));
			if (ButtonText)
			{
				if (TargetReplayActor->bShowTrajectory)
				{
					ButtonText->SetText(FText::FromString(TEXT("👁 Hide Path")));
				}
				else
				{
					ButtonText->SetText(FText::FromString(TEXT("👁 Show Path")));
				}
			}
		}
	}
}

// ========== CALLBACKS DES BOUTONS ==========

void UReplayControlWidget::OnPlayButtonClicked()
{
	if (TargetReplayActor)
	{
		TargetReplayActor->Play();
		UE_LOG(LogTemp, Log, TEXT("[ReplayControlWidget] Play button clicked"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ReplayControlWidget] No TargetReplayActor assigned!"));
	}
}

void UReplayControlWidget::OnPauseButtonClicked()
{
	if (TargetReplayActor)
	{
		TargetReplayActor->Pause();
		UE_LOG(LogTemp, Log, TEXT("[ReplayControlWidget] Pause button clicked"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ReplayControlWidget] No TargetReplayActor assigned!"));
	}
}

void UReplayControlWidget::OnStopButtonClicked()
{
	if (TargetReplayActor)
	{
		TargetReplayActor->Stop();
		UE_LOG(LogTemp, Log, TEXT("[ReplayControlWidget] Stop button clicked"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ReplayControlWidget] No TargetReplayActor assigned!"));
	}
}

void UReplayControlWidget::OnShowTrajectoryButtonClicked()
{
	if (TargetReplayActor)
	{
		// Toggle la visibilité de la trajectoire
		TargetReplayActor->ToggleTrajectoryVisualization();
		UE_LOG(LogTemp, Log, TEXT("[ReplayControlWidget] Show Trajectory button clicked"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ReplayControlWidget] No TargetReplayActor assigned!"));
	}
}

void UReplayControlWidget::OnTimelineSliderValueChanged(float Value)
{
	if (TargetReplayActor && !bIsUpdatingSlider)
	{
		// Empêcher les boucles de mise à jour
		bIsUpdatingSlider = true;

		// Convertir la valeur 0-1 en pourcentage et l'appliquer
		TargetReplayActor->SeekToProgress(Value);

		bIsUpdatingSlider = false;

		UE_LOG(LogTemp, Verbose, TEXT("[ReplayControlWidget] Slider moved to %.2f%%"), Value * 100.0f);
	}
}

// ========== MISE À JOUR DE L'AFFICHAGE ==========

void UReplayControlWidget::UpdateTimeDisplay()
{
	if (TargetReplayActor && Text_TimeDisplay)
	{
		float CurrentTime = TargetReplayActor->CurrentPlaybackTime;
		float TotalTime = TargetReplayActor->TotalDuration;

		// Formater le texte : "12.5 / 30.0 s"
		FString TimeText = FString::Printf(TEXT("%.1f / %.1f s"), CurrentTime, TotalTime);
		Text_TimeDisplay->SetText(FText::FromString(TimeText));
	}
}