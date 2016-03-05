#include "Components/TestComponent.h"

#include <stdio.h>

void TestComponent::OnCreate()
{
    printf("\t\tTestComponent OnCreate\n");
}

void TestComponent::OnStart()
{
    printf("\t\tTestComponent OnStart\n");
}

void TestComponent::Update(float deltaTime)
{

}