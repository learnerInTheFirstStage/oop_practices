from datetime import datetime
import uuid
from .vehicle import Vehicle
from .ticket import Ticket
from .ParkingManager import ParkingManager
from .FareCalculator import FareCalculator
from decimal import Decimal

class ParkingLot:
    def __init__(self, parking_manager: ParkingManager, fare_calculator: FareCalculator):
        self.parking_manager = parking_manager
        self.fare_calculator = fare_calculator
    
    # Method to handle vehicle entry into the parking lot
    def enter_vehicle(self, vehicle: Vehicle):
        spot = self.parking_manager.park_vehicle(vehicle)
        if spot:
            ticket = Ticket(generate_ticket_id(), vehicle, spot, datetime.now())
            return ticket
        raise Exception("No available spots found")

    def leave_vehicle(self, ticket: Ticket) -> Decimal:
        self.parking_manager.unpark_vehicle(ticket.vehicle)
        ticket.exit_time = datetime.now()
        return self.fare_calculator.calculate_fare(ticket)
    
    def generate_ticket_id(self):
        return "TICKET-" + str(uuid.uuid4())