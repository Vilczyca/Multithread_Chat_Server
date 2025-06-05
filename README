# Aplikacja Czatu  

## Opis problemu  
Aplikacja czatu to program umożliwiający użytkownikom komunikację w czasie rzeczywistym za pomocą wiadomości tekstowych. Głównym wyzwaniem w takiej aplikacji jest zapewnienie poprawnej synchronizacji wątków oraz efektywnej obsługi wielu połączeń klientów jednocześnie. Typowe problemy, które mogą wystąpić, to:  

- **Współbieżność dostępu do zasobów** – wiele klientów może próbować jednocześnie wysyłać wiadomości, co wymaga odpowiedniej synchronizacji, aby uniknąć wyścigów (race conditions).  
- **Blokowanie wątków** – serwer musi efektywnie zarządzać połączeniami, aby uniknąć sytuacji, w której jeden wolny klient blokuje innych.  
- **Skalowalność** – aplikacja powinna działać płynnie nawet przy dużej liczbie użytkowników.  

## Wątki i ich reprezentacja  
Każdy klient połączony z serwerem działa w osobnym wątku, który obsługuje wysyłanie i odbieranie wiadomości. Serwer zarządza wszystkimi połączeniami, przekazując wiadomości między użytkownikami.  

## Sekcje krytyczne i ich rozwiązanie  
Sekcje krytyczne występują, gdy wiele wątków próbuje jednocześnie modyfikować współdzielone zasoby, takie jak lista aktywnych użytkowników lub bufor wiadomości. Aby uniknąć problemów, stosowane są mechanizmy synchronizacji:  

- **Mutexy** – zapewniają wyłączny dostęp do współdzielonych danych (np. listy klientów).  
- **Blokady (lock_guard, unique_lock)** – automatyzują zarządzanie blokadami, zmniejszając ryzyko deadlocków.  
- **Kolejki wiadomości** – wiadomości są buforowane i przetwarzane w sposób sekwencyjny, aby uniknąć wyścigów.  

## Strategie synchronizacji  
1. **Blokowanie mutexów przy modyfikacji danych** – każda operacja na współdzielonych zasobach jest chroniona przez mutex.  
2. **Wzorzec reactor/event loop** – serwer używa pętli zdarzeń do asynchronicznego zarządzania połączeniami, co zwiększa wydajność.  
3. **Pule wątków (thread pools)** – zamiast tworzyć nowy wątek dla każdego klienta, serwer korzysta z ograniczonej puli wątków, co zmniejsza narzut związany z tworzeniem wątków.  

## Instrukcja uruchamiania projektu  

### Wymagania  
- Kompilator C++ (np. g++, clang)  
- Standard C++11 lub nowszy  
- Biblioteka do obsługi sieci (np. Boost.Asio lub systemowe socket API)  

### Jak uruchomić:  
Sugerowanym sposobem uruchomienia programu jest użycie IDE z obsługą C++, takich jak:  
- Visual Studio  
- CLion  
- Code::Blocks  

Możliwa jest również kompilacja ręczna.  

#### Klonowanie repozytorium  
```bash
git clone https://github.com/[użytkownik]/Chat_Application.git  
cd Chat_Application  
```  

#### Kompilacja projektu  
```bash
g++ -std=c++11 -o server server.cpp -lpthread  
g++ -std=c++11 -o client client.cpp -lpthread  
```  

#### Uruchomienie programu  
1. **Serwer**:  
   ```bash
   ./server [port]  
   ```  
   (Domyślnie port `8080`, jeśli nie podano argumentu.)  

2. **Klient**:  
   ```bash
   ./client [adres_serwera] [port]  
   ```  
   (np. `./client localhost 8080`)  

Program umożliwia wieloużytkownikową komunikację w czasie rzeczywistym.
