#include "hardware/gpio.h"
#include "hardware/uart.h"
#include <algorithm>
#include <array>
#include <ranges>
#include <string>

import pico_uart_stream;

// UART defines
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// class uartostream {
// public:
//     uartostream(uart_inst_t *uart) : uart_(uart) {}

//     uartostream& operator << (const char* msg) {
//         // write to UART
//         uart_puts(uart_, msg);
//         return *this;
//     }

//     uartostream& operator << (const std::string& msg) {
//         uart_puts(uart_, msg.c_str());
//         return *this;
//     }

// private:
//     uart_inst_t * uart_;
// };

// class uartistream {
// public:
//     uartistream(uart_inst_t *uart) : uart_(uart) {}
  
//     uartistream& operator >> (std::string& msg) { // blocking
//         char c  = 0;
//         // msg.clear(); no. If the string has info, add to it.
//         do { 
//             c = uart_getc(uart_);
//             if (c != 255) {
//                 msg += c;
//             }
//         } while (c != '\n');
//         return *this;
//     }

// private:
//     uart_inst_t * uart_;
// };

// class uartiostream: public uartistream, public uartostream {
// public:    
//     uartiostream(uart_inst_t *uart) : uartistream(uart), uartostream(uart) {}
// };

class dog {
public:    
    dog (const std::string& name) : name(name) {};
    operator std::string() const {
        return "dog: " + name;
    }
private:
    const std::string name;
};

class cat {
public:    
    cat (const std::string& name) : name(name) {};
    operator std::string() const {
        return "cat: " + name;
    }
private:
    const std::string name;
};

struct customer {
    customer (const std::string& name, int credit) : name(name), credit(credit) {}
    const std::string name;
    int credit;
    operator std::string() const {
        return name;
    }
};

uart_inst_t * pico_init_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    return UART_ID;
}

int main() {
    pico_uart_stream::uartiostream u(pico_init_uart());
    u << "hello element14!\n" ;  

    dog d("odie");
    cat c("garfield");
    u << d << "\n" << c << "\n" ;  

    std::array<customer, 3> customers {{{"joe", 3}, {"jane", 500}, {"averall", -8}}};

    u << "insolvent customers:" << "\n";
    for(const auto &c : customers | std::views::filter([](const auto& c){ return c.credit < 0;})) {
                u << c << "\n";
    }

    u << "rich customers:" << "\n";
    for(const auto &c : customers | std::views::filter([](const auto& c){ return c.credit > 100;})) {
                u << c << "\n";
    }

    u << "full customer portfolio:" << "\n";
    std::for_each(customers.begin(), customers.end(),[&u](const auto& c) {
        u << c << "\n";
    });

    while (true) {
        std::string s;
        u >> s;
        u << s;
    }
}