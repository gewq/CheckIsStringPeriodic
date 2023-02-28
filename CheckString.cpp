#include "CheckString.h"

#include <assert.h>
#include <mutex>
#include <future>

//Классы - обработчики исключений
#include "KEqualsZero_Exception.h"
#include "EmptyString_Exception.h"
#include "KExceedsStringSize_Exception.h"


namespace {
    //Для многопоточной версии--------------------------------------
    /*
    Флаг - обозначающий, что найдено несоответствие шаблону.
    Используется для взаимодействия 2-х функций одновременно
    проверяющих строку с двух сторон.
    Если одна из функций нашла несоответствие - выставляет данный флаг чтобы
    остановить работу другой функции.
    */
    bool isMismatch = false;

    //Мьютекс для защиты доступа при модификации флага найденного несоответствия
    std::mutex mtx;
    //--------------------------------------------------------------
}


//Для многопоточной версии--------------------------------------
/**
МНОГОПОТОЧНАЯ версия функции проверки является ли строка кратной числу K.
\param[in] stringToCheck Строка, чью кратность проверить
\param[in] K Количество символов от начала строки которые должны повторяться
\return Признак кратности строки заданному числу
*/
static bool isPeriodicMultithread(const std::string& stringToCheck,
    const std::string& pattern, size_t K);

/**
Проверить является ли первая половина строки кратной числу K
\param[in] stringToCheck Строка, чью кратность проверить
\param[in] pattern Строка - шаблоно по которой проверить кратность
\param[in] K Количество символов от начала строки которые должны повторяться
\return Признак кратности строки заданному числу
*/
static bool checkFromHead(const std::string& stringToCheck,
    const std::string& pattern, size_t K);

/**
Проверить является ли вторая половина строки кратной числу K
\param[in] stringToCheck Строка, чью кратность проверить
\param[in] pattern Строка - шаблоно по которой проверить кратность
\param[in] K Количество символов от начала строки которые должны повторяться
\return Признак кратности строки заданному числу
*/
static bool checkFromTail(const std::string& stringToCheck,
    const std::string& pattern, size_t K);
//--------------------------------------------------------------



//Однопоточная версия-------------------------------------------
/**
ОДНОПОТОЧНАЯ версия фунцкции проверки является ли строка кратной числу K.
\param[in] stringToCheck Строка, чью кратность проверить
\param[in] K Количество символов от начала строки которые должны повторяться
\return Признак кратности строки заданному числу
*/
static bool isPeriodicSinglethread(const std::string& stringToCheck,
    const std::string& pattern, size_t K);
//--------------------------------------------------------------



bool check_string::isPeriodic(const std::string& stringToCheck, size_t K)
{
    //Обработка ошибок--------------------------------------------
    if (K == 0) {
        throw KEqualsZero_Exception();
    }
    if (stringToCheck.empty()) {
        throw EmptyString_Exception();
    }
    if (K > stringToCheck.size()) {
        throw KExceedsStringSize_Exception(K, stringToCheck.size());
    }

    //Основная логика---------------------------------------------
    //K равно длине строки -> строка КРАТНА (сама себе)
    if (K == stringToCheck.size()) {
        return true;
    }
    //K не кратно длине строки -> строка НЕ кратна K
    //(шаблон не уложится в длину строки)
    if (stringToCheck.size() % K != 0) {
        return false;
    }
    if (stringToCheck.size() > 1) {
        //K больше чем половина длины строки -> строка НЕ может быть кратна K
        if (K > stringToCheck.size() / 2) {
            return false;
        }
    }

    //Определить шаблон который должен повторяться для всей строки
    //Первые K символов строки
    const std::string pattern = stringToCheck.substr(0, K);

    //Количество ядер > 1 - вызов многопоточной версии
    if (std::thread::hardware_concurrency() > 1) {
        return isPeriodicMultithread(stringToCheck, pattern, K);
    }
    //Ядро 1 или их количество нельзя определить - однопоточная версия
    else {
        return isPeriodicSinglethread(stringToCheck, pattern, K);
    }

    return false;
}



static bool isPeriodicSinglethread(const std::string& stringToCheck,
    const std::string& pattern, size_t K)
{
    //Берём попарно подстроку со стороны начала и со стороны конца
    //и сравниваем с шаблоном
    size_t headSubstringID = K;	// Начинаем с K потому что первые K символов - шаблон
    size_t tailSubstringID = stringToCheck.size();
    //Проверять пока не встретятся
    while (headSubstringID < tailSubstringID) {
        //Взять попарно подстроку со стороны начала и со стороны конца строки
        const std::string substringFromHead = stringToCheck.substr(headSubstringID, K);
        const std::string substringFromTail = stringToCheck.substr(tailSubstringID - K, K);

        //Если подстрока равна шаблону проверить следующие K символов
        if ((pattern == substringFromHead) && (pattern == substringFromTail)) {
            headSubstringID += K;
            tailSubstringID -= K;
        }

        //Если подстрока не равна шаблону -> строка НЕ кратна дальше проверять не нужно
        else {
            return false;
        }
    }

    //<- Если дошли сюда -> строка КРАТНА K
    return true;
}



