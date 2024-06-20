# MitracheAndreea_Practica2024
Client HTTP


# 17.06.2024

Am modificat README

# 18.06.2024

Am adaugat clientHTTP

# 19.06.2024

Am structurat codul si am adaugat manipularea cookie-urilor

Plan implementare Client HTTP

Functionatitati:

1.    Trimiterea de cereri HTTP
2.    Primirea de răspunsuri HTTP
3.    Manipularea cookie-urilor
4.    Setarea headerelor HTTP
5.    Autentificarea
6.    Gestionarea sesiunilor
7.    Solicitarea de resurse externe
8.    Gestionarea redirectionărilor
9.    Cache management
10.    Tratarea erorilor și a condițiilor de eroare

# 20.06.2024

Am folosit biblioteca "libcurl". Trimitem o cerere GET catre URL-ul specificat.
In procesul de configurare a cererii, se seteaza header-ele HTTP necesare, inclusiv
un header de autorizare Bearer token pentru autentificare. Cookie-urile
sunt gestionate prin intermediul unei structuri dedicate, iar raspunsul
HTTP este primit si procesat intr-o zona de memorie, unde este afisat
corpul raspunsului si header-ele corespunzatoare. Aplicatia gestioneaza
sectiuni fictive si ofera suport pentru o functionalitate teoretica de cache.