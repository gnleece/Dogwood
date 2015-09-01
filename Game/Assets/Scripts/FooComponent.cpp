#include "FooComponent.h"

#include <stdio.h>

void FooComponent::OnCreate()
{
    printf("\t\tFooComponent OnCreate\n");
}

void FooComponent::OnStart()
{
    printf("\t\tFooComponent OnStart\n");
}