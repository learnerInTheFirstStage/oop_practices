#include "ParkingLot.hpp"

#include <iostream>

ParkingLot::ParkingLot(int numCompact, int numRegular, int numLarge)
    : capacity_(numCompact + numRegular + numLarge), availableSpots_(capacity_) {
    spots_.reserve(capacity_);

    int spotNumber = 1;

    for (int i = 0; i < numCompact; ++i) {
        spots_.push_back(new ParkingSpot(spotNumber++, SpotType::COMPACT));
    }

    for (int i = 0; i < numRegular; ++i) {
        spots_.push_back(new ParkingSpot(spotNumber++, SpotType::REGULAR));
    }

    for (int i = 0; i < numLarge; ++i) {
        spots_.push_back(new ParkingSpot(spotNumber++, SpotType::LARGE));
    }
}

ParkingLot::~ParkingLot() {
    for (auto& spot : spots_) delete spot;
}

int ParkingLot::getCapacity() const { return capacity_; }
int ParkingLot::getAvailableSpots() const { return availableSpots_; }

bool ParkingLot::parkVehicle(Vehicle* vehicle) {
    if (!vehicle) return false;

    if (occupiedSpots_.find(vehicle->getLicensePlate()) != occupiedSpots_.end()) {
        return false;
    }

    ParkingSpot* spot = findAvailableSpot(vehicle);
    if (!spot) return false;

    if (spot->parkVehicle(vehicle)) {
        occupiedSpots_[vehicle->getLicensePlate()] = spot;
        availableSpots_--;
        return true;
    }
    return false;
}

Vehicle* ParkingLot::removeVehicle(const std::string& licensePlate) {
    auto it = occupiedSpots_.find(licensePlate);
    if (it == occupiedSpots_.end()) return nullptr;

    ParkingSpot* spot = it->second;
    Vehicle* vehicle = spot->removeVehicle();
    if (vehicle) {
        occupiedSpots_.erase(it);
        availableSpots_++;
    }
    return vehicle;
}

ParkingSpot* ParkingLot::findVehicle(const std::string& licensePlate) const {
    auto it = occupiedSpots_.find(licensePlate);
    return it != occupiedSpots_.end() ? it->second : nullptr;
}

void ParkingLot::displayInfo() const {
    std::cout << "\nParking Lot Status:" << std::endl;
    std::cout << "Total Capacity: " << capacity_ << std::endl;
    std::cout << "Available Spots: " << availableSpots_ << std::endl;
    std::cout << "Occupied Spots: " << (capacity_ - availableSpots_) << std::endl;
}

void ParkingLot::displayOccupancy() const {
    std::cout << "\nDetailed Occupancy:" << std::endl;
    for (const auto& spot : spots_) {
        spot->displayInfo();
    }
}

ParkingSpot* ParkingLot::findAvailableSpot(const Vehicle* vehicle) const {
    for (auto& spot : spots_) {
        if (spot->isAvailable() && spot->canFitVehicle(vehicle)) return spot;
    }
    return nullptr;
}