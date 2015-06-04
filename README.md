# KLLibs
Lekki zestaw bibliotek napisany w C++ przeznaczony do współpracy z kontrolerem AVR lub innym dowolnym procesorem.

Biblioteki używają minimalnej ilości wbudowanych funkcji C/C++ i są zaimplementowane z użyciem mechanizmów dostarczanych przez C++11.

# Składniki

## Kontenery

### KLList
Kontener reprezentujący liste obiektów.

- Indeksy tworzone są automatycznie. Indeks to numer elementu licząc od zera od początku listy.
- W przypadku wyboru nieistniejącego obiektu zostanie zwrócona referencja na pusty wskaźnik.

Możliwości:
- [X] Dodawanie obiektów.
- [X] Usuwanie obiektów.
- [X] Iteracja po zakresie.
- [X] Sprawdzenie ilości obiektów.

### KLMap
Kontener reprezentujący mape

- Szablon umożliwiający użycie dowolnych typów.
- W przypadku wyboru nieistniejącego obiektu zostanie zwrócona referencja na pusty wskaźnik.

Możliwości:
- [X] Dodawanie obiektów.
- [X] Usuwanie obiektów.
- [X] Iteracja po zakresie.
- [X] Sprawdzenie ilości obiektów.
- [X] Sprawdzanie dostępności lub użycia klucza.
- [X] Zmiana klucza (`KLMap::Update`).

### KLString
Kontener reprezentujący łańcuch znaków.

- Przechowywanie łańcuchów znaków i operacje na nich.

Możliwości:
- [X] Dodawanie i usuwanie fraz.
- [X] Wyszukiwanie znaków i fraz.
- [X] Zliczanie fraz i znaków.
- [X] Zamiana fraz.
- [X] Porównywanie łańcuchów.
- [X] Jawne konstruktory konwertujące z typów liczbowych (`double`, `int` i `const void*`).
- [ ] Iteracja po zakresie (nieplanowane).
- [ ] Operatory konwersji na typy liczbowe (nieplanowane).

### KLTree
Kontener reprezentujący drzewo obiektów.

- Indeksy tworzone są automatycznie. Indeks to numer elementu licząc od zera od początku drzewa.
- W przypadku wyboru nieistniejącego obiektu zostanie zwrócona referencja na pusty wskaźnik.

Możliwości:
- [X] Dodawanie obiektów.
- [X] Usuwanie obiektów.
- [X] Iteracja po zakresie.
- [X] Sprawdzenie ilości obiektów.
- [X] Przełączanie się pomiędzy gałęziami.

## Interpreter skryptów

### KLParser
Interpreter wyrażeń matematycznych i logicznych.

- Interpretacja wyrażeń matematycznych.
- Interpretacja wyrażeń logicznych.
- Wbudowane funkcje trygonometryczne i matematyczne.

Możliwości:
- [X] Obsługa priorytetu operatorów.
- [X] Obsługa wyrażeń w nawiasach.
- [X] Operacje na liczbach zmiennoprzecinkowych i logicznych.
- [X] Obsługa błędów.
- [ ] Rozszerzalna lista funkcji.
- [ ] Automatyczna obsługa zmiennych.
- [ ] Instrukcje przypisania.

### KLScript
Prosty interpreter skryptów.

- Zarządzanie zmiennymi.
- Bindowanie zmiennych.
- Bindowanie i wywoływanie funkcji.

Możliwości:
- [X] Obsługa zmiennych.
- [X] Wywoływanie funkcji.
- [X] Konstrukcja `if () ... else ...`.
- [X] Konstrukcja `while () ...`.

Przykład:

``` bash

     # utworzenie zmiennej
     var Zmienna;

     # przypisanie wartości
     set Zmienna sin( (2+2)*3 + 10 );

     # instrukcja if
     if Zmienna 100 || Zmienna < 50;

          # wywołanie funkcji
          call Funkcja sin(Zmienna), 30, Zmienna*2;

          # przypisanie wyniku funkcji do zmiennej
          set Zmienna return;

     # instrukcja else
     else;

          var i;

          # instrukcja while
          while i < 10;

               call Procedura Zmienna % i, Zmienna;
               set i i+1;

               if not return;

                    # przerwanie skryptu
                    exit;

               fi;

          # zakończenie while
          done;

     # zakończenie if
     fi;

     # zapisanie zwróconej wartości
     return 0;

```

### KLBindings
Bindy funkcji dostępne z poziomu skryptu.

- Wywoływanie funkcji o stałym prototypie ze skryptu.
- Przekazywanie parametrów i zwracanie wyników za pośrednictwem systemu `KLVariables`.

Moźliwości:
- [X] Bindowanie funkcji.
- [X] Edycja i zarządzanie zmiennymi w funkcji.
- [X] Edycja (usuwanie i aktualizacja) bindów.
- [X] Zwracanie wartości do skryptu (za pomocą listy zmiennych).

### KLVariables
System zarządzania zmiennymi w skrypcie.

- Przechowywanie łańcuchów znaków, typów całkowitych i zmiennoprzecinkowych.
- Silne typowanie (typowanie dynamiczne wykonywane ręcznie).
- Zarządzanie pamięcią.

Możliwości:
- [X] Bindowanie lokalnych obiektów.
- [X] Kopiowanie obiektów i zakresów.
- [X] Organizacja wielopoziomowa.
- [X] Rekursywne przeszukiwanie poziomów.
- [X] Informacja o typie.
- [X] Informacja o bindzie.
- [X] Iteracja po zakresie (zgodnie z `KLMap`).
- [ ] Iteracja po zakresie lub wybór zmiennej bez znajomości jej nazwy (TODO).
- [ ] Automatyczna kontrola typu przy poeracjach wyłuskania.
- [ ] Słabe typowanie.
- [ ] Dynamiczna zmiana typu.

# Licencja

KLLibs - Zbiór lekkich bibliotek. Copyright (C) 2015 Łukasz "Kuszki" Dróżdż.

Niniejszy program jest wolnym oprogramowaniem; możesz go rozprowadzać dalej i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU, wydanej przez Fundację Wolnego Oprogramowania - według wersji 2-giej tej Licencji lub którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH ZASTOSOWAŃ. W celu uzyskania bliższych informacji - Powszechna Licencja Publiczna GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz Powszechnej Licencji Publicznej GNU (GNU General Public License); jeśli nie - napisz do Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
