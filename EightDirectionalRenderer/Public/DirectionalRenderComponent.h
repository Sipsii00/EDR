// Copyright (c) 2025 Sipsii00 (MIT License).

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "SpriteAnimationBook.h"
#include "SpriteBook.h"
#include "GameFramework/Pawn.h"
#include "DirectionalRenderComponent.generated.h"

UENUM() // Make enum for different sprite methods
enum ERenderType
{
	IsAnimated UMETA(DisplayName="Animated"),
	IsStatic UMETA(DisplayName="Static"),
};

UENUM() // Make enum for different billboarding types
enum EBillboardType
{
	IsMaterialMethod UMETA(DisplayName="Material"),
	IsCodeMethod UMETA(DisplayName="Code"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class EIGHTDIRECTIONALRENDERER_API UDirectionalRenderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDirectionalRenderComponent();

	// Enum for switching between animated and static sprite actors.
	// * 'Static' will use book type: SpriteBook.
	// * 'Animated' will use book type: SpriteAnimationBook.
	UPROPERTY(EditAnywhere, Category=Renderer)
	TEnumAsByte<ERenderType> SpriteRenderType = ERenderType::IsAnimated;
	
	// Enum for switching between material and code based billboard-ing method.
	// * 'Material' will use the material for handling the billboard effect.
	// * 'Code' will use the code calculations included in this component.
	UPROPERTY(EditAnywhere, Category=Renderer)
	TEnumAsByte<EBillboardType> BillboardMode = EBillboardType::IsCodeMethod;

	
	// Here we can assign our "SpriteBook" data asset if actor is of render type 'Static'. This is what our paper sprite will display in-game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Renderer)
	USpriteBook* StaticBook; 

	// Boolean for switching off billboard effect. Mostly for debugging purposes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Renderer)
	bool bBypassBillboard = false;

	// Here we store our current animation book in use. Can be useful info at runtime!
	UPROPERTY(BlueprintReadOnly, Category=Renderer)
	USpriteAnimationBook* CurrentBook;

private:

	// Store our actor's Animated PaperComponent. This is the actual 3D sprite within' our actor.
	UPROPERTY()
	UPaperFlipbookComponent* SpriteCharacter; 

	// Store our actor's Static PaperComponent. This is the actual 3D sprite within' our actor.
	UPROPERTY()
	UPaperSpriteComponent* SpriteObject; 

	// Store the current angle between this sprite actor and the player.
	UPROPERTY() 
	float Angle; 

	// Boolean for checking if the player currently is on the left or the right side.
	UPROPERTY()
	bool bLeftVisible; 

	// Store our current playing flipbook frame. This is for syncing animation between angles. (If we skip this, the animation resets each angle) 
	UPROPERTY()
	float CurrentFrame;

	// Stores this actor for angle calculations.
	UPROPERTY()
	AActor* MyActor; 

	// Stores our player.
	UPROPERTY()
	APawn* PlayerPawn; 

	// Stores the remaining animation length after it begins playing.
	UPROPERTY()
	float RemainingBookLength;

	// World Context for handling runtime "GetWorld" without plugin compilation errors..
	const UObject* WorldContext;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void BillboardSprite(); // Rotate sprite to always face the player's view.
	void GetEnemyAngle(); // Calculate the current angle (in degrees) between this actor and the player.
	void CalculateSprite(); // Checks which side to display (front, back, left, etc..) based on our angle.

	// Assign the side to display via our animated sprite character.
	void SetFacingFlipbook(UPaperFlipbook* Left, UPaperFlipbook* Right);
	
	// Assign the side to display via our static sprite object.
	void SetFacingSprite(UPaperSprite* Left, UPaperSprite* Right);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Should be called at BeginPlay to assign our 3D sprite to this component.
	// If done via C++, an example of assigning a "static actor": AssignActorSprite(nullptr, ExampleActor);
	UFUNCTION(BlueprintCallable, Category=Renderer)
	void AssignActorSprite(UPaperFlipbookComponent* CharacterSprite, UPaperSpriteComponent* ObjectSprite); 

	// Call this to set an animation to play. (Only if render type: animated)
	UFUNCTION(BlueprintCallable, Category=Animation)
	void SetBook(USpriteAnimationBook* Book);

	// Function to get current animations remaining duration.
	UFUNCTION(BlueprintPure, Category=Animation)
	float GetLengthRemaining();

	// Function to get a book's total animation duration length
	UFUNCTION(BlueprintPure, Category=Animation)
	float GetBookLength(USpriteAnimationBook* Book);

	// Function to get our actor's current book's total animation duration length
	UFUNCTION(BlueprintPure, Category=Animation)
	float GetCurrentBookLength();

	// Simple function to display an error if something was forgotten. (is needed when you're tired :/ )
	UFUNCTION()
	void DisplayError(FString Message);
};
