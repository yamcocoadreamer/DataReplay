#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReplayControlWidget.generated.h"

// Forward declarations
class UButton;
class USlider;
class UTextBlock;

// Forward declaration
class ATrajectoryReplayActor;

/**
 * Widget UI pour contrôler la lecture de trajectoire
 */
UCLASS()
class DATAREPLAY_API UReplayControlWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ========== WIDGETS UI (liés depuis le Blueprint) ==========

	// Bouton Play
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Play;

	// Bouton Pause
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Pause;

	// Bouton Stop
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Stop;

	// Bouton pour afficher/cacher la trajectoire
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_ShowTrajectory;

	// Slider de timeline
	UPROPERTY(meta = (BindWidget))
	USlider* Slider_Timeline;

	// Texte d'affichage du temps
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_TimeDisplay;

	// ========== RÉFÉRENCE À L'ACTEUR ==========

	// Référence à l'acteur de replay à contrôler
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replay Control")
	ATrajectoryReplayActor* TargetReplayActor;

protected:
	// Appelé lors de l'initialisation du widget
	virtual void NativeConstruct() override;

	// Appelé chaque frame
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// ========== CALLBACKS DES BOUTONS ==========

	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnPauseButtonClicked();

	UFUNCTION()
	void OnStopButtonClicked();

	UFUNCTION()
	void OnShowTrajectoryButtonClicked();

	UFUNCTION()
	void OnTimelineSliderValueChanged(float Value);

	// ========== FONCTIONS INTERNES ==========

	// Mettre à jour l'affichage du temps
	void UpdateTimeDisplay();

	// Flag pour éviter les boucles infinies lors du changement de slider
	bool bIsUpdatingSlider;
};