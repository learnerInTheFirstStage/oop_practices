from datetime import datetime
from typing import Optional
from .vehicle import Vehicle
from .spot import ParkingSpot
from decimal import Decimal

class Ticket:
    """
    Represents a parking ticket with details like ticket ID, vehicle, parking spot, and timestamps.
    """
    def __init__(
        self,
        ticket_id: str,
        vehicle: Vehicle,
        parking_spot: ParkingSpot,
        entry_time: datetime,
        exit_time: Optional[datetime] = None,
    ):
        """
        Initialize a parking ticket.

        Args:
            ticket_id: Unique identifier for the ticket.
            vehicle: Vehicle associated with the ticket.
            parking_spot: Parking spot assigned to the vehicle.
            entry_time: Timestamp when the vehicle entered the parking lot.
            exit_time: Timestamp when the vehicle exited the parking lot (optional).
        """
        self.ticket_id = ticket_id
        self.vehicle = vehicle
        self.parking_spot = parking_spot
        self.entry_time = entry_time
        self.exit_time = exit_time

    def calculate_parking_duration(self) -> Decimal:
        if not self.exit_time:
            raise ValueError('Vehicle has not exited yet')
        duration = self.exit_time - self.entry_time
        return Decimal(duration.total_seconds() / 3600)