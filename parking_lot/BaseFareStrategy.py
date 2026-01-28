from .vehicle import Vehicle
from decimal import Decimal
from FareStrategy import FareStrategy

class BaseFareStrategy(FareStrategy):
    def calculate_fare(self, vehicle: Vehicle, input_fare: Decimal) -> Decimal:
        return input_fare