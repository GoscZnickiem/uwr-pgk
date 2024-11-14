# Podstawy Grafiki Komputerowej - Pracownia
### Autor: Maciej Zgierski

Każda pracownia ma swoją osobną gałąź. (informacja na githuba)

## Wymagane paczki
 - gcc
 - make
 - GLFW
 - GLEW

## Budowanie
Projekt zawiera parę podstawowych poleceń make'a:

    make

lub

    make release

zbuduje projekt do pliku wykonywalnego "app".

    make clean

wyczyści repozytorium po kompilacji

    make clear

wyczyści całe repozytorium (łącznie ze zbudowanym plikiem wykonywalnym).

    make debug

zbuduje projekt w konfiguracji do debugowania w katalogu "debug/".

## Funkcjonalność

Sterowanie: 
 - `W/S/A/D/R/F` - ruch do przodu/tyłu/lewo/prawo/góra/dół
 - `C` - przełączanie między kamerą zewnętrzną i główną
 - `spacja` - przełączanie trybu widoku "minimapy"
 - `escape` - pauza. Wciśnięcie `R` w trakcie pauzy powoduje reset
 - mysz - obrót kamery i przybliżanie (kółko myszy)

Celem gry jest przejście do niebieskiej kuli (mety) po przeciwnej stronie labiryntu 3D.

Na planszy znajdują się (oprócz statycznych przeszkód): 
 - poruszający się wrogowie (czerwone kulki) resetujące progress gracza,
 - wzmocnienia (niebiesko-zielone kulki), które po zebraniu na krótki czas ułatwiają przedostawanie się przez labirynt.
