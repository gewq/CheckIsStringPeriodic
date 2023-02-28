/**
\file main.cpp
\brief Программа проверяет является ли строка кратной заданному числу K.

Кратность — повторяемость строки каждые K символов.
Пример:
- строка "abcabcabcabc" - кратна 3 состоит из подстрок "abc", имеющих длину 3.
- строка "aaaaaaaaaa" - кратна:
	- 1 (подстрока "a")
	- 2 (подстрока "aa")
	- 5 (подстрока "aaaaa")
	- 10 (подстрока "aaaaaaaaaa" - кратна сама себе)
Ограничения:
- K должно быть > 0
- K должно быть <= длины строки
- Длина строки > 0
- Символы строки только латинские буквы и математические операторы
*/


#include <iostream>
#include "CheckString.h"	//Функция проверки кратности строки



int main()
{
	try {
		check_string::testIsPeriodic();
	}
	catch (const std::exception& error) {
		std::cerr << error.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Undefined exception" << std::endl;
	}

    return EXIT_SUCCESS;
}