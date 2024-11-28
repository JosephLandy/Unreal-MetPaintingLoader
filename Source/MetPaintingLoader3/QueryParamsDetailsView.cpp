// Fill out your copyright notice in the Description page of Project Settings.


#include "QueryParamsDetailsView.h"

void UQueryParamsDetailsView::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	// This might be really, really bad.
	// NVM! WTF? I can't believe that worked! I'm so happy right now.
	SetObject(this);
}
