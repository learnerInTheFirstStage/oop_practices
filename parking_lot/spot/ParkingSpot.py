from abc import ABC, abstractmethod

class ParkingSpot(ABC):
    @abstractmethod
    def is_available(self) -> bool:
        pass

    @abstractmethod
    def occupy(self, vehicle: Vehicle) -> None:
        pass

    @abstractmethod
    def vacate(self) -> None:
        pass

    @abstractmethod
    def get_spot_number(self) -> int:
        pass

    @abstractmethod
    def get_size(self) -> VehicleSize:
        pass
