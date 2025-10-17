# Собственный контейнер матрицы

## Способы

- Алгоритм Барейса
- Алгоритм Гаусса

## Запуск генератора тестов

Собираем:

```bash
cmake -S . -B build && cmake --build build
```

Даем доступ запуска `genTest.sh`

```bash
chmod +x genTest.sh
```

Запускаем:

```bash
./genTest.sh
```