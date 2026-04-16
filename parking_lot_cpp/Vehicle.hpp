#pragma once

#include <string>

enum class VehicleType { CAR, MOTORCYCLE, TRUCK, BUS };

class Vehicle {
public:
    Vehicle(std::string licensePlate, VehicleType type, std::string color);

    std::string getLicensePlate() const;
    VehicleType getType() const;
    std::string getColor() const;
    void displayInfo() const;

private:
    std::string licensePlate_;
    VehicleType type_;
    std::string color_;
};