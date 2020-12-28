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
    UniformBuffer(uint32_t count, GLuint ub_range);
    ~UniformBuffer();
    void Data(T& data, uint32_t sub_start = 0);

private:
    uint32_t id_;
    std::size_t single_size_;
    uint32_t el_count_;
    GLuint range_;

    void generate();
    void bind();
    void allocate();
    void bindRange();
    void unbind();
    void remove();
};

template<class T>
inline UniformBuffer<T>::UniformBuffer(uint32_t count, GLuint ub_range) : 
    single_size_(sizeof(T)),
    el_count_(count),
    range_(ub_range)
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
inline void UniformBuffer<T>::Data(T& data, uint32_t sub_start)
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, sub_start * single_size_, single_size_, glm::value_ptr(data));
    unbind();
}

template<class T>
inline void UniformBuffer<T>::generate()
{
    glGenBuffers(1, &id_);
}

template<class T>
inline void UniformBuffer<T>::bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
}

template<class T>
inline void UniformBuffer<T>::allocate()
{
    glBufferData(GL_UNIFORM_BUFFER, el_count_ * single_size_, NULL, GL_STATIC_DRAW);
}

template<class T>
inline void UniformBuffer<T>::bindRange()
{
    glBindBufferRange(GL_UNIFORM_BUFFER, range_, id_, 0, el_count_ * single_size_);
}

template<class T>
inline void UniformBuffer<T>::unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<class T>
inline void UniformBuffer<T>::remove()
{
    glDeleteBuffers(1, &id_);
}
