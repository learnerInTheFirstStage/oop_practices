from Vehicle import Vehicle, VehicleSize

class Motorcycle(Vehicle):
    def __init__(self, license_plate: str):
        self.license_plate = license_plate
    
    def get_license_plate(self) -> str:
        return self.license_plate
    
    def get_size(self) -> VehicleSize:
        return VehicleSize.SMALL