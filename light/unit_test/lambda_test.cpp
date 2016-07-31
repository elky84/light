#include "stdafx.h"
#include <algorithm>

#include "boost/function.hpp"

/*
[]      // lambda capture 영역
()      // parameter list 영역
{}      // body 영역
()      // 이는 위 lambda 함수를 명시적으로 호출할 때
*/

template< typename T >
void fill(std::vector<int> & v, T done)
{
    int i = 0;
    while (!done())
    {
        v.push_back(i++);
    }
}

// http://devsw.tistory.com/197
// http://itguru.tistory.com/196
// http://egloos.zum.com/sweeper/v/3015035

TEST(lambda, capture_test)
{
    std::vector<int> stuff;
    fill(stuff, [&]()->bool { return stuff.size() >= 8; });

    std::array<int, 8> stuff_array;
    for (int n = 0; n < 8; ++n)
    {
        stuff_array[n] = n;
    }

    for_each(stuff.begin(), stuff.end(), [&](int i) {  ASSERT_EQ(stuff_array[i], i); });
}

template<typename TFunc>
void TemplateFunc(TFunc func)
{
    func();
};

TEST(lambda, parameter)
{
    auto func = [] {  };
    TemplateFunc(func);
}

boost::function<void()> FunctionReturn()
{
    return [] { std::cout << "대한민국~~~짝짝짝 짝짝~~~" << std::endl; };
}

TEST(lambda, return)
{
    auto func = FunctionReturn();
    func();
}

TEST(lambda, vector)
{
    std::vector<boost::function<const char* ()> > v;

    v.push_back([] { return "대한민국 "; });
    v.push_back([] { return "짝짝짝~~~짝짝~~~"; });
}

TEST(lambda, explicit_return)
{
    // 암시적 타입 반환
    auto func1 = [] { return 3.14; };

    // 후행 반환 형식을 이용한 명시적 반환값 타입 지정 ( [ -> (return type) ] )
    // 파라미터가 없더라도 반드시 파라미터 블록을 지정해 주어야 한다
    auto func2 = []() -> float { return 3.14f; };

    // func2의 반환값은 double 타입으로 추론되었다. 아래 경고 발생
    // warning C4244: '초기화 중' :
    //                  'double'에서 'float'(으)로 변환하면서 데이터가 손실될 수 있습니다.
    //float f1 = func1();
    //ASSERT_EQ(3.14, f1);

    // 명시적으로 float 반환했기에 OK
    float f2 = func2();
    ASSERT_EQ(3.14f, f2);
}


TEST(lambda, capture_reference)
{
    std::vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i);
    }

    int evenCount = 0;

    // lambda 함수
    for_each(v.begin(), v.end(), [&evenCount](int n)
    {
        if (n % 2 == 0) { evenCount++; }
    });
}

TEST(lambda, copy_reference)
{
    std::vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i);
    }

    int evenCount = 0;

    // lambda 함수
    for_each(v.begin(), v.end(), [evenCount](int n) mutable
    {
        if (n % 2 == 0) { evenCount++; }
    });
}

TEST(lambda, capture_multi)
{
    int a;
    short b;
    float c;
    char d;

    // a, d 참조로... b, c 복사로...
    auto func = [&a, b, c, &d] { std::cout << a << b << c << d; };
}

int g_evenCount = 0;

TEST(lambda, capture_global)
{
    std::vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i);
    }

    // lambda 함수
    for_each(v.begin(), v.end(), [&](int n)
    {
        if (n % 2 == 0) { g_evenCount++; }
    });
}

class PrimeNumber
{
public:
    PrimeNumber()
    {
        m_primeList.push_back(1);
        m_primeList.push_back(2);
        m_primeList.push_back(5);
        m_primeList.push_back(7);

        // ...
    }

    void PrintPrimeNumbers() const
    {
        // this를 캡쳐한 것에 주목!!!
        for_each(m_primeList.begin(), m_primeList.end(), [this](int primeNumber)
        {
            _Print(primeNumber);
        });
    }

private:
    typedef std::vector<int> PrimeNumberList;
    PrimeNumberList m_primeList;

    void _Print(int primeNumber) const
    {
        std::cout << "The prime number : " << primeNumber << std::endl;
    }
};

TEST(lambda, capture_class)
{
    PrimeNumber pn;
    pn.PrintPrimeNumbers();
}