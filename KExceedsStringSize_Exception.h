/**
\file KExceedsStringSize_Exception.h
\brief Класс KExceedsStringSize_Exception - для обработки исключения "K > длины строки"

Класс KExceedsStringSize_Exception наследует у класса std::exception и перегружает метод what()
*/

#pragma once

#include <string>
#include <exception>

class KExceedsStringSize_Exception : public std::exception {
    public:
        /**
        Конструктор по-умолчанию не имеет смысла - поэтому удалён.
        */
        KExceedsStringSize_Exception() = delete;

        /**
        Параметризованный конструктор.
        На основе переданного значения кратности строки K
        формирует сообщение об ошибке
        \param[in] K Кратность строки
        */
        explicit KExceedsStringSize_Exception(size_t K, size_t stringSize);

        /**
        Перегруженный метод базового класса std::exception
        \return Указатель на сообщение об ошибке
        */
        virtual const char* what() const noexcept override;

    private:
        std::string errorMessage_;	///<Сообщение об ошибке
};