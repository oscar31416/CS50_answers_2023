import os

# from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# (SUGGESTED BY THE STUDENT)
# Can't we use Python's built-in 'sqlite3' package?
import sqlite3

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
# db = SQL("sqlite:///birthdays.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # DONE - Add the user's entry into the database
        # Step 1 - Fetch the content of the form.
        new_name = request.form.get("name", None)
        new_day = request.form.get("day", None)
        new_month = request.form.get("month", None)

        # Step 2 - Check the content of the form.
        # - Cancel the process unless both the day and the month are integer numbers.
        try:
            new_day = int(new_day)
            new_month = int(new_month)
        except:
            return redirect("/")

        if not ((type(new_day) is int) and (type(new_month) is int)):
            return redirect("/")

        # - Cancel the process unless a name has been typed.
        if new_name == None:
            return redirect("/")

        # - Verify that the user hasn't written a date that doesn't exist
        #   (for example, 30th of February).
        if (new_day < 1) or (new_day > 31) or (new_month < 1) or (new_month > 12):
            return redirect("/")

        if (new_month in [4, 6, 9, 11]) and (new_day == 31):
            return redirect("/")

        if (new_month == 2) and (new_day == 30):
            return redirect("/")

        # Step 3 - If the form passes all checks, we can update the database.
        db = sqlite3.connect("birthdays.db")
        db_cur = db.cursor()
        add = "INSERT INTO birthdays (name,month,day) VALUES(\'" + str(new_name) + "\',\'" + str(new_month) + "\',\'" + str(new_day) + "\');"
        try:
            db_cur.execute(add)
            db.commit()
            db.close()
        except:
            pass
        del db_cur
        del db
        del new_name
        del new_month
        del new_day
        return redirect("/")

    else:

        # DONE - Display the entries in the database on index.html
        db = sqlite3.connect("birthdays.db")
        db_cur = db.cursor()
        items = db_cur.execute("SELECT name,month,day FROM birthdays;")
        items = list(items)
        db.commit()
        db.close()
        return render_template("index.html", item_list=items)
        del items
        del db_cur
        del db
