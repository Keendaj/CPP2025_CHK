# Выполнил Шевцов Дмитрий Александрович 5030102/30202

# Wrapper + Engine

## Описание проекта

Поддержка:
- **Именованных аргументов** - вызов параметров в произвольном порядке, порядок задаётся при создании wrapper, имена аргументов могут отличаться от имён используемых функций во wrapper.
- **Дефолтных значений** - автоматическое использование значений по умолчанию
- **Разные типы данных** - функция может использовать различные типы данных, но при вызове функции через engine необходимо точное соответствие аргументов, например, если функция принимает std::string, то необходимо передать std::string{"Hello world!"}, а не "Hello world!", т.к. это const char*

## Архитектура

### Основные компоненты

- `wrapper.h` - заголовочный файл с классом wrapper
- `makeWrapper()` - функция для создания экземпляров wrapper (создано исключительно для красивого создания экземпляров без указания типа шаблона)
- `Subject1.h/cpp` - тестовый класс с примерами использования
- `engine.h` - заголовочный файл с классом engine
- `Test.h/cpp` - набор тестов для engine и wrapper

### Класс wrapper

```cpp
template<class Class, class Function>
class wrapper
```

**Шаблонные параметры:**
- `Class` - тип класса, метод которого обёртывается
- `Function` - указатель на метод класса

## Возможности

### 1. Именованные аргументы
```cpp
auto wrap = makeWrapper(subject, &Subject1::calculate, {{"x", 10}, {"y", 20}, {"z", 30}});
engine.registerCommand("1", wrap);

engine.execute("1", {});

engine.execute("1", {{"z", 100}});

engine.execute("1", {{"z", 77}, {"x", 11}, {"y", 22}});
```

### 2. Поддержка различных типов данных
- Любые типы с `std::any`

### 3. Захват возвращаемых значений
```cpp
auto result = engine.execute("1", {{"x", 5}, {"y", 15}, {"z", 25}});
Далее необходимо выполнить any_cast к желаемому типу. Получить возвращаемый тип можно:
engine.getReturnType("1");
```

### 4. Статическая проверка типов
- Проверка типов на этапе компиляции
- `static_assert` для валидации шаблонных параметров (Проверяется только передача класса и метода, но не принадлежность переданного метода к классу)

## Сборка и запуск

### Требования
- C++17 или выше
- Cmake
- Ninja (опционально, я проводил сборку с использованием Ninja)

### Сборка с CMake без Ninja

```bash
mkdir build
cd build
cmake ..
cmake --build .
./Wrapper
```

### Сборка с CMake с Ninja

```bash
mkdir build
cd build
cmake .. -G Ninja
ninja
./Wrapper
```

## Примеры использования

### Базовый пример

```cpp
#include "wrapper.h"
#include "TestSubjects/Subject1.h"

int main() {
    Engine engine;
    auto subject = std::make_shared<Subject1>();

    auto wrap = makeWrapper(subject, &Subject1::calculate,
                           {{"x", 10}, {"y", 20}, {"z", 30}});
    engine.registerCommand("1", wrap);

    engine.execute("1", {});
    engine.execute("1", {{"x", 5}, {"z", 100}});
    int result = std::any_cast<int>(engine.execute("1",{{"x", 1}, {"y", 2}, {"z", 3}}));

    return 0;
}
```
