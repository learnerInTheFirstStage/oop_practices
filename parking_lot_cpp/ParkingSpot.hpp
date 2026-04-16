#pragma once

#include "Vehicle.hpp"

enum class SpotType { COMPACT, REGULAR, LARGE };

class ParkingSpot {
public:
    ParkingSpot(int spotNumber, SpotType type);

    int getSpotNumber() const;
    SpotType getType() const;
    Vehicle* getVehicle() const;
    bool isAvailable() const;

    bool canFitVehicle(const Vehicle* vehicle) const;
    bool parkVehicle(Vehicle* vehicle);
    Vehicle* removeVehicle();
    void displayInfo() const;

private:
    int spotNumber_;
    SpotType type_;
    Vehicle* vehicle_;
    bool available_{true};
};