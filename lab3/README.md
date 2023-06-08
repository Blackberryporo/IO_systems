# Лабораторная работа 3

**Название:** "Разработка драйверов сетевых устройств"

**Цель работы:** 
Получить знания и навыки разработки драйверов сетевых интерфейсов для операционной системы Linux.

## Описание функциональности драйвера

1. Драйвер должен создавать виртуальный сетевой интерфейс в ОС Linux.
2. Созданный сетевой интерфейс должен перехватывать пакеты родительского интерфейса (eth0 или другого).
3. Сетевой интерфейс должен перехватывать:
    Пакеты протокола IPv4, адресуемые конкретному IP. Вывести IP адреса отправителя и получателя.
    Состояние разбора пакетов необходимо выводить в файл в директории /proc
4. Должна иметься возможность просмотра статистики работы созданного интерфейса.

## Инструкция по сборке

Сборка

`make`

Загрузка модуля в ядро

`make start`

Выгрузка модуля из ядра

`make exit`

Очистка

`make clean`

## Инструкция пользователя

Выполнить после загрузки модуля команду `ifconfig` и проверить, что на устройстве появился новый интерфейс:
```
daria@daria-VirtualBox:~/Desktop/IO_systems/lab3$ ifconfig
enp0s3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.0.178  netmask 255.255.255.0  broadcast 192.168.0.255
        inet6 fe80::dbe8:97ff:4d7:fe85  prefixlen 64  scopeid 0x20<link>
        ether 08:00:27:ee:89:1e  txqueuelen 1000  (Ethernet)
        RX packets 573751  bytes 739411880 (739.4 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 133660  bytes 18579172 (18.5 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lab3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.0.178  netmask 255.255.255.0  broadcast 192.168.0.255
        inet6 fe80::c879:7a1d:7e7d:35ad  prefixlen 64  scopeid 0x20<link>
        ether 08:00:27:ee:89:1e  txqueuelen 1000  (Ethernet)
        RX packets 207  bytes 23028 (23.0 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 198  bytes 24519 (24.5 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

Запомнить указанный в интерфейсе IP адрес и "пингануть" его с другой машины командой `ping 192.168.0.178` (для IP адресса из примера)

Так как пингуется этот IP адрес UDP дейтаграммами, которые посылаются на сетевом уровне как IP пакеты. Наш драйвер должен будет их отфильтровать. 
И их будет видно в файле `/proc/lab3`. Его содержимое можно увидеть командой `cat /proc/lab3`:

Пример вывода:
```
daria@daria-VirtualBox:~/Desktop/IO_systems/lab3$ cat /proc/lab3
Captured IP packet, saddr: 91.105.192.100
daddr: 192.168.0.178
Captured IP packet, saddr: 91.105.192.100
daddr: 192.168.0.178
Captured IP packet, saddr: 64.233.164.194
daddr: 192.168.0.178
Captured IP packet, saddr: 192.168.0.178
daddr: 64.233.164.194
Captured IP packet, saddr: 192.168.0.171
daddr: 255.255.255.255
Captured IP packet, saddr: 192.168.0.171
daddr: 255.255.255.255
Captured IP packet, saddr: 192.168.0.171
daddr: 255.255.255.255
Captured IP packet, saddr: 192.168.0.171
daddr: 192.168.0.255
Captured IP packet, saddr: 192.168.0.171
daddr: 255.255.255.255
```
