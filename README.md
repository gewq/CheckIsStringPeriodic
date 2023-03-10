### О программе
---
- Программа представляет собой функцию проверки строки символов на кратность заданному числу $K$
- Кратность - повторяемость строки каждые $K$ символов - т.е. соединяя первые $K$ символов можно получить исходную строку
- Пример:	
	- строка "abcabcabcabc" - кратна:
		- 3 состоит из подстрок "abc", имеющих длину 3.
		- 6 (подстрока "abcabc")
		- 12 (подстрока "abcabcabcabc" - кратна сама себе)
	- строка "aaaaaaaaaa" - кратна:
		- 1 (подстрока "a")
		- 2 (подстрока "aa")
		- 5 (подстрока "aaaaa")
		- 10 (подстрока "aaaaaaaaaa" - кратна сама себе)


### Ограничения
---
- K должно быть > 0
- K должно быть <= длины строки
- Длина строки > 0
- Символы строки только латинские буквы и математические операторы (Кириллица запрещена)


### Принцип работы
---
- Двигаясь с двух концов строки навстречу - проверяем каждые $K$ символов на равенство шаблону
- В качестве шаблона используется подстрока - первые $K$ символов
- Как только будет найдено первое несоответствие шаблону - проверка прекращается (строка НЕ кратна $K$)
- Если несоответствий шаблону не найдено - значит строка КРАТНА $K$


### Особенности
---
- Проверка с конца строки позволяет быстрее определить некратность строки, особенно для длинных строк
- Для ускорения работы программы реализована многопоточная версия, позволяющая проверять строку одновременно с двух концов
- Выбор одно- или много- поточной версии осуществляется автоматически на основе информации о количестве вычислительных ядер 


### Описание функций
---
- Модуль `CheckString`
	- содержит функцию проверки является ли строка кратной заданному числу $K$
	- содержит функцию тестирования проверки строки на кратность
- Класс `EmptyString_Exception` - обработчик исключения "Пустая строка"
- Класс `KExceedsStringSize_Exception` - обработчик исключения " $K$ превышает длину строки"
- Класс `KEqualsZero_Exception` - обработчик исключения " $K$ равно 0"