static bool isPeriodicMultithread(const std::string& stringToCheck,
    const std::string& pattern, size_t K)
{
    //Флаг - глобальная переменная, поэтому при повторных вызовах его
    //нужно сбрасывать, иначе его значение останется неизменным с прошлого вызова
    isMismatch = false;

    //Одновременный проход от начала к концу и от конца к началу в двух потоках
    std::future<bool> resultFromHead = std::async(checkFromHead, std::ref(stringToCheck), std::ref(pattern), K);
    std::future<bool> resultFromTail = std::async(checkFromTail, std::ref(stringToCheck), std::ref(pattern), K);

    //Строка кратна при проверке от начала до конца
    if ((resultFromHead.get() == true) && (resultFromTail.get() == true)) {
        return true;
    }
    else {
        return false;
    }

    //<- Если дошли сюда -> строка КРАТНА K
    return true;
}



static bool checkFromHead(const std::string& stringToCheck,
    const std::string& pattern, size_t K)
{
    //Логика работы:
    //Двигаясь от начала к концу строки проверить каждые K символов строки на
    //соответствие шаблону
    //Если найдено несоответствие шаблону - сообщить функции которая проверяет
    //от конца к началу, что дальше проверять не нужно

    //Индекс элемента строки - начало очередной подстроки которую проверить
    size_t headSubstringID = K;	//Первые K символов сам шаблон -> их не проверяем

    //Проверять до середины строки ИЛИ пока вторая функция не сообщит,
    //что нашла несоответствие
    while ((headSubstringID <= stringToCheck.size() / 2) && (isMismatch == false)) {
        const std::string substring = stringToCheck.substr(headSubstringID, K);

        //Если подстрока равна шаблону - проверить следующие K символов
        if (pattern == substring) {
            headSubstringID += K;
        }

        //Если подстрока не равна шаблону -> строка НЕ кратна
        else {
            //Сообщить второй функции, что дальше проверять не надо
            //Защита модификации разделяемых данных=======================
            std::lock_guard<std::mutex> guard(mtx);	//Захватить mutex
            isMismatch = true;
            //<- Mutex освободится здесь деструктором lock_guard
            return false;
        }
    }

    //<- Дошли сюда -> несоответствий НЕ найдено
    return true;
}



static bool checkFromTail(const std::string& stringToCheck,
    const std::string& pattern,
    size_t K)
{
    //Логика работы:
    //Двигаясь от конца к началу строки проверить каждые K символов на соответствие шаблону
    //Если найдено несоответствие шаблону - сообщить функции которая проверяет от начала к концу,
    //что дальше проверять не нужно

    //Индекс элемента строки - конец очередной подстроки которую проверить
    size_t tailSubstringID = stringToCheck.size();	//Начинаем проверять с конца строки

    //Проверять до середины строки ИЛИ пока вторая функция не сообщит,
    //что нашла несоответствие
    while ((tailSubstringID >= stringToCheck.size() / 2) && (isMismatch == false)) {
        const std::string substring = stringToCheck.substr(tailSubstringID - K, K);

        //Если подстрока равна шаблону проверить следующие K символов
        if (pattern == substring) {
            tailSubstringID -= K;	//Идём к началу строки
        }

        //Если не равна шаблону -> строка НЕ кратна
        else {
            //Сообщить второй функции, что дальше проверять не надо
            //Защита модификации разделяемых данных=======================
            std::lock_guard<std::mutex> guard(mtx);	//Захватить mutex
            isMismatch = true;
            //<- Mutex освободится здесь деструктором lock_guard
            return false;
        }
    }

    //<- Дошли сюда -> несоответствий НЕ найдено
    return true;
}



void check_string::testIsPeriodic()
{
    //K == длине строки -> строка КРАТНА
    assert(isPeriodic("a", 1) == true);
    assert(isPeriodic("abcdefg", 7) == true);
    assert(isPeriodic("aaaaaaaaaa", 10) == true);
    assert(isPeriodic("foobarfoobar", 12) == true);

    //K НЕ кратно длине строки -> строка НЕ кратна
    assert(isPeriodic("abababab", 3) == false);
    assert(isPeriodic("aaaaaaaaaa", 3) == false);
    assert(isPeriodic("aaaaaaaaaa", 4) == false);

    //K > половина длины строки -> строка НЕ кратна
    assert(isPeriodic("abababab", 5) == false);
    assert(isPeriodic("aaaaaaaaaa", 6) == false);

    //Различная кратность при разных K
    assert(isPeriodic("aaaaaaaaaa", 1) == true);
    assert(isPeriodic("aaaaaaaaaa", 2) == true);
    assert(isPeriodic("aaaaaaaaaa", 5) == true);
    assert(isPeriodic("aaaaaQQaaaaa", 2) == false);

    assert(isPeriodic("abcabcabcabc", 1) == false);
    assert(isPeriodic("abcabcabcabc", 2) == false);
    assert(isPeriodic("abcabcabcabc", 3) == true);
    assert(isPeriodic("abcabcabcabc", 4) == false);
    assert(isPeriodic("abcabcabcabc", 5) == false);
    assert(isPeriodic("abcabcabcabc", 6) == true);
    assert(isPeriodic("abcabcabcabc", 12) == true);

    assert(isPeriodic("foobarfoobar", 1) == false);
    assert(isPeriodic("foobarfoobar", 2) == false);
    assert(isPeriodic("foobarfoobar", 3) == false);
    assert(isPeriodic("foobarfoobar", 4) == false);
    assert(isPeriodic("foobarfoobar", 5) == false);
    assert(isPeriodic("foobarfoobar", 6) == true);

    //Тест выброс исключения
    //isPeriodic("a", 0);	//K = 0
    //isPeriodic("a", 12);	//K > длины строки
    //isPeriodic("", 1);	//Пустая строка
}