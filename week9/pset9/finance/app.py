import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    portfolio = db.execute("SELECT symbol, name, shares FROM inventory WHERE user_id = ?", session["user_id"])
    length = len(portfolio)
    grand_total = 0

    # # get actual price
    for i in range(len(portfolio)):
        result = lookup(portfolio[i]["symbol"])
        portfolio[i]["price"] = usd(result["price"])
        portfolio[i]["total"] = result["price"] * portfolio[i]["shares"]
        grand_total += portfolio[i]["total"]

        portfolio[i]["total"] = usd(portfolio[i]["total"])

    grand_total = usd(grand_total)

    # get current cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = usd(cash[0]["cash"])

    return render_template("index.html", portfolio=portfolio, length=length, grand_total=grand_total, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # initialize variables
        query_symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not query_symbol or not shares:
            return apology("Symbol or shares is blank")

        # sanitize input
        if not query_symbol.isalpha():
            return apology("Only A-Z charaters allowed")
        elif not shares.isdigit():
            return apology("Only integer allowed in shares")

        # lookup the symbol
        result = lookup(query_symbol)

        # Ensure symbol exists
        if result == None:
            return apology("Invalid Symbol")

        # get user cash amount
        userdata = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        user_cash = userdata[0]["cash"]

        # check whether user has enough cash
        transaction_amount = round(float(result["price"]), 2) * int(shares)
        if user_cash - transaction_amount < 0:
            return apology("Not enough cash")

        # record transaction to database
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)", session[
            "user_id"], result["symbol"], int(shares), round(result["price"], 2))

        # subtract the user cash
        remaining_cash = user_cash - transaction_amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, session["user_id"])

        # check if shares already in inventory
        status = db.execute("SELECT * FROM inventory WHERE user_id = ? AND symbol = ?", session["user_id"], result["symbol"])

        # update inventory
        if len(status) == 0:
            db.execute("INSERT INTO inventory VALUES (?, ?, ?, ?)", session["user_id"], result["symbol"], result["name"], shares)
        else:
            db.execute("UPDATE inventory SET shares = ? WHERE user_id = ? AND symbol = ?", int(status[
                0]["shares"]) + int(shares), session["user_id"], result["symbol"])

        flash(f"Successfully bought {shares} share(s) of {result['symbol']}", "info")

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get transactions data
    history = db.execute("SELECT symbol, shares, price, time FROM transactions WHERE user_id = ?", session["user_id"])
    length = len(history)

    # Format the price
    for i in range(length):
        history[i]["price"] = usd(history[i]["price"])

    return render_template("history.html", history=history, length=length)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
    if request.method == "POST":
        query_symbol = request.form.get("symbol")

        if not query_symbol:
            return apology("Quote cannot be blank")

        if not query_symbol.isalpha():
            return apology("Only A-Z charaters allowed")

        result = lookup(query_symbol)
        if result == None:
            return apology("Invalid Symbol")

        price_usd = usd(result["price"])
        return render_template("quoted.html", result=result, price=price_usd)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Initialize variables
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username")

        # Ensure password was submitted
        if not password:
            return apology("must provide password")

        # Ensure password match confirmation
        if not password == confirmation:
            return apology("password confirmation does not match")

        # Check if username already exist
        check_username = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(check_username) != 0:
            return apology("username already taken")

        # Hash password
        hashed_password = generate_password_hash(password)

        # Insert new user to database
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hashed_password)

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Initiate variables
        request_symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure user input positive integer for shares
        if not shares.isdigit() or int(shares) <= 0:
            return apology("Share must be positive integer")

        # Verify user request
        verify = db.execute("SELECT symbol, shares FROM inventory WHERE user_id = ? AND symbol = ?", session[
            "user_id"], request_symbol)

        if len(verify) != 1:
            return apology("Stock symbol does not exist")

        get_symbol_data = lookup(verify[0]["symbol"])
        current_price = round(get_symbol_data["price"], 2)

        # If requested shares is larger than inventory
        if int(shares) > int(verify[0]["shares"]):
            return apology("You don't have that many shares")

        # Delete row if deleted shares equals amount in inventory
        elif int(shares) == int(verify[0]["shares"]):
            db.execute("DELETE FROM inventory WHERE user_id = ? AND symbol = ?", session[
                "user_id"], request_symbol)

        else:
            # Get new amount
            new_amount = int(verify[0]["shares"]) - int(shares)
            # Update new amount
            db.execute("UPDATE inventory SET shares = ? WHERE user_id = ? AND symbol = ?", new_amount, session[
                "user_id"], request_symbol)

        # Record transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)", session[
            "user_id"], request_symbol, -1 * int(shares), current_price)

        # Update user cash
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        acquired_cash = int(shares) * current_price

        new_cash = float(current_cash[0]["cash"]) + acquired_cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

        # Flash message
        flash(f"Successfully sold {shares} share(s) of {request_symbol}")

        return redirect("/")

    else:
        symbols = db.execute("SELECT symbol FROM inventory WHERE user_id = ?", session["user_id"])
        length = len(symbols)
        return render_template("sell.html", symbols=symbols, length=length)
        