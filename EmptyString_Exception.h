/**
\file EmptyString_Exception.h
\brief Класс EmptyString_Exception - для обработки исключения "Пустая строка"

Класс EmptyString_Exception наследует у класса std::exception и перегружает метод what()
*/

#pragma once

#include <string>
#include <exception>

class EmptyString_Exception : public std::exception {
    public:
        /**
        Конструктор по-умолчанию
        */
        EmptyString_Exception();

        /**
        Перегруженный метод базового класса std::exception
        \return Указатель на сообщение об ошибке
        */
        virtual const char* what() const noexcept override;
};