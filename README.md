# MitracheAndreea_Practica2024
Client HTTP


# 17.06.2024

Am modificat README

# 18.06.2024

Am adaugat clientHTTP

# 19.06.2024

Am structurat codul si am adaugat manipularea cookie-urilor

Plan implementare Client HTTP

Functionalitati principale:

    Cereri HTTP:
        GET: Permite obtinerea de date de la o adresa URL specificata.
        POST: Trimite date catre o adresa URL data si primeste raspunsul de la server.
        Descarcare de fisiere: Permite descarcarea fisierelor de la adrese URL si salvarea lor local.

    Autorizare:
        Suport pentru autentificarea JWT (JSON Web Token) pentru adaugarea de securitate si autorizare la cererile HTTP.

# 20.06.2024

Am folosit biblioteca "libcurl". Trimitem o cerere GET catre URL-ul specificat.
In procesul de configurare a cererii, se seteaza header-ele HTTP necesare, inclusiv
un header de autorizare Bearer token pentru autentificare. Cookie-urile
sunt gestionate prin intermediul unei structuri dedicate, iar raspunsul
HTTP este primit si procesat intr-o zona de memorie, unde este afisat
corpul raspunsului si header-ele corespunzatoare. Aplicatia gestioneaza
sectiuni fictive si ofera suport pentru o functionalitate teoretica de cache.

# 21.06.2024

Am implementat suport pentru debug în codul meu.

# 25.06.2024

Am adaugat o functie pentru descarcarea de fisiere.

# 26.06.2024

Am adaugat pentru autentificarea JWT (JSON Web Token) pentru adaugarea de securitate si autorizare la cererile HTTP.


