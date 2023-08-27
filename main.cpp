#include <concepts>
#include <string>
#include <iostream>

template <typename T> concept ComplexConcept = requires(const T v)
{
  {v.hash()}->std::convertible_to<long>;
  {v.toString()}->std::same_as<std::string>;
}
&& !std::has_virtual_destructor<T>::value;

template <ComplexConcept T> void VerificationConcept(T val) 
{
  std::cout << "Метод hash(): " << val.hash() << std::endl;
  std::cout << "Метод toString(): " << val.toString() << std::endl;
  std::cout << "Наличие виртуального деструктора: " 
    << std::has_virtual_destructor<T>::value << std::endl;
}

//Тип соответствующий концепту.
struct Type1 
{
  double val_ = 0;
  Type1(auto const val):val_(val) {};
  ~Type1() = default;
  auto hash() const
  {
    return static_cast<long>(val_);
  }
  auto toString() const 
  {
    return std::to_string(val_);
  }
};

//Тип не соответствующий концепту. Отсутствует метод hash().
struct Type2
{
  double val_ = 0;
  Type2(auto const val):val_(val) {};
  ~Type2() = default;
  auto toString() const
  {
    return std::to_string(val_);
  }
};

//Тип соответствующий концепту. С преобразованием строки в double.
struct Type3
{
  double val_ = 0;
  Type3(auto const val):val_(std::stod(val)) {};
  ~Type3() = default;
  auto hash() const
  {
    return static_cast<long>(val_);
  }
  auto toString() const 
  {
    return std::to_string(val_);
  }
};

//Тип не соответствующий концепту. Метод toString() возвращает double.
struct Type4
{
  double val_ = 0;
  Type4(auto const val):val_(val) {};
  ~Type4() = default;
  auto hash() const
  {
    return static_cast<long>(val_);
  }
  auto toString() const
  {
    return val_;
  }
};

//Тип не соответствующий концепту. Наличие виртуального деструктора.
struct Type5
{
  double val_ = 0;
  Type5(auto const val):val_(val) {};
  virtual ~Type5() = default;
  auto hash() const
  {
    return static_cast<long>(val_);
  }
  auto toString() const
  {
    return std::to_string(val_);
  }
};


auto main() -> int
{
  system("chcp 1251");
  system("cls");
  Type1 t1(666.13);
  VerificationConcept<Type1>(t1);
  Type2 t2(15.156);
  /*Ошибка компиляции. Отсутствует метод hash().
  VerificationConcept<Type2>(t2);*/
  Type3 t3("568.65"); 
  VerificationConcept<Type3>(t3);
  Type4 t4(-18);
  /*Ошибка компиляции. Метод toString() возвращает double.
  VerificationConcept<Type4>(t4);*/
  Type5 t5(238.996);
  /*Ошибка компиляци. Наличие виртуального деструктора.
  VerificationConcept<Type5>(t5);*/
  return 0;
}