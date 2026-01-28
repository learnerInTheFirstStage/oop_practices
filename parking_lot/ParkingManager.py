class ParkingManager:
    def __init__(self):
        self.available_spots: dict[VehicleSize, List[ParkingSpot]] = {}
        self.vehicle_to_spot: dict[Vehicle, ParkingSpot] = {}

    def find_spot_for_vehicle(self, vehicle: Vehicle) -> ParkingSpot | None:
        spots = self.available_spots.get(vehicle.get_size(), [])
        for spot in spots:
            if spot.is_available():
                return spot
        return None
    
    def park_vehicle(self, vehicle: Vehicle) -> ParkingSpot:
        spot = self.find_spot_for_vehicle(vehicle)
        if not spot:
            raise Exception("No available spots found")
        spot.occupy(vehicle)
        self.vehicle_to_spot[vehicle] = spot
        return spot
    
    def unpark_vehicle(self, vehicle: Vehicle) -> None:
        spot = self.vehicle_to_spot.get(vehicle)
        if spot:
            spot.vacate()
            del self.vehicle_to_spot[vehicle]