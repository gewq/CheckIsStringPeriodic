/**
\file KExceedsStringSize_Exception.h
\brief ����� KExceedsStringSize_Exception - ��� ��������� ���������� "K > ����� ������"

����� KExceedsStringSize_Exception ��������� � ������ std::exception � ����������� ����� what()
*/

#pragma once

#include <string>
#include <exception>

class KExceedsStringSize_Exception : public std::exception {
    public:
        /**
        ����������� ��-��������� �� ����� ������ - ������� �����.
        */
        KExceedsStringSize_Exception() = delete;

        /**
        ����������������� �����������.
        �� ������ ����������� �������� ��������� ������ K
        ��������� ��������� �� ������
        \param[in] K ��������� ������
        */
        explicit KExceedsStringSize_Exception(size_t K, size_t stringSize);

        /**
        ������������� ����� �������� ������ std::exception
        \return ��������� �� ��������� �� ������
        */
        virtual const char* what() const noexcept override;

    private:
        std::string errorMessage_;	///<��������� �� ������
};