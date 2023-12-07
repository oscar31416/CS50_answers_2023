import datetime
import os

#from cs50 import SQL
import sqlite3
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # 1 - Fetch user's ID.
    uid = session["user_id"]

    # 2 - Open the data base.
    db = sqlite3.connect("finance.db")
    db_c = db.cursor()

    # 3 - Get data about transactions.
    command_get_transactions = "SELECT symbol, SUM(shares) FROM transactions WHERE user_id = " + str(uid) + " GROUP BY symbol"
    transactions_iterator = db_c.execute(command_get_transactions)
    db.commit()
    transactions = []
    value_sum = 0.0
    for row in transactions_iterator:
        new_row = {}
        if row[1] == 0:
            continue
        new_row["symbol"] = row[0]
        new_row["shares"] = row[1]
        temp_symbol = lookup(str(row[0]).upper())
        new_row["price"] = temp_symbol["price"]
        new_row["value"] = temp_symbol["price"] * row[1]
        value_sum += new_row["value"]
        transactions.append(new_row)

    # 4 - Get user's current quantity of money.
    command_get_money = "SELECT cash FROM users WHERE id = " + str(uid)
    money_iterator = db_c.execute(command_get_money)
    db.commit()
    money_list = []
    for row in money_iterator:
        new_row = {}
        new_row["cash"] = row[0]
        money_list.append(new_row)
    money = money_list[0]["cash"] + value_sum

    # 5 - Close the connection to the database.
    # Display the data.
    db.close()
    del db_c
    del db
    return render_template("index.html", data=transactions, money=money)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Show template 'buy.html' if GET.
    if request.method == "GET":
        return render_template("buy.html")

    # If it isn't the case...
    else:

        # 1 - Get the symbol and the number of shares.
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # 2 - Check the content of the form.
        # > There's a symbol.
        # > There's a string in 'shares'.
        # > 'shares' is a positive integer number.

        # 2.1 - Symbol.
        if not symbol:
            return apology("A symbol is required")

        # 2.2 - Shares.
        if not shares:
            return apology("A number of shares must be given")

        # 2.3 - Positive number of shares.
        try:
            shares = int(shares)
            if shares <= 0:
                raise Exception
        except:
            return apology("The number of shares must be a positive integer")

        # 3 - Check that the symbol exists.
        data = lookup(symbol.upper())
        if data is None:
            return apology("Symbol not found")

        # 4 - Check whether the user has got enough money.
        # Fetch user's current money and compare to transaction's value.
        target_value = shares * data["price"]
        uid = session["user_id"]

        db = sqlite3.connect("finance.db")
        db_c = db.cursor()
        command_get_data = "SELECT cash FROM users WHERE id = " + str(uid)
        user_data_iterator = db_c.execute(command_get_data)
        db.commit()
        user_data = []

        for row in user_data_iterator:
            new_row = {}
            new_row["cash"] = row[0]
            user_data.append(new_row)

        del new_row
        current_money = user_data[0]["cash"]

        if current_money < target_value:
            db.close()
            return apology("Not enough money for transaction")

        # 5 - If there's enough money, update the user's data.
        money_update = current_money - target_value
        command_update_money = "UPDATE users SET cash = (?) WHERE id = (?)"
        db_c.execute(command_update_money, (money_update, uid))
        db.commit()

        # 6 - Save the transaction to a table.
        current_datetime = datetime.datetime.now()
        command_save_transaction = "INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES(?,?,?,?,?)"
        db_c.execute(command_save_transaction, (uid, symbol, shares, data["price"], current_datetime))
        db.commit()

        # 7 - Close the connection to the data base, delete all unnecessary variables and return to home page.
        db.close()
        del db_c
        del db
        return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # 1 - Get user's ID.
    uid = session["user_id"]

    # 2 - Open the data base.
    db = sqlite3.connect("finance.db")
    db_c = db.cursor()

    # 3 - Fetch data from the table called 'transactions'.
    command_get_transactions = "SELECT symbol,shares,price,date FROM transactions WHERE user_id = " + str(uid)
    transactions_iterator = db_c.execute(command_get_transactions)
    db.commit()
    transactions_list = []
    for row in transactions_iterator:
        new_row = {}
        new_row["symbol"] = row[0]
        new_row["shares"] = row[1]
        new_row["price"] = row[2]
        new_row["date"] = row[3]
        transactions_list.append(new_row)

    # 4 - Render the 'history.html' template.
    db.close()
    del db_c
    del db
    return render_template("history.html", data=transactions_list)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("Username required")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("Password required")

        # Query database for username
        db = sqlite3.connect("finance.db")
        db_cur = db.cursor()
        command_get_username = "SELECT * FROM users WHERE username = \'" + request.form.get("username") + "\'"
        rows_iterator = db_cur.execute(command_get_username)
        db.commit()
        rows = []
        for row in rows_iterator:
            new_row = {}
            new_row["id"] = row[0]
            new_row["username"] = row[1]
            new_row["hash"] = row[2]
            new_row["cash"] = row[3]
            rows.append(new_row)
        db.close()
        del db_cur
        del db

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid username and/or password")

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # If a symbol has been typed...
    if request.method == "POST":

        # Try to fetch data.
        data = lookup(str(request.form.get("symbol")).upper())
        if data is None:
            return apology("Error while trying to find symbol. Try again.")

        # If fetched, display.
        return render_template("quoted.html", name=data["name"], price=data["price"], symbol=data["symbol"])

    # If the 'quote' page has been reached via GET,
    # ask the user to type a symbol.
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Close a session if open.
    try:
        session.clear()
    except:
        pass

    # If the user reaches the route via 'POST'...
    if request.method == "POST":

        # 1 - Check the very basics of the input. The form MUST:
        # > Include something in the three entries.
        # > Include a username that is NOT present in the database.
        # > Have identical password entries.
        # PERSONAL TOUCH - Force the passwords to include letters AND numbers.

        # 1.1 - Include something in all three entries.
        if not request.form.get("username"):
            return apology("Username required")
        if not request.form.get("password"):
            return apology("Password required")
        if not request.form.get("confirmation"):
            return apology("Password confirmation required")

        # 1.2 - Include a username that is NOT present in the database.
        db = sqlite3.connect("finance.db")
        db_cur = db.cursor()
        command_get_username = "SELECT * FROM users WHERE username = \'" + request.form.get("username") + "\'"
        rows_iterator = db_cur.execute(command_get_username)
        db.commit()
        rows = []
        for row in rows_iterator:
            new_row = {}
            new_row["id"] = row[0]
            new_row["username"] = row[1]
            new_row["hash"] = row[2]
            new_row["cash"] = row[3]
            rows.append(new_row)

        if len(rows) != 0:
            db.close()
            del db_cur
            del db
            return apology("Username already exists")

        # 1.3 - Identical password entries.
        if request.form.get("password") != request.form.get("confirmation"):
            db.close()
            del db_cur
            del db
            return apology("The passwords do not match")

        # PERSONAL TOUCH - The new password MUST have letters AND numbers BUT NOT symbols.
        potential_pw = request.form.get("password")
        if not (potential_pw.isalnum() and (not potential_pw.isalpha()) and (not potential_pw.isnumeric())):
            db.close()
            del db_cur
            del db
            return apology("The password must contain both letters and numbers, and no symbols", 403)

        # 2 - Register the new user.
        # > Make sure the username is a string.
        # > Hash the password.
        # > Save the new user.

        # 2.1 - Force the conversion of the username into a string.
        un = str(request.form.get("username"))

        # 2.2 - Hash the password.
        pw_hash = generate_password_hash(request.form.get("password"))

        # 2.3 - Save the new user.
        command_add_user = "INSERT INTO users (username,hash) VALUES(?, ?)"
        try:
            db_cur.execute(command_add_user, (un, pw_hash))
            db.commit()
        except:
            return apology("That username already exists")

        # 3 - Close the connection to the database.
        db.close()
        del db_cur
        del db

        # 4 - Return to home page.
        return redirect("/")

    # Otherwise...
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    uid = session["user_id"]

    # Show template 'sell.html' if GET.
    if request.method == "GET":

        # 1 - Open data base.
        db = sqlite3.connect("finance.db")
        db_c = db.cursor()

        # 2 - Get available symbols.
        command_get_symbols = "SELECT symbol FROM transactions WHERE user_id = " + str(uid) + " GROUP BY symbol HAVING SUM(shares) > 0"
        symbols_iterator = db_c.execute(command_get_symbols)
        db.commit()
        symbols_list = []
        for row in symbols_iterator:
            symbols_list.append(row[0])
        db.close()
        del db_c
        del db
        # 3 - Return the 'sell.html' template.
        return render_template("sell.html", symbols=symbols_list)

    # If it isn't the case...
    else:

        # 1 - Get the symbol and the number of shares.
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # 2 - Check the content of the form.
        # > There's a symbol.
        # > There's a string in 'shares'.
        # > 'shares' is a positive integer number.

        # 2.1 - Symbol.
        if not symbol:
            return apology("A symbol is required")

        # 2.2 - Shares.
        if not shares:
            return apology("A number of shares must be given")

        # 2.3 - Positive number of shares.
        try:
            shares = int(shares)
            if shares <= 0:
                raise Exception
        except:
            return apology("The number of shares must be a positive integer")

        # 3 - Check that the symbol exists.
        data = lookup(symbol.upper())
        if data is None:
            return apology("Symbol not found")

        # 4 - Check whether the user has got enough money.
        # Fetch user's current money and compare to transaction's value.
        sell_value = shares * data["price"]

        db = sqlite3.connect("finance.db")
        db_c = db.cursor()
        command_get_data = "SELECT cash FROM users WHERE id = " + str(uid)
        user_data_iterator = db_c.execute(command_get_data)
        db.commit()
        user_data = []

        for row in user_data_iterator:
            new_row = {}
            new_row["cash"] = row[0]
            user_data.append(new_row)

        current_money = user_data[0]["cash"]

        command_get_shares = "SELECT shares FROM transactions WHERE user_id = " + str(uid) + " AND symbol = \'" + str(symbol) + "\'"
        user_shares_iterator = db_c.execute(command_get_shares)
        user_shares_list = []
        for row in user_shares_iterator:
            new_row = {}
            new_row["shares"] = row[0]
            user_shares_list.append(new_row)
        user_shares = user_shares_list[0]["shares"]

        if shares > user_shares:
            db.close()
            return apology("Not enough shares for transaction")

        # 5 - If there are enough shares, update the user's data.
        money_update = current_money + sell_value
        command_update_money = "UPDATE users SET cash = ? WHERE id = ?"
        db_c.execute(command_update_money, (money_update, uid))
        db.commit()

        # 6 - Save the transaction to a table.
        shares = (-1) * shares
        current_datetime = datetime.datetime.now()
        command_save_transaction = "INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)"
        db_c.execute(command_save_transaction, (uid, symbol, shares, data["price"], current_datetime))
        db.commit()

        # 7 - Close the data base and return to home page.
        db.close()
        del db_c
        del db
        return redirect("/")
