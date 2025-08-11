# TCP клиент и многопоточный сервер (C++)

Простой учебный пример по заданию «Сетевое программирование». Один проект, две цели: `tcp_server` и `tcp_client`.
Код максимально базовый, без дополнительных зависимостей. Сервер обрабатывает каждого клиента в отдельном потоке.

## Сборка

### Linux / macOS
```bash
mkdir build && cd build
cmake ..
cmake --build . -j
```

### Windows (Visual Studio / MSVC)
```bat
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Запуск

1. Сначала запустите сервер:
   - Linux/macOS: `./tcp_server`
   - Windows: `.\Release\tcp_server.exe` (или `.\Debug\tcp_server.exe`)

2. В другом терминале запустите клиент:
   - Linux/macOS: `./tcp_client`
   - Windows: `.\Release\tcp_client.exe` (или `.\Debug\tcp_client.exe`)

Клиент отправляет строку `Привет, сервер!` и получает ответ `Привет от сервера!`.

## Настройка

- Порт по умолчанию: `8080` (измените в исходниках при необходимости).
- Адрес сервера для клиента: `127.0.0.1` (локально). Чтобы подключаться к удалённому серверу, замените IP в `client.cpp`.