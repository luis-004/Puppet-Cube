#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Contact {
    std::string name;
    std::string phone;

    void display() const {
        std::cout << "Name: " << name << ", Phone: " << phone << '\n';
    }
};

class ContactManager {
private:
    std::vector<Contact> contacts;

public:
    void addContact(const std::string& name, const std::string& phone) {
        contacts.push_back({name, phone});
    }

    void listContacts() const {
        if (contacts.empty()) {
            std::cout << "No contacts available.\n";
            return;
        }
        std::cout << "Contact List:\n";
        for (const auto& contact : contacts) {
            contact.display();
        }
    }

    void searchContact(const std::string& keyword) const {
        bool found = false;
        for (const auto& contact : contacts) {
            if (contact.name.find(keyword) != std::string::npos) {
                contact.display();
                found = true;
            }
        }
        if (!found) {
            std::cout << "No contacts found with name containing \"" << keyword << "\".\n";
        }
    }
};

int main() {
    ContactManager manager;
    int choice;
    std::string name, phone, keyword;

    do {
        std::cout << "\n--- Contact Manager ---\n";
        std::cout << "1. Add Contact\n2. List Contacts\n3. Search Contact\n4. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        std::cin.ignore(); // clear newline

        switch (choice) {
            case 1:
                std::cout << "Enter name: ";
                std::getline(std::cin, name);
                std::cout << "Enter phone: ";
                std::getline(std::cin, phone);
                manager.addContact(name, phone);
                break;
            case 2:
                manager.listContacts();
                break;
            case 3:
                std::cout << "Enter name to search: ";
                std::getline(std::cin, keyword);
                manager.searchContact(keyword);
                break;
            case 4:
                std::cout << "Exiting Contact Manager.\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
