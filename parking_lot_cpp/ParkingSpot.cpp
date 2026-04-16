#include "ParkingSpot.hpp"

#include <iostream>

ParkingSpot::ParkingSpot(int spotNumber, SpotType type) : spotNumber_(spotNumber), type_(type) {}

int ParkingSpot::getSpotNumber() const { return spotNumber_; }
SpotType ParkingSpot::getType() const { return type_; }
Vehicle* ParkingSpot::getVehicle() const { return vehicle_; }
bool ParkingSpot::isAvailable() const { return available_; }

bool ParkingSpot::canFitVehicle(const Vehicle* vehicle) const {
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

bool ParkingSpot::parkVehicle(Vehicle* vehicle) {
    if (!available_ || !canFitVehicle(vehicle)) return false;

    this->vehicle_ = vehicle;
    available_ = false;
    return true;
}

Vehicle* ParkingSpot::removeVehicle() {
    if (!vehicle_) return nullptr;

    Vehicle* removedVehicle = vehicle_;
    vehicle_ = nullptr;
    available_ = true;
    return removedVehicle;
}

void ParkingSpot::displayInfo() const {
    std::cout << "Spot " << spotNumber_ << " (";
    switch (type_) {
        case SpotType::COMPACT:
            std::cout << "Compact";
            break;
        case SpotType::REGULAR:
            std::cout << "Regular";
            break;
        case SpotType::LARGE:
            std::cout << "Large";
            break;
    }
    std::cout << "): " << (available_ ? "Available" : "Occupied");
    if (vehicle_) {
        std::cout << " by ";
        vehicle_->displayInfo();
    } else {
        std::cout << std::endl;
    }
}