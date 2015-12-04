# KLLibs
Lekki zestaw bibliotek napisany w C++ przeznaczony do współpracy z kontrolerem AVR lub innym dowolnym procesorem.

Biblioteki używają minimalnej ilości wbudowanych funkcji C/C++ i są zaimplementowane z użyciem mechanizmów dostarczanych przez C++11.

# Składniki
KLLibs zawiera kilka głównych modułów takich jak kontenery, czy mechanizmy języka skryptowego. Niektóre komponenty wymagają dołączenia do projektu innych modułów KLLibs.

## Kontenery
KLLibs posiada zestaw wbudowanych kontenerów używanych w innych modułach projektu. Kontenery zaprojektowano pod kątem oszczędności pamięci i wydajności obliczeń tak, aby mogły one być wykorzystane nawet na słabych platformach sprzętowych.

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
Interpreter skryptów zawiera parser matematyczny i system bindowania zmiennych i funkcji. Cały mechanizm da się uruchomić na platformie 8-bitowej z minimum 18 kB pamięci programu i około 1 kB pamięci RAM (ilość pamięci zależy od przeprowadzanych operacji). Przy wykonywaniu skryptu interpreter nie potrzebuje alokować dużych obszarów pamięci więć zwykle jeśli skrypt zdoła zostać umieszczony w pamięci oraz zostanie zainicjowany interpreter, to skrypt ten zostanie poprawnie wykonany.

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
- [X] Dynamiczne definiowanie funkcji `define ... end`.

Przykład:

``` bash

	# definicja własnej funkcji
	define funkcja;
		pop a;	# pobranie pierwszej wartości ze ztosu parametrów
		pop b;	# pobranie drugiej wartości ze ztosu parametrów

		# obliczenie wyniku funkcji
		if (a > b); return a;
		else; return b;
	end;

     # utworzenie zmiennej
     var Zmienna1, Zmienna2;

     # przypisanie wartości
     set Zmienna sin( (2+2)*3 + 10 );

     # wywołanie własnej funkcji
     goto funkcja Zmienna1, Zmienna2;

     # pobranie wyniku wywołanej funkcji do zmiennej
     var wynik; set wynik return;

     # instrukcja if
     if Zmienna != 100 || Zmienna < 50;

          # wywołanie funkcji (kolejne parametry po przecinku)
          call Funkcja sin(Zmienna), 30, Zmienna*2;

          # przypisanie wyniku funkcji do zmiennej
          set Zmienna return;

          # eksport zmiennej do zmiennych globalnych
          export zmienna;

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
- [X] Iteracja po zakresie lub wybór zmiennej bez znajomości jej nazwy.
- [ ] Automatyczna kontrola typu przy operacjach wyłuskania.
- [ ] Słabe typowanie.
- [ ] Dynamiczna zmiana typu.

## Dodatki do Qt
KLLibs zawiera kilka dodatków do biblioteki Qt ułatwiających edycje kodu skryptu w GUI. Aby zbudować biblioteke z tymi dodatkami należy w argumentach qmake dodać następujący wpis: `CONFIG+=addons`. Domyślnie biblioteka budowana jest bez tych dodatków.

### KLHighlighter
Mechanizm podświetlania składni języka `KLScript`

- do użycia wraz z obiektem `QTextDocument`
- podświetlanie składni i elementów języka

Możliwości:
- [X] Edycja styli podświetlania.

### KLScripteditor
Rozszerzenie `QPlainTextEdit` umożliwiające podświetlanie składni i numerowanie linii przy edycji kodu `KLScript`.

Możliwości:
- [X] Numeracja wierszy.
- [X] Podświetlanie aktywnej linii.
- [X] Automatyczne wyróżnianie składni (`KLHighlighter`).
- [ ] Zmiana kolorów w widgecie.

### KLScriptbinding
Bazujący na `QObject` i `KLScript` bind ułatwiający współpracę z `KLScript` w środowisku QT.

### KLParserbinding
Bazujący na `QObject` i `KLParser` bind ułatwiający współpracę z `KLParser` w środowisku QT.

## Użycie `boost::function` i `boost::bind`
Aby zamiast prostego bindowania funkcji w stylu `C` używać biblioteki `boost` należy skompilować bibliotekę z użyciem `CONFIG+=boost`, a przy linkowaniu biblioteki uaktywnić globalne makro `USING_BOOST`.

# Licencja
KLLibs - Zbiór lekkich bibliotek. Copyright (C) 2015 Łukasz "Kuszki" Dróżdż.

Niniejszy program jest wolnym oprogramowaniem – możesz go rozpowszechniać dalej i/lub modyfikować na warunkach Powszechnej Licencji Publicznej GNU wydanej przez Fundację Wolnego Oprogramowania, według wersji 3 tej Licencji lub dowolnej z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on użyteczny – jednak BEZ ŻADNEJ GWARANCJI, nawet domyślnej gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH ZASTOSOWAŃ. Bliższe informacje na ten temat można uzyskać z Powszechnej Licencji Publicznej GNU.

Kopia Powszechnej Licencji Publicznej GNU powinna zostać ci dostarczona razem z tym programem. Jeżeli nie została dostarczona, odwiedź http://www.gnu.org/licenses/.
