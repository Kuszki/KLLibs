# KLLibs {#mainpage}
Lekki zestaw bibliotek napisany w C++ przeznaczony do współpracy z kontrolerem AVR lub innym dowolnym procesorem.

Biblioteki używają minimalnej ilości wbudowanych funkcji C/C++ i są zaimplementowane z użyciem mechanizmów dostarczanych przez C++11.

# Składniki

## Kontenery

### KLList
Kontener reprezentujący liste obiektów.

- Indeksy tworzone są automatycznie. Indeks to numer elementu licząc od zera od paczątku listy.
- W przypadku wyboru nieistniejącego obiektu zostanie zwrócona referencja na pusty wskaźnik!

Możliwości:
- [x] Dodawanie obiektów.
- [x] Usuwanie obiektów.
- [x] Iteracja po zakresie.
- [x] Sprawdzenie ilości obiektów.

### KLMap
Kontener reprezentujący mape

- Szablon umożliwiający użycie dowolnych typów.
- W przypadku wyboru nieistniejącego obiektu zostanie zwrócona referencja na pusty wskaźnik.

- [x] Dodawanie obiektów.
- [x] Usuwanie obiektów.
- [x] Iteracja po zakresie.
- [x] Sprawdzenie ilości obiektów.
- [X] Sprawdzanie dostępności lub użycia klucza.
- [X] Zmiana klucza (KLMap::Update).

### KLString
Kontener reprezentujący łańcuch znaków.

- Przechowywanie łańcuchów znaków i operacje na nich.
- Uniwersalny konstruktor konwertujący z typów liczbowych.

- [x] Dodawanie i usuwanie fraz.
- [x] Wyszukiwanie znaków i fraz.
- [x] Zliczanie fraz i znaków.
- [x] Zamiana fraz.
- [x] Porównywanie łańcuchów.
- [ ] Iteracja po zakresie (nieplanowane).
- [X] Konstruktory konwertujące z typów liczbowych (uniwersalny konstruktor).
- [ ] Operatory konwersji na typy liczbowe (nieplanowane).

## Interpreter skryptów

### KLScript
Prosty interpreter skryptów.

- SID

### KLBindings
Bindy funkcji dostępne z poziomu skryptu.

- SID

### KLVariables
Zmienne dostępne w skrypcie i ich powiązania ze zmiennymi programu.

- SID

# Licencja

KLLibs - Zbiór lekkich bibliotek. Copyright (C) 2015 Łukasz "Kuszki" Dróżdż.

Niniejszy program jest wolnym oprogramowaniem; możesz go rozprowadzać dalej i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU, wydanej przez Fundację Wolnego Oprogramowania - według wersji 2-giej tej Licencji lub którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH ZASTOSOWAŃ. W celu uzyskania bliższych informacji - Powszechna Licencja Publiczna GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz Powszechnej Licencji Publicznej GNU (GNU General Public License); jeśli nie - napisz do Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
