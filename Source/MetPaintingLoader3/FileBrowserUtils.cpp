// Fill out your copyright notice in the Description page of Project Settings.


#include "FileBrowserUtils.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"

void UFileBrowserUtils::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames) {
	const void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		uint32 SelectionFlag = 1; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
		DesktopPlatform->OpenFileDialog(ParentWindowPtr, DialogTitle, DefaultPath, FString(""), FileTypes, EFileDialogFlags::None, OutFileNames);
	}
}

void UFileBrowserUtils::OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolder)
{
	const void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		DesktopPlatform->OpenDirectoryDialog(ParentWindowPtr, DialogTitle, DefaultPath, OutFolder);
	}
}
