# cpp_templates

Учебные примеры возможностей C++20: шаблоны функций и классов, специализация шаблонов, политики (policy-based design).

## Сборка

```
make
./build/demo
```

## Содержание demo.cpp

- `max` — шаблон функции с проверкой через `static_assert` на этапе компиляции.
- `to_string` — специализация шаблона функции для `enum class Status`.
- `Client<Policy>` — policy-based класс, выбирающий таймаут по типу политики.
