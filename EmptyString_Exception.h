/**
\file EmptyString_Exception.h
\brief ����� EmptyString_Exception - ��� ��������� ���������� "������ ������"

����� EmptyString_Exception ��������� � ������ std::exception � ����������� ����� what()
*/

#pragma once

#include <string>
#include <exception>

class EmptyString_Exception : public std::exception {
    public:
        /**
        ����������� ��-���������
        */
        EmptyString_Exception();

        /**
        ������������� ����� �������� ������ std::exception
        \return ��������� �� ��������� �� ������
        */
        virtual const char* what() const noexcept override;
};