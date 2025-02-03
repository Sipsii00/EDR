// Copyright (c) 2025 Sipsii00 (MIT License).


#include "DirectionalRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDirectionalRenderComponent::UDirectionalRenderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UDirectionalRenderComponent::BeginPlay()
{
	Super::BeginPlay();

	WorldContext = Cast<UObject>(GetWorld());

	// Find and assign the player
	PlayerPawn = UGameplayStatics::GetPlayerPawn(WorldContext, 0);

	// Find ourselves :o
	MyActor = this->GetOwner();
}

// Called every frame
void UDirectionalRenderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsValid(SpriteCharacter) || IsValid(SpriteObject)) // Check if we have our 3D sprite referenced
	{
		if(PlayerPawn != nullptr) // Check if we have our player referenced
		{
			if(CurrentBook != nullptr && SpriteRenderType == ERenderType::IsAnimated) // Necessary for stopping Unreal from crashing if we forgot to assign something
			{
				if(!bBypassBillboard && BillboardMode == EBillboardType::IsCodeMethod)
					BillboardSprite(); // If we haven't bypassed this, run our "make sprite a billboard" function

				GetEnemyAngle(); // Run our angle calculations
				CalculateSprite(); // Run our "check which side" function
			}
			else if(!CurrentBook && SpriteRenderType == ERenderType::IsAnimated)
			{
				// Log error if we forgot to assign a "SpriteAnimationBook"
				DisplayError("SpriteAnimatedActor is missing: SpriteAnimationBook"); 
			}

			if(StaticBook != nullptr && SpriteRenderType == ERenderType::IsStatic) // Necessary for stopping Unreal from crashing if we forgot to assign something
			{
				if(!bBypassBillboard)
					BillboardSprite(); // If we haven't bypassed this, run our "make sprite a billboard" function
				
				CalculateSprite(); // Run our angle calculations
				GetEnemyAngle(); // Run our "check which side" function
			}
			else if(!StaticBook && SpriteRenderType == ERenderType::IsStatic)
			{;
				DisplayError("SpriteActor is wrongly setup! Missing: SpriteBook"); // Log error if we forgot to assign our "SpriteBook"
			}
		}
		else
		{
			DisplayError("No Player found!"); // Log error if we can't find the player
		}
	}
	else
	{
		if(!SpriteCharacter)
			DisplayError("No PaperSpriteComponent assigned!"); // Log error if we forgot to assign a sprite component
				
	}

	// Subtract from a book's animation duration until reached 0. Only useful for "One-Shot" animations.
	if(RemainingBookLength > 0)
		RemainingBookLength = FMath::Clamp((RemainingBookLength -= DeltaTime), 0.f, RemainingBookLength);
}

void UDirectionalRenderComponent::BillboardSprite()
{
	// Divided because we use two different types of PaperSpriteComponents depending on if static or animated is selected. 
	if(SpriteRenderType == ERenderType::IsAnimated)
	{
		// Find our look at rotation between player and ourselves, then update our sprite's rotation to "look at" the player. 
		FRotator YawRot = UKismetMathLibrary::FindLookAtRotation(SpriteCharacter->GetComponentLocation(), UGameplayStatics::GetPlayerCameraManager(WorldContext, 0)->GetCameraLocation());
		SpriteCharacter->SetWorldRotation(FRotator(0.f, YawRot.Yaw - 90.0f, 0.f));
	}
	else
	{
		// Find our look at rotation between player and ourselves, then update our sprite's rotation to "look at" the player. 
		FRotator YawRot = UKismetMathLibrary::FindLookAtRotation(SpriteObject->GetComponentLocation(), UGameplayStatics::GetPlayerCameraManager(WorldContext, 0)->GetCameraLocation());
		SpriteObject->SetWorldRotation(FRotator(0.f, YawRot.Yaw - 90.0f, 0.f));
	}
}

void UDirectionalRenderComponent::GetEnemyAngle()
{
	// Gets our forward vector, right vector, and location.
	// Then we use a dot product to get an angle between player and ourselves, which we run through a "DegAcos" function to get that angle in degrees.
	FVector MyForwardVector = FVector(MyActor->GetActorForwardVector().X, MyActor->GetActorForwardVector().Y, 0.f);
	FVector PlayerLoc = FVector(PlayerPawn->GetActorLocation().X, PlayerPawn->GetActorLocation().Y, 0.f);
	FVector MyLoc = FVector(MyActor->GetActorLocation().X, MyActor->GetActorLocation().Y, 0.f);
	FVector MyRightVector = FVector(MyActor->GetActorRightVector().X, MyActor->GetActorRightVector().Y, 0.f);

	float AngleDotProduct = UKismetMathLibrary::Dot_VectorVector(MyForwardVector,FVector(PlayerLoc - MyLoc).GetSafeNormal());
	Angle = UKismetMathLibrary::DegAcos(AngleDotProduct);

	// Based on the angle between us and the player, check if the player is on our right or left side.
	bLeftVisible = UKismetMathLibrary::Dot_VectorVector(FVector(PlayerLoc - MyLoc).GetSafeNormal(), MyRightVector) < 0.f;
}

