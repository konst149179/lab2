#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>
#include <cctype>

using namespace std;


enum class Destination {
    MOSCOW = 1,
    SAINT_PETERSBURG,
    EKATERINBURG,
    NOVOSIBIRSK,
    SOCHI,
    KRASNODAR
};


int getIntegerInput(const string& prompt);
int inputInt(const string& prompt, int min = 1, int max = 2000000000);
double inputDouble(const string& prompt, double min = 1.0, double max = 1000000.0);
string inputString(const string& prompt);
string inputName(const string& prompt);
string inputPassport(const string& prompt);
Destination inputDestination();


class Station;
class Passenger;


class Passenger {
private:
    string passport;
    string name;
    string surname;
public:
    Passenger(const string& pass, const string& n, const string& s)
        : passport(pass), name(n), surname(s) {}

    string getPassport() const { return passport; }
    string getName() const { return name; }
    string getSurname() const { return surname; }

    void getInfo() const {
        cout << "Passport: " << passport << endl;
        cout << "Name: " << name << endl;
        cout << "Surname: " << surname << endl;
    }
};


class Ticket {
private:
    Destination destination;
    double price;
    Passenger* passenger;
public:
    Ticket(Destination dest, double p, Passenger* pass)
        : destination(dest), price(p), passenger(pass) {}

    Destination getDestination() const { return destination; }
    double getPrice() const { return price; }
    Passenger* getPassenger() const { return passenger; }

    void getInfo() const {
        cout << "Destination: ";
        switch (destination) {
        case Destination::MOSCOW: cout << "Moscow"; break;
        case Destination::SAINT_PETERSBURG: cout << "Saint Petersburg"; break;
        case Destination::EKATERINBURG: cout << "Ekaterinburg"; break;
        case Destination::NOVOSIBIRSK: cout << "Novosibirsk"; break;
        case Destination::SOCHI: cout << "Sochi"; break;
        case Destination::KRASNODAR: cout << "Krasnodar"; break;
        }
        cout << endl;
        cout << "Price: " << price << " RUB" << endl;
        cout << "Passenger: " << passenger->getName() << " " << passenger->getSurname() << endl;
    }
};


class Cashbox {
private:
    vector<Ticket*> tickets;
    vector<Passenger*> passengers;
    map<Destination, double> tariffs;
    Station* station;

public:
    Cashbox(Station* st) : station(st) {
        
        tariffs[Destination::MOSCOW] = 1500.0;
        tariffs[Destination::SAINT_PETERSBURG] = 1200.0;
        tariffs[Destination::EKATERINBURG] = 2000.0;
        tariffs[Destination::NOVOSIBIRSK] = 3500.0;
        tariffs[Destination::SOCHI] = 2500.0;
        tariffs[Destination::KRASNODAR] = 2200.0;

        cout << "Cashbox created and bound to station" << endl;
    }

    ~Cashbox() {
        for (auto ticket : tickets) {
            delete ticket;
        }
        for (auto passenger : passengers) {
            delete passenger;
        }
        cout << "Cashbox closed" << endl;
    }

    void addTariff() {
        cout << "\n=== ADD TARIFF ===" << endl;
        Destination dest = inputDestination();
        double price = inputDouble("Enter tariff price (RUB): ", 1.0, 100000.0);

        tariffs[dest] = price;
        cout << "Tariff successfully added/updated!" << endl;
    }

    void addPassenger() {
        cout << "\n=== REGISTER PASSENGER ===" << endl;
        string passport = inputPassport("Enter passport number: ");

        for (auto passenger : passengers) {
            if (passenger->getPassport() == passport) {
                cout << "Passenger with this passport already registered!" << endl;
                return;
            }
        }

        string name = inputName("Enter name: ");
        string surname = inputName("Enter surname: ");

        Passenger* newPassenger = new Passenger(passport, name, surname);
        passengers.push_back(newPassenger);

        cout << "Passenger successfully registered!" << endl;
        cout << "Total passengers in system: " << passengers.size() << endl;
    }

    void buyTicket() {
        cout << "\n=== BUY TICKET ===" << endl;

        if (passengers.empty()) {
            cout << "No registered passengers!" << endl;
            return;
        }

        cout << "Passenger list:" << endl;
        for (int i = 0; i < passengers.size(); i++) {
            cout << i + 1 << ". ";
            passengers[i]->getInfo();
            cout << "---" << endl;
        }

        int passengerIndex = inputInt("Select passenger: ", 1, passengers.size()) - 1;
        Passenger* selectedPassenger = passengers[passengerIndex];

        Destination dest = inputDestination();

        if (tariffs.find(dest) == tariffs.end()) {
            cout << "Tariff for this destination is not set!" << endl;
            return;
        }

        double price = tariffs[dest];

        Ticket* newTicket = new Ticket(dest, price, selectedPassenger);
        tickets.push_back(newTicket);

        cout << "Ticket successfully purchased at station cashbox!" << endl;
        newTicket->getInfo();
        cout << "Total tickets sold: " << tickets.size() << endl;
    }

