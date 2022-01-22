// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: MIT-0

#pragma once

#include "Http.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OfflineMainMenuWidget.generated.h"
/**
 * 
 */
UCLASS()

//TODO change GAMELIFT_UE4_API to <YOUR_PROJECT_NAME>_API
class GAMELIFT_UE4_API UOfflineMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOfflineMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void OnLoginClicked();

	UPROPERTY(EditAnywhere)
		FString ApiGatewayEndpoint;

	UPROPERTY(EditAnywhere)
		FString LoginURI;

	UPROPERTY(EditAnywhere)
		FString StartSessionURI;

	UPROPERTY(BluePrintReadWrite)
		FString user;

	UPROPERTY(BluePrintReadWrite)
		FString pass;


private:
	FHttpModule* Http;

	void LoginRequest(FString usr, FString pwd);
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void StartSessionRequest(FString idt);
	void OnStartSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
