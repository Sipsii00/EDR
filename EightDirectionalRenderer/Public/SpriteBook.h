// Copyright (c) 2025 Sipsii00 (MIT License).

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "SpriteBook.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EIGHTDIRECTIONALRENDERER_API USpriteBook : public UDataAsset
{
	GENERATED_BODY()
public:
	
	// Properties for each sprite side angle we should display
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Front)
	UPaperSprite* Front;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Front)
	UPaperSprite* Front_Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Front)
	UPaperSprite* Front_Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Side)
	UPaperSprite* Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Side)
	UPaperSprite* Right;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Back)
	UPaperSprite* Back;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Back)
	UPaperSprite* Back_Left;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Back)
	UPaperSprite* Back_Right;
};
