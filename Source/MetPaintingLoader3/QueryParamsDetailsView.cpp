// Fill out your copyright notice in the Description page of Project Settings.


#include "QueryParamsDetailsView.h"

#include "HttpModule.h"
#include "GenericPlatform/GenericPlatformHttp.h"

void UQueryParamsDetailsView::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	// This might be really, really bad.
	// NVM! WTF? I can't believe that worked! I'm so happy right now.
	SetObject(this);
}

FString UQueryParamsDetailsView::GenerateQueryParams()
{
	FString QueryParams;

	// Helper macro to append query parameters conditionally
	auto AppendParam = [&](const FString& Key, auto Value, bool UseFlag) {
		if (UseFlag)
		{
			if (!QueryParams.IsEmpty())
			{
				QueryParams += "&"; // Append '&' between parameters
			}
			QueryParams += FString::Printf(TEXT("%s=%s"), *Key, *FString::FromInt(Value));
		}
	};

	auto AppendStringParam = [&](const FString& Key, const FString& Value, bool UseFlag) {
		if (UseFlag)
		{
			if (!QueryParams.IsEmpty())
			{
				QueryParams += "&";
			}
			QueryParams += FString::Printf(TEXT("%s=%s"), *Key, *FGenericPlatformHttp::UrlEncode(Value));
		}
	};

	// Append parameters based on their "Use_" flags
	AppendStringParam("q", SearchParameters.q, SearchParameters.Use_q);
	AppendParam("isHighlight", SearchParameters.isHighlight, SearchParameters.Use_isHighlight);
	AppendParam("title", SearchParameters.title, SearchParameters.Use_title);
	AppendParam("tags", SearchParameters.tags, SearchParameters.Use_tags);
	AppendParam("departmentId", SearchParameters.departmentId, SearchParameters.Use_departmentId);
	AppendParam("isOnView", SearchParameters.isOnView, SearchParameters.Use_isOnView);
	AppendParam("artistOrCulture", SearchParameters.artistOrCulture, SearchParameters.Use_artistOrCulture);
	AppendStringParam("medium", SearchParameters.medium, SearchParameters.Use_medium);
	AppendParam("hasImages", SearchParameters.hasImages, SearchParameters.Use_hasImages);
	AppendStringParam("geoLocation", SearchParameters.geoLocation, SearchParameters.Use_geoLocation);
	AppendParam("dateBegin", SearchParameters.dateBegin, SearchParameters.Use_dateBegin);
	AppendParam("dateEnd", SearchParameters.dateEnd, SearchParameters.Use_dateEnd);

	return QueryParams;
}
