# Лабораторная работа 2

**Название:** "Разработка драйверов блочных устройств"

**Цель работы:** Получить знания и навыки разработки драйверов блочных устройств для операционной системы Linux.

## Описание функциональности драйвера
### Вариант - 2
Драйвер создаёт виртуальный жесткий диск в оперативной  памяти с размером 50 Мбайт.
Созданный диск должен быть разбит на два первичных и один расширенный разделы с размерами
10Мбайт, 20Мбайт и 20Мбайт соответственно. Расширенный
раздел должен быть разделен на два логических с размерами
по 10Мбайт каждый.

## Инструкция по сборке

### Сборка
```
make
```
### Установка модуля
```
make start
```

### Удаление модуля
```
make exit
```

### Тестовые примеры для модуля
```
make test
```

### Создание файлов для тестов времени копирования
```
make create
```

### Очистка директории
```
make clean
```
## Инструкция пользователя

После загрузки модуля появится новые файлы /dev/vramdisk и /dev/vramdisk1, /dev/vramdisk2, /dev/vramdisk5, /dev/vramdisk6
В списке блочных устройств появятся 5 новых записей
Выполнить команду `lsblk`
```
mkfs.vfat /dev/vramdisk1 - создать файловую систему vfat на разделе диска
mkdir /mnt/vramdisk1 – создать директорию для монтирования диска
mount -t vfat /dev/vramdisk1 /mnt/vramdisk1 - смонтировать файловую систему в директорию
```
## Примеры использования
```
make 
make start
make test
make create

time sudo cp /mnt/vramdisk1/file /mnt/vramdisk2
time sudo cp /mnt/vramdisk2/file /mnt/vramdisk5
time sudo cp /mnt/vramdisk5/file /mnt/vramdisk6
time sudo cp /mnt/vramdisk6/file /mnt/vramdisk1

time sudo cp /mnt/vramdisk1/file ~
time sudo cp /mnt/vramdisk2/file ~
time sudo cp /mnt/vramdisk5/file ~
time sudo cp /mnt/vramdisk6/file ~

make exit
```
