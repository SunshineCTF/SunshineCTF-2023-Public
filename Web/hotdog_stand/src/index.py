from flask import (Flask, flash, redirect, render_template,
                   render_template_string, request, send_from_directory,
                   session, url_for)

app = Flask(__name__)
app.secret_key = 'super_secret_key123123dfasdfasdkhasdfljka'

USERNAME = 'hotdogstand'
PASSWORD = 'slicedpicklesandonions'

@app.route('/')
def index():
    if 'logged_in' in session and session['logged_in']:
       return render_template('index.html')

    return redirect(url_for('login'))

@app.route('/hotdog-database/')
def secret_interface():
    return send_from_directory(directory='hotdog-database', filename='robot_data.db')


@app.route('/login', methods=['GET', 'POST'])
def login():
    error = None
    if request.method == 'POST':
        if request.form['username'] == USERNAME and request.form['password'] == PASSWORD:
            session['logged_in'] = True
            return redirect(url_for('index'))
        else:
            error = 'Invalid credentials. Please try again.'

    return render_template("home.html")

@app.route('/logout')
def logout():
    session.pop('logged_in', None)
    flash('You were logged out')
    return redirect(url_for('index'))

@app.route('/robots.txt')
def robots():

    return render_template("robots.txt")

def create_app():
    return app

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)
