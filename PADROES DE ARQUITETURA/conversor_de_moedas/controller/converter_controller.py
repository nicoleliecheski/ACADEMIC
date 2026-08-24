from flask import render_template, request

from model.currency_model import CurrencyModel


class ConverterController:
   
    def __init__(self):
        self.model = CurrencyModel()

    def index(self):
        currencies = self.model.available_currencies()
        result = None
        error = None
        form_data = {
            "amount": "1",
            "from_currency": "USD",
            "to_currency": "BRL",
        }

        if request.method == "POST":
            form_data["amount"] = request.form.get("amount", "")
            form_data["from_currency"] = request.form.get("from_currency", "")
            form_data["to_currency"] = request.form.get("to_currency", "")

            try:
                amount = float(form_data["amount"])
                result = self.model.convert(
                    amount,
                    form_data["from_currency"],
                    form_data["to_currency"],
                )
            except ValueError as exc:
                error = str(exc)

        return render_template(
            "index.html",
            currencies=currencies,
            result=result,
            error=error,
            form_data=form_data,
        )
