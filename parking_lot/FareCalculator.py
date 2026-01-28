class FareCalculator():
    def __init__(self, strategies: List[FareStrategy]):
        self.strategies = strategies
    
    def calculate_fare(self, ticket: Ticket) -> Decimal:
        base_fare = ticket.calculate_parking_duration() * Decimal("10")
        fare = base_fare
        for strategy in self.strategies:
            fare = strategy.calculate_fare(ticket.vehicle, fare)
        return fare