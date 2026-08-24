from flask import Flask

from controller.converter_controller import ConverterController

app = Flask(__name__)
controller = ConverterController()

app.add_url_rule("/", view_func=controller.index, methods=["GET", "POST"])

if __name__ == "__main__":
    app.run(debug=True)
