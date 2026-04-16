#include "Vehicle.hpp"
#include <iostream>

Vehicle::Vehicle(std::string licensePlate, VehicleType type, std::string color)
    : licensePlate_(licensePlate), type_(type), color_(color) {}

std::string Vehicle::getLicensePlate() const { return licensePlate_; }
VehicleType Vehicle::getType() const { return type_; }
std::string Vehicle::getColor() const { return color_; }

void Vehicle::displayInfo() const {
    std::cout << "Vehicle: " << color_ << " ";
    switch (type_) {
        case VehicleType::CAR: std::cout << "Car"; break;
        case VehicleType::MOTORCYCLE: std::cout << "Motorcycle"; break;
        case VehicleType::TRUCK: std::cout << "Truck"; break;
        case VehicleType::BUS: std::cout << "Bus"; break;
    }
    std::cout << " (License: " << licensePlate_ << ")" << std::endl;
}