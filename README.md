# Что такое c-my-db?

Это маленькая база данных на 1 таблицу с зашитой схемой в код (по ТЗ)

## Как оно работает

БД парсит данные из файла в формате `ini`

На протяжении работы программы данные таблицы не меняются

Запросы принимаются их `stdin` в формате `ini`. (1 секция - 1 именованный запрос)

Результат запросы выплёвывается в `stdout` в формате `ini`. (имя секции совпадает с именем запроса)