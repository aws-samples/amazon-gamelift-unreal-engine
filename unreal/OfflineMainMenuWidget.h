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
class GAMELIFT_UE4_API UOfflineMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOfflineMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void OnLoginClicked();

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString ApiGatewayEndpoint;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString LoginURI;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString StartSessionURI;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		FString user;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		FString pass;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
		FString IpAddress;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
		FString Port;

private:
	FHttpModule* Http;

	void LoginRequest(FString usr, FString pwd);
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void StartSessionRequest(FString idt);
	void OnStartSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