    void getPassengersByDestination() {
        cout << "\n=== PASSENGERS BY DESTINATION ===" << endl;

        Destination dest = inputDestination();

        cout << "Passengers who bought tickets for: ";
        switch (dest) {
        case Destination::MOSCOW: cout << "Moscow"; break;
        case Destination::SAINT_PETERSBURG: cout << "Saint Petersburg"; break;
        case Destination::EKATERINBURG: cout << "Ekaterinburg"; break;
        case Destination::NOVOSIBIRSK: cout << "Novosibirsk"; break;
        case Destination::SOCHI: cout << "Sochi"; break;
        case Destination::KRASNODAR: cout << "Krasnodar"; break;
        }
        cout << endl;

        bool found = false;
        for (auto ticket : tickets) {
            if (ticket->getDestination() == dest) {
                ticket->getPassenger()->getInfo();
                cout << "---" << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No tickets sold for this destination." << endl;
        }
    }

    void calculateTotalCost() {
        cout << "\n=== CALCULATE TICKET COST ===" << endl;

        if (passengers.empty()) {
            cout << "No registered passengers!" << endl;
            return;
        }

        cout << "Passenger list:" << endl;
        for (int i = 0; i < passengers.size(); i++) {
            cout << i + 1 << ". ";
            passengers[i]->getInfo();
            cout << "---" << endl;
        }

        int passengerIndex = inputInt("Select passenger: ", 1, passengers.size()) - 1;
        Passenger* selectedPassenger = passengers[passengerIndex];

        double totalCost = 0.0;
        int ticketCount = 0;

        for (auto ticket : tickets) {
            if (ticket->getPassenger() == selectedPassenger) {
                totalCost += ticket->getPrice();
                ticketCount++;
            }
        }

        cout << "Passenger: " << selectedPassenger->getName() << " " << selectedPassenger->getSurname() << endl;
        cout << "Number of tickets from cashbox: " << ticketCount << endl;
        cout << "Total cost: " << totalCost << " RUB" << endl;
    }

    void showTariffs() {
        cout << "\n=== CASHBOX TARIFFS ===" << endl;
        for (auto& tariff : tariffs) {
            cout << "Destination: ";
            switch (tariff.first) {
            case Destination::MOSCOW: cout << "Moscow"; break;
            case Destination::SAINT_PETERSBURG: cout << "Saint Petersburg"; break;
            case Destination::EKATERINBURG: cout << "Ekaterinburg"; break;
            case Destination::NOVOSIBIRSK: cout << "Novosibirsk"; break;
            case Destination::SOCHI: cout << "Sochi"; break;
            case Destination::KRASNODAR: cout << "Krasnodar"; break;
            }
            cout << " - " << tariff.second << " RUB" << endl;
        }
    }

    void showStatistics() {
        cout << "\n=== CASHBOX STATISTICS ===" << endl;
        cout << "Number of registered passengers: " << passengers.size() << endl;
        cout << "Number of tickets sold: " << tickets.size() << endl;
        cout << "Number of tariffs set: " << tariffs.size() << endl;

        double totalRevenue = 0.0;
        for (auto ticket : tickets) {
            totalRevenue += ticket->getPrice();
        }
        cout << "Total cashbox revenue: " << totalRevenue << " RUB" << endl;
    }

    vector<Passenger*>& getPassengers() { return passengers; }
    vector<Ticket*>& getTickets() { return tickets; }
    map<Destination, double>& getTariffs() { return tariffs; }
};

class Station {
//приватный статич указатель
private:
    static Station* instance;
    Cashbox* cashbox;
    //приватный конструктор
    Station() {
        cashbox = new Cashbox(this);
        cout << "Station instance created (Singleton) with cashbox" << endl;
    }

public:
    //запрет копирования и присваивания
    Station(const Station&) = delete;
    Station& operator=(const Station&) = delete;
    //метод явного уничтожения
    static Station* getInstance() {
        if (instance == nullptr) {
            instance = new Station();
        }
        return instance;
    }

    static void destroyInstance() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }

    ~Station() {
        delete cashbox;
        cout << "Station closed" << endl;
    }

    Cashbox* getCashbox() {
        return cashbox;
    }

    void showInfo() {
        cout << "\n=== STATION INFORMATION ===" << endl;
        cout << "Station operates using Singleton pattern" << endl;
        cout << "Station has one main cashbox" << endl;
        cout << "Address: Main Station, City Center" << endl;
    }
};

Station* Station::instance = nullptr;

int getIntegerInput(const string& prompt) {
    string inputStr;
    cout << prompt;
    getline(cin, inputStr);

    if (inputStr.empty()) {
        return -1;
    }

    for (char c : inputStr) {
        if (!isdigit(c)) {
            return -1;
        }
    }

    try {
        return stoi(inputStr);
    }
    catch (...) {
        return -1;
    }
}

