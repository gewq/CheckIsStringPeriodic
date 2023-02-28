/**
\file KEqualsZero_Exception.h
\brief Класс ZeroK_Exception - обработчик исключения "K = 0"

Класс KEqualsZero_Exception наследует у класса std::exception и перегружает метод what()
*/

#pragma once

#include <string>
#include <exception>

class KEqualsZero_Exception : public std::exception {
    public:
        /**
        Конструктор по-умолчанию
        */
        KEqualsZero_Exception();

        /**
        Перегруженный метод базового класса std::exception
        \return Указатель на сообщение об ошибке
        */
        virtual const char* what() const noexcept override;
};