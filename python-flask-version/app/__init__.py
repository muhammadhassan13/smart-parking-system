from flask import Flask
from .routes import bp

def create_app():
    app = Flask(__name__, template_folder='../templates', static_folder='../static')
    app.secret_key = 'smart-parking-system-secret-key-2026'
    app.register_blueprint(bp)
    return app