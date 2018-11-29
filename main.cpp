#include <iostream>
#include <list>
#include <vector>
#include "utils/utils.h"

using namespace std;

template <typename X>
class Base {
public:
    int basefield;
    typedef int T;
};
class D1 : public Base<Base<void> > {        //实际上不是模板
public:
    void f() { basefield =3; }
};
template<typename T2>
class D2 : public Base<double> {    //非依赖型基类
public:
    void f() { basefield =7; }    //正常访问继承成员
    T2 strange;                    //T是Base<double>::T，而不是模板参数
};

void g (D2<float*>& d2, float* p)
{
    d2.strange = p;        //错误，类型不匹配
}

template <typename T>
class MyType{
public:
    typedef T Type;
};

class X {
public:
    operator int() { return 123; }
    operator float() { return 1.23; }
};
//X x;
//cout << int(x) << ", " << float(x) << endl;

template <typename T>
typename T::ElemntType at(T const & a, int i){
    return a[i];
}

//class A{
//public:
//    typedef int ElemntType;
//};
//void f(A p){
//    at<A>(p, 1);
//}

template<typename T> class List;
class Marker;
List< ::Marker>* markers;


//=================================
template <typename T>
class Array {
public:
    typedef T ElementT;
//    …
};
template <typename T>
void clear (typename Array<T>::ElementT& p){

};      // 正确

//template<>
//void clear(typename Array<int>::ElementT& p){
//
//};    // 错误

//======================================================================================================================
//a typedef cannot be a template
//template <typename T>
//typedef vector<list<T> > Table;

template <typename T>
class Table {
public:
    typedef vector<list<T> > Type;
};
  // t的类型为vector<list<int> >


//======================================================================================================================
template<typename>
class AccumulationTraits;
template<>
class AccumulationTraits<float> {
public:
    typedef double AccT;
    static AccT const zero ;  // 错误：并不是一个整型变量
};
double const AccumulationTraits<float>::zero = 1.0;


//======================================================================================================================
template <bool, typename ,typename >
class IfThenElse;

template <typename Ta, typename Tb>
class IfThenElse<true, Ta, Tb>{
public:
    typedef Ta Type;
};

template <typename Ta, typename Tb>
class IfThenElse<false, Ta, Tb>{
public:
    typedef Tb Type;
};

template <typename Ta, typename Tb>
class ParamType{
public:
    typedef typename IfThenElse<
            (sizeof(Ta)>sizeof(Tb)),
            Ta,
            typename IfThenElse<sizeof(Ta)<sizeof(Tb), Tb, void>::Type
    >::Type Type;
};

template <typename T1, typename T2>
typename ParamType<T1, T2>::Type max(T1 a, T2 b){
    return a > b ? a : b;
}

#define A ClassA
class A{
public:
    void print(){
        cout<< __FUNCTION__ << endl;
    }
};

#undef A

//======================================================================================================================
template <int N, int I=0>
class Sqrt {
public:
    enum { result = (I*I<N) ? Sqrt<N,I+1>::result
                            : I };
};

template <int N>
class Sqrt<N, N>{
public:
    enum {result = N};
};

//======================================================================================================================
int main() {

//    std::cout << AccumulationTraits<float>::zero << endl;
//    Table<float>::Type t2;
//    typename ParamType<int, char>::Type a = 500;
//    cout << a << endl;

//    ClassA a;
//    a.print();

//    int a = 20;
//    char b = 100;
//    cout << ::max(a, b) << endl;
    cout << Sqrt<200>::result << endl;
    return 0;
}