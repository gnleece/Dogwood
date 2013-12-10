#pragma once

class Primitive
{
public:
    Primitive();
    virtual void Render() = 0;

protected:
    bool m_compiled;
};

class Sphere : public Primitive
{
public:
    virtual void Render();

private:
    int m_listID;
};

class Cube : public Primitive
{
public:
    virtual void Render();
};

class Cylinder : public Primitive
{
public:
    virtual void Render();
};