#include "CheckString.h"

#include <assert.h>
#include <mutex>
#include <future>

//������ - ����������� ����������
#include "KEqualsZero_Exception.h"
#include "EmptyString_Exception.h"
#include "KExceedsStringSize_Exception.h"


namespace {
    //��� ������������� ������--------------------------------------
    /*
    ���� - ������������, ��� ������� �������������� �������.
    ������������ ��� �������������� 2-� ������� ������������
    ����������� ������ � ���� ������.
    ���� ���� �� ������� ����� �������������� - ���������� ������ ���� �����
    ���������� ������ ������ �������.
    */
    bool isMismatch = false;

    //������� ��� ������ ������� ��� ����������� ����� ���������� ��������������
    std::mutex mtx;
    //--------------------------------------------------------------
}


//��� ������������� ������--------------------------------------
/**
������������� ������ ������� �������� �������� �� ������ ������� ����� K.
\param[in] stringToCheck ������, ��� ��������� ���������
\param[in] K ���������� �������� �� ������ ������ ������� ������ �����������
\return ������� ��������� ������ ��������� �����
*/
static bool isPeriodicMultithread(const std::string& stringToCheck,
    const std::string& pattern, size_t K);

/**
��������� �������� �� ������ �������� ������ ������� ����� K
\param[in] stringToCheck ������, ��� ��������� ���������
\param[in] pattern ������ - ������� �� ������� ��������� ���������
\param[in] K ���������� �������� �� ������ ������ ������� ������ �����������
\return ������� ��������� ������ ��������� �����
*/
static bool checkFromHead(const std::string& stringToCheck,
    const std::string& pattern, size_t K);

/**
��������� �������� �� ������ �������� ������ ������� ����� K
\param[in] stringToCheck ������, ��� ��������� ���������
\param[in] pattern ������ - ������� �� ������� ��������� ���������
\param[in] K ���������� �������� �� ������ ������ ������� ������ �����������
\return ������� ��������� ������ ��������� �����
*/
static bool checkFromTail(const std::string& stringToCheck,
    const std::string& pattern, size_t K);
//--------------------------------------------------------------



//������������ ������-------------------------------------------
/**
������������ ������ �������� �������� �������� �� ������ ������� ����� K.
\param[in] stringToCheck ������, ��� ��������� ���������
\param[in] K ���������� �������� �� ������ ������ ������� ������ �����������
\return ������� ��������� ������ ��������� �����
*/
static bool isPeriodicSinglethread(const std::string& stringToCheck,
    const std::string& pattern, size_t K);
//--------------------------------------------------------------



bool check_string::isPeriodic(const std::string& stringToCheck, size_t K)
{
    //��������� ������--------------------------------------------
    if (K == 0) {
        throw KEqualsZero_Exception();
    }
    if (stringToCheck.empty()) {
        throw EmptyString_Exception();
    }
    if (K > stringToCheck.size()) {
        throw KExceedsStringSize_Exception(K, stringToCheck.size());
    }

    //�������� ������---------------------------------------------
    //K ����� ����� ������ -> ������ ������ (���� ����)
    if (K == stringToCheck.size()) {
        return true;
    }
    //K �� ������ ����� ������ -> ������ �� ������ K
    //(������ �� �������� � ����� ������)
    if (stringToCheck.size() % K != 0) {
        return false;
    }
    if (stringToCheck.size() > 1) {
        //K ������ ��� �������� ����� ������ -> ������ �� ����� ���� ������ K
        if (K > stringToCheck.size() / 2) {
            return false;
        }
    }

    //���������� ������ ������� ������ ����������� ��� ���� ������
    //������ K �������� ������
    const std::string pattern = stringToCheck.substr(0, K);

    //���������� ���� > 1 - ����� ������������� ������
    if (std::thread::hardware_concurrency() > 1) {
        return isPeriodicMultithread(stringToCheck, pattern, K);
    }
    //���� 1 ��� �� ���������� ������ ���������� - ������������ ������
    else {
        return isPeriodicSinglethread(stringToCheck, pattern, K);
    }

    return false;
}



static bool isPeriodicSinglethread(const std::string& stringToCheck,
    const std::string& pattern, size_t K)
{
    //���� ������� ��������� �� ������� ������ � �� ������� �����
    //� ���������� � ��������
    size_t headSubstringID = K;	// �������� � K ������ ��� ������ K �������� - ������
    size_t tailSubstringID = stringToCheck.size();
    //��������� ���� �� ����������
    while (headSubstringID < tailSubstringID) {
        //����� ������� ��������� �� ������� ������ � �� ������� ����� ������
        const std::string substringFromHead = stringToCheck.substr(headSubstringID, K);
        const std::string substringFromTail = stringToCheck.substr(tailSubstringID - K, K);

        //���� ��������� ����� ������� ��������� ��������� K ��������
        if ((pattern == substringFromHead) && (pattern == substringFromTail)) {
            headSubstringID += K;
            tailSubstringID -= K;
        }

        //���� ��������� �� ����� ������� -> ������ �� ������ ������ ��������� �� �����
        else {
            return false;
        }
    }

    //<- ���� ����� ���� -> ������ ������ K
    return true;
}



