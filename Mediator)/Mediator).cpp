#include <iostream>
#include <string>
#include <vector>
#include <memory>

class User;


class Mediator {
public:
    virtual void sendMessage(const std::string& message, User* sender) = 0;
    virtual void addUser(User* user) = 0;
    virtual ~Mediator() = default;
};


class User {
protected:
    Mediator* mediator;
    std::string name;

public:
    User(Mediator* mediator, const std::string& name) : mediator(mediator), name(name) {}
    virtual void send(const std::string& message) {
        std::cout << name << " відправляє повідомлення: " << message << std::endl;
        mediator->sendMessage(message, this);
    }
    virtual void receive(const std::string& message) {
        std::cout << name << " отримує повідомлення: " << message << std::endl;
    }
    std::string getName() const { return name; }
};


class ChatMediator : public Mediator {
private:
    std::vector<User*> users;

public:
    void sendMessage(const std::string& message, User* sender) override {
        for (auto* user : users) {
            if (user != sender) {
                user->receive(message);
            }
        }
    }

    void addUser(User* user) override {
        users.push_back(user);
    }
};


class ConcreteUser : public User {
public:
    ConcreteUser(Mediator* mediator, const std::string& name) : User(mediator, name) {}
};


int main() {
    auto chat = std::make_unique<ChatMediator>();

    auto user1 = std::make_unique<ConcreteUser>(chat.get(), "Андрій");
    auto user2 = std::make_unique<ConcreteUser>(chat.get(), "Марія");
    auto user3 = std::make_unique<ConcreteUser>(chat.get(), "Іван");

    chat->addUser(user1.get());
    chat->addUser(user2.get());
    chat->addUser(user3.get());

    user1->send("Привіт всім!");
    user2->send("Привіт, Андрій!");
    user3->send("Привіт, друзі!");

    return 0;
}
