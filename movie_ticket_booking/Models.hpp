#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>

// --- Pricing Strategy Pattern ---
class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual double getPrice() const = 0;
};

// --- Detailed Pricing Implementation ---
class NormalRate: public PricingStrategy {
public:
    double getPrice() const override { return 50.0; }
};

class VIPRate: public PricingStrategy {
public:
    double getPrice() const override { return 100.0; }
};

// --- SEAT ENTITY ---
class Seat {
private:
    std::string seatNumber;
    std::unique_ptr<PricingStrategy> pricingStrategy;

public:
    Seat(std::string num, std::unique_ptr<PricingStrategy> strategy)
        : seatNumber(std::move(num)), pricingStrategy(std::move(strategy)) {}
    
    // COPY Disable (Seat is unique), MOVE Allowed
    Seat(const Seat&) = delete;
    Seat& operator=(const Seat&) = delete;
    Seat(Seat&&) = default;

    std::string getNumber() const { return seatNumber; }
    double getPrice() const { return pricingStrategy->getPrice(); }
};

// === MOVIE ENTITY ===
class Movie {
    std::string title;
    std::string genre;
    std::chrono::minutes durations;
};