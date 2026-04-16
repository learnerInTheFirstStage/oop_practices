#pragma once

#include <map>
#include <string>
#include <vector>

#include "ParkingSpot.hpp"

class ParkingLot {
public:
    ParkingLot(int numCompact, int numRegular, int numLarge);
    ~ParkingLot();

    int getCapacity() const;
    int getAvailableSpots() const;

    bool parkVehicle(Vehicle* vehicle);
    Vehicle* removeVehicle(const std::string& licensePlate);
    ParkingSpot* findVehicle(const std::string& licensePlate) const;

    void displayInfo() const;
    void displayOccupancy() const;

private:
    std::vector<ParkingSpot*> spots_;
    std::map<std::string, ParkingSpot*> occupiedSpots_;
    int capacity_;
    int availableSpots_;

private:
    ParkingSpot* findAvailableSpot(const Vehicle* vehicle) const;
};