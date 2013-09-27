#pragma once

class Primitive
{
    virtual void Render() = 0;
};

class Sphere : public Primitive
{
    virtual void Render();
};

class Cube : public Primitive
{
    virtual void Render();
};

class Cylinder : public Primitive
{
    virtual void Render();
};