void UDirectionalRenderComponent::CalculateSprite()
{
	// We check every 45 degrees which side we should display to the player.
	// we call two different (but same) functions based on our selected render type. 
	if(Angle < 22.5f)
	{
		if(SpriteRenderType == ERenderType::IsAnimated)
			SetFacingFlipbook(CurrentBook->Front, CurrentBook->Front);
		else
			SetFacingSprite(StaticBook->Front, StaticBook->Front);
	}
	else if (Angle < 67.5f)
	{
		if(SpriteRenderType == ERenderType::IsAnimated)
			SetFacingFlipbook(CurrentBook->Front_Left, CurrentBook->Front_Right);
		else
			SetFacingSprite(StaticBook->Front_Left, StaticBook->Front_Right);
	}
	else if(Angle < 112.5f)
	{
		if(SpriteRenderType == ERenderType::IsAnimated)
			SetFacingFlipbook(CurrentBook->Left, CurrentBook->Right);
		else
			SetFacingSprite(StaticBook->Left, StaticBook->Right);
	}
	else if(Angle < 157.5f)
	{
		if(SpriteRenderType == ERenderType::IsAnimated)
			SetFacingFlipbook(CurrentBook->Back_Left, CurrentBook->Back_Right);
		else
			SetFacingSprite(StaticBook->Back_Left, StaticBook->Back_Right);
	}
	else
	{
		if(SpriteRenderType == ERenderType::IsAnimated)
			SetFacingFlipbook(CurrentBook->Back, CurrentBook->Back);
		else
			SetFacingSprite(StaticBook->Back, StaticBook->Back);
	}
}

void UDirectionalRenderComponent::SetFacingFlipbook(UPaperFlipbook* Left, UPaperFlipbook* Right)
{
	// Here we set our animated sprite's sides based on our "CalculateSprite" function.
	
	//Store which frame we are currently at before we switch to a new angle.
	CurrentFrame = SpriteCharacter->GetPlaybackPosition();
	

	// Are the player on our left side?
	if(bLeftVisible)
	{
		// Here we check if our "current side" is the same as the "new side" provided.    (Side = front, back, back_left, etc..)
		
		// This is important because if we don't check this and just switch to a new side, one-shot animations resets everytime a new angle is assigned.
		// Example: your character begins an animation where they begin to do something. You walk slightly to the left and the whole animation reset to the first frame. 
		if(SpriteCharacter->GetFlipbook() != Left) 
		{
			SpriteCharacter->SetFlipbook(Left);
			SpriteCharacter->SetPlaybackPosition(CurrentFrame, false); // Set our new book's first frame to match the previous book's last frame.
		}
		else
		{
			SpriteCharacter->SetFlipbook(Left); // If the new side is the same as our current one, we skip setting previous frame. (introduces bugs instead)
		}
	}
	else
	{
		// Same as above, just for the right side if the player is not our left side.
		if(SpriteCharacter->GetFlipbook() != Right)
		{
			SpriteCharacter->SetFlipbook(Right);
			SpriteCharacter->SetPlaybackPosition(CurrentFrame, false);
		}
		else
		{
			SpriteCharacter->SetFlipbook(Right);
		}
	}
}

void UDirectionalRenderComponent::SetFacingSprite(UPaperSprite* Left, UPaperSprite* Right)
{
	// Here we set our static sprite's sides based on our "CalculateSprite" function.
	if(bLeftVisible)
	{
		SpriteObject->SetSprite(Left);
	}
	else
	{
		SpriteObject->SetSprite(Right);
	}
}

void UDirectionalRenderComponent::SetBook(USpriteAnimationBook* Book)
{
	// Don't set our book if it's empty. Prevents crashes :)
	if(!IsValid(Book))
		return;
	
	// We set our current book to the one assigned from this function's input parameter.
	CurrentBook = Book;
	
	// We reset our remaining value to the books full duration length.
	RemainingBookLength = GetBookLength(Book);

	// We set our sprite looping based of our animation book config.
	SpriteCharacter->SetLooping(Book->BookType == EBookAnimationType::IsBookLoop);
	
	// If the animation is of "one-shot" type, we reset our PaperSprite's playback to 0 to reassure we play our animation from the beginning.
	if(Book->BookType == EBookAnimationType::IsBookOneShot)
		SpriteCharacter->SetPlaybackPosition(0, false);

	//Play our animation book. (Especially important to call if previous book were a 'OneShot')
	SpriteCharacter->Play();
}

void UDirectionalRenderComponent::AssignActorSprite(UPaperFlipbookComponent* CharacterSprite, UPaperSpriteComponent* ObjectSprite)
{
	// We assign this component's actual 3D sprite.
	// You only have to assign the one you need since validation checks is implemented, you shouldn't get any errors or crashes regardless. (just an error log from me if you missed something >:) )
	
	if(IsValid(CharacterSprite))
		SpriteCharacter = CharacterSprite;

	if(IsValid(ObjectSprite))
		SpriteObject = ObjectSprite;
}

float UDirectionalRenderComponent::GetBookLength(USpriteAnimationBook* Book)
{
	return Book->Front->GetTotalDuration(); // We always use a book's "front" flipbook for reference, since that should always exist 
}

float UDirectionalRenderComponent::GetLengthRemaining()
{
	return RemainingBookLength; // Get the remaining duration left from our current book's play time
}

float UDirectionalRenderComponent::GetCurrentBookLength()
{
	return CurrentBook->Front->GetTotalDuration(); // We always use a book's "front" flipbook for reference, since that should always exist 
}

void UDirectionalRenderComponent::DisplayError(FString Message)
{
	// Just for debug purposes! :)
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.03f, FColor::Red, FString::Printf(TEXT("%s"), *Message));
	}
}




