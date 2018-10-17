#include <iostream>
#include <string>
#include <memory>
#include <set>
#include <vector>



class IA {
public:
    virtual std::string ReceiveData() = 0;
    virtual void Notify() = 0;
};

class IObserver {
public:
    virtual void Subscribe(std::shared_ptr<IA> new_subscriber) = 0;

    virtual void Unsubscribe(std::shared_ptr<IA> subscriber) = 0;

    virtual void Notify() = 0;
};

class IB {
public:
    virtual void PassObserver(std::shared_ptr<IObserver> c) = 0;
    virtual void SendData(std::string& new_data) = 0;
    virtual void PrintData() = 0;
};

template <int T>
class C {};

class A : public IA {
public:
    A() {
        notifed = false;
    };
    std::string ReceiveData() override {
        return "NEW_DATA";
    }

    void Notify() override {
        notifed = true;
    }

    void PrintStatus() {
        if( notifed ) {
            std::cout << "Notified" << std::endl;
        } else {
            std::cout << "Not notified" << std::endl;
        }
    }

private:
    bool notifed;
};


class B : public IB {
public:
    B() {
        data = "DATA";
    };

    void PassObserver( std::shared_ptr<IObserver> c ) override {
        observer = c;
    }

    void SendData(std::string& new_data) override {
        data = new_data;
    }

    void PrintData() override {
        std:: cout << data << std::endl;
    }

    void Change() {
        observer->Notify();
    }
private:

    std::shared_ptr<IObserver> observer;
    std::string data;
};


//Proxy
template <>
class C<1> {
public:
    C( std::shared_ptr<B> new_receiver ) {
        receiver = new_receiver;
    }

    void SendData(std::string& new_data) {
        receiver->SendData(new_data);
    }

private:
    std::shared_ptr<B> receiver;
};


//Mediator
template <>
class C<2> {
public:
    C( std::vector<std::shared_ptr<B>>& a ) {
        receivers = a;
    }

    void SendData(std::string& new_data, int i) {
        receivers[i]->SendData(new_data);
    }

    void PrintReceivers() {
        for(const auto &r : receivers ) {
            r->PrintData();
        }
    }
private:
    std::vector<std::shared_ptr<B>> receivers;
};


//Observer
template <>
class C<3> : public IObserver {
public:
    C( ) = default;

    void Subscribe(std::shared_ptr<IA> new_subscriber) override {
        subscribers.insert(new_subscriber);
    }

    void Unsubscribe(std::shared_ptr<IA> subscriber) override {
        subscribers.erase(subscriber);
    }

    void Notify() override {
        for( auto subscriber : subscribers ) {
            subscriber->Notify();
        }
    }

private:
    std::set<std::shared_ptr<IA>> subscribers;
};

int main() {
    //Proxy
    std::cout << "Proxy" << std::endl;
    {
        A a;
        std::shared_ptr<B> b = std::make_shared<B>();
        C<1> c(b);  // Proxy to B

        b->PrintData();
        std::string send_data = a.ReceiveData();
        c.SendData(send_data);  // Sending using proxy
        b->PrintData();
    }

    //Mediator
    std::cout << std::endl << "Mediator" << std::endl;
    {
        A a;
        std::vector<std::shared_ptr<B>> b;
        for( int i = 0; i < 4; ++i ) {
            b.push_back(std::make_shared<B>());
        }
        C<2> c(b);  // Proxy to several Bs


        c.PrintReceivers();
        std::cout << "--" << std::endl;
        std::string send_data = a.ReceiveData();
        c.SendData(send_data, 1);  // Sending using proxy
        c.PrintReceivers();
    }

    //Observer
    std::cout << std::endl << "Observer" << std::endl;
    {
        std::shared_ptr<C<3>> c = std::make_shared<C<3>>();  //Observer instance
        B b;
        b.PassObserver(c);  //Passing observer to B



        std::shared_ptr<A> a1 = std::make_shared<A>();
        std::shared_ptr<A> a2 = std::make_shared<A>();
        a1->PrintStatus();
        a2->PrintStatus();

        c->Subscribe(a1);  //Subscribe a1 to Observer
        b.Change();

        a1->PrintStatus();  //Notified
        a2->PrintStatus();  //Not notified


    }

    return 0;
}