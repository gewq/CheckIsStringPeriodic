/**
\file KEqualsZero_Exception.h
\brief ����� ZeroK_Exception - ���������� ���������� "K = 0"

����� KEqualsZero_Exception ��������� � ������ std::exception � ����������� ����� what()
*/

#pragma once

#include <string>
#include <exception>

class KEqualsZero_Exception : public std::exception {
    public:
        /**
        ����������� ��-���������
        */
        KEqualsZero_Exception();

        /**
        ������������� ����� �������� ������ std::exception
        \return ��������� �� ��������� �� ������
        */
        virtual const char* what() const noexcept override;
};