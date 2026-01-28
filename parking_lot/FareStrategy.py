from abc import ABC, abstractmethod
from .vehicle import Vehicle
from decimal import Decimal

class FareStrategy(ABC):
    @abstractmethod
    def calculate_fare(self, vehicle: Vehicle, input_fare: Decimal) -> Decimal:
        pass