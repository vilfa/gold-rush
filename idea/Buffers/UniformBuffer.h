#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template <class T>
class UniformBuffer
{
public:
    UniformBuffer(uint32_t count, GLuint ubRange);
    ~UniformBuffer();
    void Data(T& data, uint32_t subStart = 0);

private:
    uint32_t ID;
    std::size_t sSize;
    uint32_t count;
    GLuint range;

    void generate();
    void bind();
    void allocate();
    void bindRange();
    void unbind();
    void remove();
};

template<class T>
inline UniformBuffer<T>::UniformBuffer(
    uint32_t count,
    GLuint ubRange
) : 
    sSize(sizeof(T)),
    count(count),
    range(ubRange)
{
    generate();
    bind();
    allocate();
    unbind();
    bindRange();
}

template<class T>
inline UniformBuffer<T>::~UniformBuffer()
{
    remove();
}

template<class T>
inline void UniformBuffer<T>::Data(T& data, uint32_t subStart)
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, subStart * sSize, sSize, glm::value_ptr(data));
    unbind();
}

template<class T>
inline void UniformBuffer<T>::generate()
{
    glGenBuffers(1, &ID);
}

template<class T>
inline void UniformBuffer<T>::bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
}

template<class T>
inline void UniformBuffer<T>::allocate()
{
    glBufferData(GL_UNIFORM_BUFFER, count * sSize, NULL, GL_STATIC_DRAW);
}

template<class T>
inline void UniformBuffer<T>::bindRange()
{
    glBindBufferRange(GL_UNIFORM_BUFFER, range, ID, 0, count * sSize);
}

template<class T>
inline void UniformBuffer<T>::unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<class T>
inline void UniformBuffer<T>::remove()
{
    glDeleteBuffers(1, &ID);
}
