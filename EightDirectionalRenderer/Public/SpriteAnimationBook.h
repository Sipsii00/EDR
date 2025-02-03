// Copyright (c) 2025 Sipsii00 (MIT License).

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "SpriteAnimationBook.generated.h"

UENUM(BlueprintType)

enum EBookAnimationType // Make Enum
{
	IsBookLoop UMETA(DisplayName="Loop"),
	IsBookOneShot UMETA(DisplayName="One Shot"),
};


/**
 * 
 */
UCLASS(BlueprintType)
class EIGHTDIRECTIONALRENDERER_API USpriteAnimationBook : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// Enum for animation types: Loop or One-Shot.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=LoopMode)
	TEnumAsByte<EBookAnimationType> BookType = EBookAnimationType::IsBookLoop;
	
	// Properties for each sprite side angle we should display
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Front)
	UPaperFlipbook* Front;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Front)
	UPaperFlipbook* Front_Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Front)
	UPaperFlipbook* Front_Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Side)
	UPaperFlipbook* Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Side)
	UPaperFlipbook* Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Back)
	UPaperFlipbook* Back;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Back)
	UPaperFlipbook* Back_Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Back)
	UPaperFlipbook* Back_Right;
};
