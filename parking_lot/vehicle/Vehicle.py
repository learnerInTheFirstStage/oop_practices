from enum import Enum
from abc import ABC, abstractmethod

class VehicleSize(Enum):
    SMALL = "SMALL"
    MEDIUM = "MEDIUM"
    LARGE = "LARGE"

class Vehicle(ABC):
    @abstractmethod
    def get_license_plate(self) -> str:
        pass

    @abstractmethod
    def get_size(self) -> VehicleSize:
        pass