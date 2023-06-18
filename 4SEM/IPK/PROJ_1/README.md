# IPK 2021


## Ciel Projektu
Cielom je vytvorenie serveru komunikujuceho pomocou protokolu HTTP, ktory bude poskytovať informacie ako meno hostu, záťaž CPU a nazov CPU . Server bude pripojeny na zadaný portv argumente a bude vraciať požadované informace na url. 


## 
Využivany je socket programing
```c
//  creating socket
   (server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
// specify
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(argument);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind
    rc = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address))

    // listen
    rc = listen(server_socket, 1))
```
## Spustenie
Program je spustitelný cez prikazový riadok s argumentom označujucim port.

spustenie serveru

./hinfosvc 12345 

pripojenie na server

curl http://localhost:12345/hostname

curl http://localhost:12345/cpu-name

curl http://localhost:12345/load


## Autor
Lukasz Pycz