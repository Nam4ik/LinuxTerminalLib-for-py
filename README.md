


# Документация для модуля `terminallib`

Модуль `linuxterminallibRM` предоставляет функции для взаимодействия с терминалом, включая запуск команд, установку режимов терминала, чтение и запись данных.

## Функции

### `run_command(command: str) -> str`

Запускает команду в терминале и возвращает её вывод.

- **Параметры:**
  - `command` (str): Команда для выполнения в терминале.
- **Возвращает:**
  - `str`: Вывод команды.

### `set_terminal_mode(fd: int, mode: int) -> None`

Устанавливает режим терминала.

- **Параметры:**
  - `fd` (int): Файловый дескриптор терминала.
  - `mode` (int): Режим терминала. `1` для raw mode, `0` для cooked mode.
- **Возвращает:**
  - `None`

### `read_from_terminal(fd: int) -> str`

Читает данные из терминала.

- **Параметры:**
  - `fd` (int): Файловый дескриптор терминала.
- **Возвращает:**
  - `str`: Прочитанные данные.

### `write_to_terminal(fd: int, data: str) -> None`

Записывает данные в терминал.

- **Параметры:**
  - `fd` (int): Файловый дескриптор терминала.
  - `data` (str): Данные для записи.
- **Возвращает:**
  - `None`

## Пример использования

```python
import linuxterminallibRM as terminal

# Запуск команды в терминале
output = terminal.run_command("ls -l")
print(output)

# Установка режима терминала
terminal.set_terminal_mode(0, 1)  # Устанавливаем raw mode

# Чтение данных из терминала
input_data = terminal.read_from_terminal(0)
print(input_data)

# Запись данных в терминал
terminal.write_to_terminal(0, "Hello, terminal!")

## Установка

Для использования модуля `terminal`, скомпилируйте его с помощью `setup.py` или другого инструмента сборки Python C-расширений.

bash

Copy code

python setup.py build
python setup.py install

## Зависимости

- Python 3.x
    
- Стандартная библиотека Python
    
- Библиотеки POSIX (unistd.h, fcntl.h, termios.h)
    

## Автор

[Nam4ik]

