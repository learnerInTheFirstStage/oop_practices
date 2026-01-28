from .vehicle import Vehicle
from decimal import Decimal
from FareStrategy import FareStrategy

class PeakHoursFareStrategy(FareStrategy):
    def calculate_fare(self, vehicle: Vehicle, input_fare: Decimal) -> Decimal:
        return input_fare * Decimal("1.5")