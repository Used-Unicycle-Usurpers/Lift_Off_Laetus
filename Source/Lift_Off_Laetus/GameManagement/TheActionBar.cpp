// Fill out your copyright notice in the Description page of Project Settings.


//#include "GameManagement/TheActionBar.h"
#include "TheActionBar.h"

void UTheActionBar::UpdateActionBar(float newValue) { 
	if(ActionBar != nullptr)
		ActionBar->SetPercent(newValue); 
}

