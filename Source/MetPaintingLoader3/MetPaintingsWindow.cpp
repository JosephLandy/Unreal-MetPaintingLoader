// Fill out your copyright notice in the Description page of Project Settings.


#include "MetPaintingsWindow.h"

#include "HttpModule.h"

void UMetPaintingsWindow::QueryAPI(FString search)
{
	auto Req = FHttpModule::Get().CreateRequest();
	Req->SetVerb("GET");
	// Req->SetOption()
	// Req->SetURL(URLBase + FString::FromInt(ObjectID));
}
