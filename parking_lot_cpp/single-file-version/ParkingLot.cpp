#include <chrono>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

// Enums
enum class VehicleType { CAR, MOTORCYCLE, TRUCK, BUS };
enum class SpotType { COMPACT, REGULAR, LARGE };

// Vehicle
class Vehicle {
public:
    Vehicle(string licensePlate, VehicleType type) : licensePlate_(licensePlate), type_(type) {}

    VehicleType getType() const { return type_; }
    const string& getLicensePlate() const { return licensePlate_; }

private:
    string licensePlate_;
    VehicleType type_;
};

// ParkingSpot
class ParkingSpot {
public:
    ParkingSpot(int spotNumber, SpotType type) : spotNumber_(spotNumber), type_(type) {}

    int getSpotNumber() const { return spotNumber_; }
    SpotType getType() const { return type_; }
    Vehicle* getVehicle() const { return vehicle_; }
    bool isAvailable() const { return available_; }

    bool canFitVehicle(const Vehicle* vehicle) const {
        if (!vehicle) return false;
        switch (vehicle->getType()) {
            case VehicleType::MOTORCYCLE:
                return true;
            case VehicleType::CAR:
                return type_ != SpotType::COMPACT;
            case VehicleType::TRUCK:
            case VehicleType::BUS:
                return type_ == SpotType::LARGE;
        }
        return false;
    }

    bool parkVehicle(Vehicle* vehicle) {
        if (!available_ || !canFitVehicle(vehicle)) return false;
        vehicle_ = vehicle;
        available_ = false;
        return true;
    }

    Vehicle* removeVehicle() {
        if (!vehicle_) return nullptr;
        Vehicle* removedVehicle = vehicle_;
        vehicle_ = nullptr;
        available_ = true;
        return removedVehicle;
    }

private:
    int spotNumber_;
    SpotType type_;
    Vehicle* vehicle_{nullptr};
    bool available_{true};
};

// Ticket
class Ticket {
public:
    Ticket(string licensePlate, ParkingSpot* spot)
        : licensePlate_(licensePlate), spot_(spot), enntryTime_(chrono::steady_clock::now()) {}

    const string& getLicensePlate() const { return licensePlate_; }
    ParkingSpot* getSpot() const { return spot_; }

    double getDurationHours() const {
        auto duration = chrono::steady_clock::now() - enntryTime_;
        return chrono::duration<double, ratio<3600>>(duration).count();
    }

private:
    string licensePlate_;
    ParkingSpot* spot_;
    chrono::steady_clock::time_point enntryTime_;
};

// FeeCalculator (Strategy Pattern)
class FeeCalculator {
public:
    virtual double calculate(const Ticket& ticket, VehicleType type) const = 0;
    virtual ~FeeCalculator() = default;
};

// HourlyFee
class HourlyFee : public FeeCalculator {
public:
    explicit HourlyFee(double ratePerHour) : rate_(ratePerHour) {}

    double calculate(const Ticket& ticket, VehicleType type) const override {
        auto hours = ticket.getDurationHours();
        return ceil(max(hours, 1.0 / 60)) * rate_;
    }

private:
    double rate_;
};

// VehicleTypeFee
class VehicleTypeFee : public FeeCalculator {
public:
    double calculate(const Ticket& ticket, VehicleType type) const override {
        double hours = ticket.getDurationHours();
        double rate = 2.0;
        switch (type) {
            case VehicleType::MOTORCYCLE:
                rate = 1.0;
                break;
            case VehicleType::CAR:
                rate = 2.0;
                break;
            case VehicleType::TRUCK:
            case VehicleType::BUS:
                rate = 5.0;
                break;
        }
        return ceil(max(hours, 1.0 / 60)) * rate;
    }
};

