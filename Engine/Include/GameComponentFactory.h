#pragma once

class GameComponent;

class GameComponentFactory
{
public:
    virtual GameComponent* CreateComponent(unsigned int guid) = 0;
};