int inputInt(const string& prompt, int min, int max) {
    int value;
    bool firstAttempt = true;

    do {
        if (firstAttempt) {
            cout << prompt;
            firstAttempt = false;
        }

        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cout << "Error! Enter a number from " << min << " to " << max << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    } while (true);
    return value;
}

double inputDouble(const string& prompt, double min, double max) {
    double value;
    bool firstAttempt = true;

    do {
        if (firstAttempt) {
            cout << prompt;
            firstAttempt = false;
        }

        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cout << "Error! Enter a number from " << min << " to " << max << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    } while (true);
    return value;
}

string inputString(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

bool isValidName(const string& str) {
    if (str.empty()) return false;

    for (char c : str) {
        if (!isalpha(c) && c != ' ' && c != '-') {
            return false;
        }
    }
    return true;
}

bool isValidPassport(const string& str) {
    if (str.empty()) return false;

    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }

    return str.length() == 6;
}

string inputName(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);

        if (!isValidName(value)) {
            cout << "Error! Name can only contain letters, spaces and hyphens. Please try again: ";
        }
        else if (value.length() < 2) {
            cout << "Error! Name must be at least 2 characters long. Please try again: ";
        }
        else if (value.length() > 50) {
            cout << "Error! Name is too long (max 50 characters). Please try again: ";
        }
        else {
            break;
        }
    } while (true);

    bool capitalizeNext = true;
    for (char& c : value) {
        if (capitalizeNext && isalpha(c)) {
            c = toupper(c);
            capitalizeNext = false;
        }
        else if (c == ' ' || c == '-') {
            capitalizeNext = true;
        }
    }

    return value;
}

string inputPassport(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);

        if (!isValidPassport(value)) {
            cout << "Error! Passport number must contain exactly 6 digits. Please try again: ";
        }
        else {
            break;
        }
    } while (true);

    return value;
}

Destination inputDestination() {
    cout << "Select destination:" << endl;
    cout << "1. Moscow" << endl;
    cout << "2. Saint Petersburg" << endl;
    cout << "3. Ekaterinburg" << endl;
    cout << "4. Novosibirsk" << endl;
    cout << "5. Sochi" << endl;
    cout << "6. Krasnodar" << endl;

    int choice = inputInt("Your choice: ", 1, 6);
    return static_cast<Destination>(choice);
}

void cashboxMenu(Cashbox* cashbox) {
    while (true) {
        cout << "\n=== STATION CASHBOX ===" << endl;
        cout << "1. Add/update tariff" << endl;
        cout << "2. Register passenger" << endl;
        cout << "3. Buy ticket" << endl;
        cout << "4. Passengers by destination" << endl;
        cout << "5. Calculate passenger ticket cost" << endl;
        cout << "6. Show all tariffs" << endl;
        cout << "7. Cashbox statistics" << endl;
        cout << "8. Back to main menu" << endl;

        int choice;
        do {
            choice = getIntegerInput("Select action (1-8): ");
            if (choice < 1 || choice > 8) {
                cout << "Error! Please enter a number from 1 to 8: ";
            }
        } while (choice < 1 || choice > 8);

        switch (choice) {
        case 1:
            cashbox->addTariff();
            break;
        case 2:
            cashbox->addPassenger();
            break;
        case 3:
            cashbox->buyTicket();
            break;
        case 4:
            cashbox->getPassengersByDestination();
            break;
        case 5:
            cashbox->calculateTotalCost();
            break;
        case 6:
            cashbox->showTariffs();
            break;
        case 7:
            cashbox->showStatistics();
            break;
        case 8:
            return;
        }
    }
}

void showMenu() {
    Station* station = Station::getInstance();

    while (true) {
        cout << "\n=== MAIN MENU - STATION SYSTEM ===" << endl;
        cout << "1. Work with cashbox" << endl;
        cout << "2. Station information" << endl;
        cout << "3. Exit" << endl;

        int choice;
        do {
            choice = getIntegerInput("Select action (1-3): ");
            if (choice < 1 || choice > 3) {
                cout << "Error! Please enter a number from 1 to 3: ";
            }
        } while (choice < 1 || choice > 3);

        switch (choice) {
        case 1:
            cashboxMenu(station->getCashbox());
            break;
        case 2:
            station->showInfo();
            break;
        case 3:
            Station::destroyInstance();
            return;
        }
    }
}
int main() {
    //в памяти существует только ОДИН объект Station, и все вызовы getInstance() возвращают указатель на него
    Station* a = Station::getInstance();
    Station* b = Station::getInstance();
    cout << "Address of a: " << a << endl;
    cout << "Address of b: " << b << endl;

    showMenu();
    return 0;
}