// SpotTypeFee
class SpotTypeFee : public FeeCalculator {
public:
    double calculate(const Ticket& ticket, VehicleType type) const override {
        double hours = ticket.getDurationHours();
        double rate = 2.0;
        switch (ticket.getSpot()->getType()) {
            case SpotType::COMPACT:
                rate = 1.0;
                break;
            case SpotType::REGULAR:
                rate = 2.0;
                break;
            case SpotType::LARGE:
                rate = 4.0;
                break;
        }
        return ceil(max(hours, 1.0 / 60)) * rate;
    }
};

// ParkingLot
class ParkingLot {
public:
    ParkingLot(int numCompact, int numRegular, int numLarge, FeeCalculator& calculator)
        : capacity_(numCompact + numRegular + numLarge),
          availableSpots_(capacity_),
          calculator_(calculator) {
        spots_.reserve(capacity_);
        int spotNumber = 1;

        for (int i = 0; i < numCompact; ++i)
            spots_.push_back(make_unique<ParkingSpot>(spotNumber++, SpotType::COMPACT));
        for (int i = 0; i < numRegular; ++i)
            spots_.push_back(make_unique<ParkingSpot>(spotNumber++, SpotType::REGULAR));
        for (int i = 0; i < numLarge; ++i)
            spots_.push_back(make_unique<ParkingSpot>(spotNumber++, SpotType::LARGE));
    }

    int getCapacity() const { return capacity_; }
    int getAvailableSpots() const { return availableSpots_; }

    Ticket* parkVehicle(Vehicle* vehicle) {
        if (!vehicle) return nullptr;

        if (tickets_.count(vehicle->getLicensePlate())) return nullptr;

        ParkingSpot* spot = findAvailableSpot(vehicle);
        if (!spot) return nullptr;

        spot->parkVehicle(vehicle);
        auto ticket = make_unique<Ticket>(vehicle->getLicensePlate(), spot);
        Ticket* ticketPtr = ticket.get();
        tickets_[vehicle->getLicensePlate()] = std::move(ticket);
        availableSpots_--;
        return ticketPtr;
    }

    double removeVehicle(const string& licensePlate) {
        auto it = tickets_.find(licensePlate);
        if (it == tickets_.end()) return -1.0;

        Ticket& ticket = *it->second;
        Vehicle* vehicle = ticket.getSpot()->getVehicle();
        double fee = calculator_.calculate(ticket, vehicle->getType());

        ticket.getSpot()->removeVehicle();
        tickets_.erase(it);
        availableSpots_++;
        return fee;
    }

private:
    ParkingSpot* findAvailableSpot(const Vehicle* vehicle) {
        for (const auto& spot : spots_) {
            if (spot->isAvailable() && spot->canFitVehicle(vehicle)) return spot.get();
        }
        return nullptr;
    }

    int capacity_;
    int availableSpots_;
    FeeCalculator& calculator_;
    vector<unique_ptr<ParkingSpot>> spots_;
    unordered_map<string, unique_ptr<Ticket>> tickets_;
};

// main
int main() {
    VehicleTypeFee calculator;
    ParkingLot lot{2, 3, 2, calculator};

    Vehicle car("CAR001", VehicleType::CAR);
    Vehicle moto("MOTO001", VehicleType::MOTORCYCLE);
    Vehicle truck("TRUCK001", VehicleType::TRUCK);

    auto tryPark = [&](Vehicle& v) {
        Ticket* t = lot.parkVehicle(&v);
        if (t) {
            cout << "Parked!";
        } else {
            cout << "Failed to park!" << "\n";
        };
    };

    tryPark(car);
    tryPark(moto);
    tryPark(truck);

    cout << "\nRemoving vehicles and calculating fees...\n";

    auto checkout = [&](const string& plate) {
        double fee = lot.removeVehicle(plate);
        if (fee >= 0)
            cout << "Fee for " << plate << ": $" << fee << "\n";
        else
            cout << plate << " not found in lot.\n";
    };

    checkout("CAR001");
    checkout("MOTO001");
    checkout("TRUCK001");
    checkout("UNKNOWN");

    return 0;
}