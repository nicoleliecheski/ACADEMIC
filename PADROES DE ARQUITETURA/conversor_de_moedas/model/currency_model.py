class CurrencyModel:
    
    RATES_TO_USD = {
        "USD": 1.0,
        "BRL": 5.40,
        "EUR": 0.92,
        "GBP": 0.79,
        "JPY": 155.0,
        "ARS": 970.0,
    }

    def available_currencies(self):
        return sorted(self.RATES_TO_USD.keys())

    def convert(self, amount, from_currency, to_currency):
        if from_currency not in self.RATES_TO_USD:
            raise ValueError(f"Unknown currency: {from_currency}")
        if to_currency not in self.RATES_TO_USD:
            raise ValueError(f"Unknown currency: {to_currency}")
        if amount < 0:
            raise ValueError("Valor precisa ser maior ou igual a 0")

        amount_in_usd = amount / self.RATES_TO_USD[from_currency]
        result = amount_in_usd * self.RATES_TO_USD[to_currency]
        return round(result, 4)
