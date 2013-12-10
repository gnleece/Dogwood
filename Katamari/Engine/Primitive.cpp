#include "Primitive.h"

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

// TODO (gnleece) OPENGL: use Vertex Buffer Objects instead of CallLists?

Primitive::Primitive() : m_compiled(false)
{ }