# Aplikacja Czatu  

## Opis problemu  
Aplikacja czatu to program umożliwiający użytkownikom komunikację w czasie rzeczywistym za pomocą wiadomości tekstowych. Głównym wyzwaniem w takiej aplikacji jest zapewnienie poprawnej synchronizacji wątków oraz efektywnej obsługi wielu połączeń klientów jednocześnie. Typowe problemy, które mogą wystąpić, to:  

- **Współbieżność dostępu do zasobów** – wiele klientów może próbować jednocześnie wysyłać wiadomości, co wymaga odpowiedniej synchronizacji, aby uniknąć zjawiska wyścigu.  
- **Blokowanie wątków** – serwer musi efektywnie zarządzać połączeniami, aby uniknąć sytuacji, w której jeden klient blokuje innych.  
- **Skalowalność** – aplikacja powinna działać płynnie nawet przy dużej liczbie użytkowników.
  
## Kluczowe funkcjonalności
1. **Komunikacja grupowa** - rozsyłanie wiadomości do wszystkich podłączonych klientów
2. **Wiadomości prywatne** - możliwość wysyłania wiadomości do wybranego użytkownika
3. **Dynamiczne zarządzanie użytkownikami** - automatyczne powiadomienia o dołączaniu i opuszczaniu czatu
4. **System nazw użytkowników** - walidacja unikalności nazw użytkowników
   
## Wątki i ich reprezentacja  
Każdy klient połączony z serwerem działa w osobnym wątku, który obsługuje wysyłanie i odbieranie wiadomości. Serwer zarządza wszystkimi połączeniami, przekazując wiadomości między użytkownikami.  

### Główne wyzwania
- **Współbieżny dostęp do listy klientów** - wiele wątków może jednocześnie modyfikować listę aktywnych połączeń
- **Race conditions** - przy przydzielaniu nazw użytkowników
- **Deadlocki** - potencjalne blokady przy zarządzaniu zasobami

## Sekcje krytyczne i ich rozwiązanie  
Sekcje krytyczne występują, gdy wiele wątków próbuje jednocześnie modyfikować współdzielone zasoby, takie jak lista aktywnych użytkowników. Aby uniknąć problemów, stosowane są mechanizmy synchronizacji:  

- **Mutexy** – zapewniają wyłączny dostęp do współdzielonych danych (listy klientów).  
- **Blokady (lock_guard)** – automatyzują zarządzanie blokadami, zmniejszając ryzyko deadlocków.  
- **Kolejki wiadomości** – wiadomości są buforowane i przetwarzane w sposób sekwencyjny, aby uniknąć wyścigów.  
- **Flagi atomowe** - bezpieczne zarządzanie stanem serwera bez ryzyka blokady / wyścigu.

## Instrukcja uruchamiania projektu  

### Wymagania  
- Kompilator C++ (np. g++, clang)  
- Standard C++ 20 lub nowszy  
- System operacyjny: Windows (WinSock 2.2)
- Biblioteka: WinSock2
- MinGW 14.2 lub nowszy.  

### Jak uruchomić:  
#### GUI
Należy przejść do folderu `cmake-build-release/Server` i uruchomić program `Server.exe`. Program nie potrzebuje żadnych dodatkowych argumentów. Następnie, należy cofnąć się do folderu wstecz i przejść do folderu `Client`. 
Z tego miejsca należy uruchomić aplikację klienta chatu (`Client.exe`). 
Konieczne jest upewnienie się, że aplikacja Server.exe została wcześniej uruchomiona!

#### CMD
Należy otworzyć okno konsoli / wiersza poleceń, a następnie przejść do lokalizacji w której znajduje się plik `Server.exe`. W ramach polecenia wywołującego należy podać nazwę pliku. Przykład wywołania poniżej:
```cmd
C:\{Path_to_file}> Server.exe 
```
Następnie należy cofnąć się do folderu nadrzędnego i przejść do folderu `Client`. Tam analogicznie wywołujemy program `Client.exe`.

W celu edycji / kompilacji programów sugerowanym sposobem jest użycie IDE mające zintegrowany kompilator C++, takich jak:
- Visual Studio
- CLion
- Code::Blocks
  
Jednakże, projekt ten można również ręcznie skompilować. Należy się jednak upewnić, że środowisko dla kompilatora jest poprawnie skonfigurowane.

#### Klonowanie repozytorium  
```bash
git clone https://github.com/Vilczyca/Multithred_Chat_Server.git  
cd Multithred_Chat_Server  
```  

#### Kompilacja projektu  
```cmd
g++ -std=c++20 -o server.exe server.cpp -lws2_32 
g++ -std=c++20 -o client client.cpp -lpthread  
```  

#### Uruchomienie programu z poziomu cmd  
1. **Serwer**:  
   ```cmd
   {Path_to_file}\Server.exe  
   ```  

2. **Klient**:  
   ```cmd
   {Path_to_file}\Client.exe  
   ```  

3. Parametry konfiguracyjne (modyfikacja w kodzie):
   ```cpp
   const int PORT = 5555;
   ```
## Informacje dotyczące działania i komunikacji w aplikacji 
### Protokół komunikacyjny
Serwer używa prostego protokołu tekstowego z prefixami:
- s - wiadomość ogólnoserwerowa
- n - powiadomienie o nowym użytkowniku
- x - powiadomienie o opuszczeniu czatu
- p - wiadomość prywatna

### Znane ograniczenia
- Brak szyfrowania komunikacji
- Brak zapisu historii czatu
- Brak uwierzytelniania użytkowników