static bool isPeriodicMultithread(const std::string& stringToCheck,
    const std::string& pattern, size_t K)
{
    //���� - ���������� ����������, ������� ��� ��������� ������� ���
    //����� ����������, ����� ��� �������� ��������� ���������� � �������� ������
    isMismatch = false;

    //������������� ������ �� ������ � ����� � �� ����� � ������ � ���� �������
    std::future<bool> resultFromHead = std::async(checkFromHead, std::ref(stringToCheck), std::ref(pattern), K);
    std::future<bool> resultFromTail = std::async(checkFromTail, std::ref(stringToCheck), std::ref(pattern), K);

    //������ ������ ��� �������� �� ������ �� �����
    if ((resultFromHead.get() == true) && (resultFromTail.get() == true)) {
        return true;
    }
    else {
        return false;
    }

    //<- ���� ����� ���� -> ������ ������ K
    return true;
}



static bool checkFromHead(const std::string& stringToCheck,
    const std::string& pattern, size_t K)
{
    //������ ������:
    //�������� �� ������ � ����� ������ ��������� ������ K �������� ������ ��
    //������������ �������
    //���� ������� �������������� ������� - �������� ������� ������� ���������
    //�� ����� � ������, ��� ������ ��������� �� �����

    //������ �������� ������ - ������ ��������� ��������� ������� ���������
    size_t headSubstringID = K;	//������ K �������� ��� ������ -> �� �� ���������

    //��������� �� �������� ������ ��� ���� ������ ������� �� �������,
    //��� ����� ��������������
    while ((headSubstringID <= stringToCheck.size() / 2) && (isMismatch == false)) {
        const std::string substring = stringToCheck.substr(headSubstringID, K);

        //���� ��������� ����� ������� - ��������� ��������� K ��������
        if (pattern == substring) {
            headSubstringID += K;
        }

        //���� ��������� �� ����� ������� -> ������ �� ������
        else {
            //�������� ������ �������, ��� ������ ��������� �� ����
            //������ ����������� ����������� ������=======================
            std::lock_guard<std::mutex> guard(mtx);	//��������� mutex
            isMismatch = true;
            //<- Mutex ����������� ����� ������������ lock_guard
            return false;
        }
    }

    //<- ����� ���� -> �������������� �� �������
    return true;
}



static bool checkFromTail(const std::string& stringToCheck,
    const std::string& pattern,
    size_t K)
{
    //������ ������:
    //�������� �� ����� � ������ ������ ��������� ������ K �������� �� ������������ �������
    //���� ������� �������������� ������� - �������� ������� ������� ��������� �� ������ � �����,
    //��� ������ ��������� �� �����

    //������ �������� ������ - ����� ��������� ��������� ������� ���������
    size_t tailSubstringID = stringToCheck.size();	//�������� ��������� � ����� ������

    //��������� �� �������� ������ ��� ���� ������ ������� �� �������,
    //��� ����� ��������������
    while ((tailSubstringID >= stringToCheck.size() / 2) && (isMismatch == false)) {
        const std::string substring = stringToCheck.substr(tailSubstringID - K, K);

        //���� ��������� ����� ������� ��������� ��������� K ��������
        if (pattern == substring) {
            tailSubstringID -= K;	//��� � ������ ������
        }

        //���� �� ����� ������� -> ������ �� ������
        else {
            //�������� ������ �������, ��� ������ ��������� �� ����
            //������ ����������� ����������� ������=======================
            std::lock_guard<std::mutex> guard(mtx);	//��������� mutex
            isMismatch = true;
            //<- Mutex ����������� ����� ������������ lock_guard
            return false;
        }
    }

    //<- ����� ���� -> �������������� �� �������
    return true;
}



void check_string::testIsPeriodic()
{
    //K == ����� ������ -> ������ ������
    assert(isPeriodic("a", 1) == true);
    assert(isPeriodic("abcdefg", 7) == true);
    assert(isPeriodic("aaaaaaaaaa", 10) == true);
    assert(isPeriodic("foobarfoobar", 12) == true);

    //K �� ������ ����� ������ -> ������ �� ������
    assert(isPeriodic("abababab", 3) == false);
    assert(isPeriodic("aaaaaaaaaa", 3) == false);
    assert(isPeriodic("aaaaaaaaaa", 4) == false);

    //K > �������� ����� ������ -> ������ �� ������
    assert(isPeriodic("abababab", 5) == false);
    assert(isPeriodic("aaaaaaaaaa", 6) == false);

    //��������� ��������� ��� ������ K
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

    //���� ������ ����������
    //isPeriodic("a", 0);	//K = 0
    //isPeriodic("a", 12);	//K > ����� ������
    //isPeriodic("", 1);	//������ ������
}