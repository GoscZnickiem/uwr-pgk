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
 - `W/S/A/D/SPACE/SHIFT` - ruch do przodu/tyłu/lewo/prawo/góra/dół
 - `TAB` - przełączanie między kamerą zewnętrzną i wewnętrzną
 - `+/-` - zmiana FOV
 - `ENTER` - tymczasowe wyłączenie sortowania przezroczystych rzeczy
 - `ESCAPE` - przejście do następnego poziomu (może się przydać żeby zobaczyć jak program radzi sobie przy większej liczbie bombelków)
 - mysz - obrót kamery i zmiana FOV (kółko myszy)

Celem gry jest przejście do przeciwległej ścianki akwarium, unikając bombleków i zbierając specjalne świecące bombelki.
