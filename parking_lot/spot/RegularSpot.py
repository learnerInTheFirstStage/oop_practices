from ..vehicle import Vehicle, VehicleSize
from ParkingSpot import ParkingSpot

class RegularSpot(ParkingSpot):
    def __init__(self, spot_number: int):
        self.spot_number = spot_number
        self.vehicle = None
    
    def is_available(self) -> bool:
        return self.vehicle is None
    
    def occupy(self, vehicle: Vehicle) -> None:
        self.vehicle = vehicle
    
    def vacate(self) -> None:
        self.vehicle = None
    
    def get_spot_number(self) -> int:
        return self.spot_number
    
    def get_size(self) -> VehicleSize:
        return VehicleSize.MEDIUM