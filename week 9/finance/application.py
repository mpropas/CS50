from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


##index only supports GET
@app.route("/", methods=["GET"])
@login_required
def index():
    ##index is a get method
    ##get lines from transaction table in db
    transactions = db.execute("SELECT * FROM transactions WHERE userId = :userId", userId=session["user_id"])

    ##setup a new transactions dict object to make rows of each symbol adding quantities for multiples
    newTransactions = {}
    value = 0
    for transaction in transactions:
        if transaction["symbol"] in newTransactions:
            ##calculate added value using existing price in newTransactions
            value += transaction["quantity"] * newTransactions[transaction["symbol"]]["price"]
            newTransactions[transaction["symbol"]]["quantity"] += transaction["quantity"]
            ##if this quantity becomes zero just delete that key
            if newTransactions[transaction["symbol"]]["quantity"] == 0:
                del newTransactions[transaction["symbol"]]
            
        else:
            ##use symbol as key and put dict within newTransactions
            newTransactions[transaction["symbol"]] = transaction
            ##get pricing using lookup helper
            newTransactions[transaction["symbol"]]["price"] = lookup(transaction["symbol"])["price"]
            ##calculate additional value using price just calulated
            value += transaction["quantity"] * newTransactions[transaction["symbol"]]["price"]
            
    
    ##get users cash from db
    cash = db.execute("SELECT cash FROM users WHERE id = :userId", userId=session["user_id"])[0]["cash"]
    net = cash + value
    
    ##send back to index
    return render_template("index.html", transactions=newTransactions, cash=usd(cash), net=usd(net))
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        ##first check that quantity can be converted to a float. if it cannot or it is 0 or less,
        ##apologize with an invalid quantity error
        try:
            quantity = float(request.form.get("quantity"))
        except ValueError:
            return apology("invalid quantity")
            
        if quantity <= 0:
            return apology("invalid quantity")
        
        ##get quote with lookup helper
        quote = lookup(request.form.get("symbol"))
        ##give apology if symbol was not valid
        if quote == None:
            return apology("invalid symbol")
        ##select cash from the account of the user that is currently logged in  
        cash = db.execute("SELECT cash FROM users WHERE id = :userId", userId=session["user_id"])[0]["cash"]
        
        ##now check to see if the user has enough money for the transaction
        newCash = cash - quote["price"] * quantity
        if newCash < 0:
            return apology("insufficient funds for transaction")
            
        ##since user input is now verified and they have sufficient funding, update portfolio
        transaction = db.execute("INSERT INTO transactions (transType, userId, symbol, price, quantity) VALUES(:transType, :userId, :symbol, :price, :quantity)",
        transType="b", userId=session["user_id"], symbol=quote["symbol"], price=quote["price"], quantity=quantity)
        
        ##and update cash
        updateCash = db.execute("UPDATE users SET cash = :cash WHERE id = :userId", cash=newCash, userId=session["user_id"])
        
        ##return to index
        return redirect(url_for("index"))
        
    else:
        ##otherwise method was just a get so simply render buy.html
        return render_template("buy.html")
        


@app.route("/history", methods=["GET"])
@login_required
def history():
    """Show history of transactions."""
    ##get the transactions table
    transactions = db.execute("SELECT * FROM transactions WHERE userId = :userId", userId=session["user_id"])
    newTransactions = []
    ##map b to purchase and s to sale
    transTypes = {
        "b": "purchase",
        "s": "sale"
    }
    for transaction in transactions:
        transaction["transType"] = transTypes[transaction["transType"]]
        ##show positive for sales
        transaction["quantity"] = abs(transaction["quantity"])
        newTransactions.append(transaction)
        
    ##reverse list to have most recent at top
    newTransactions.reverse()
    
    ##send to history.html
    return render_template("history.html", transactions=newTransactions)


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "POST":
        ##make sure amount makes sense
        try:
            amount = float(request.form.get("amount"))
        except ValueError:
            return apology("invalid amount")
        ##make sure amount is greater than 0
        if amount <= 0:
            return apology("invalid amount")
        ##get the current amount of cash
        cash = db.execute("SELECT cash FROM users WHERE id = :userId", userId=session["user_id"])[0]["cash"]
        ##update to newCash
        newCash = cash + amount
        updateCash = db.execute("UPDATE users SET cash = :cash WHERE id = :userId", cash=newCash, userId=session["user_id"])
        ##go back to index
        return redirect(url_for("index"))
        
    else:
        ##otherwise this is just a "GET" request so return addCash.html
        return render_template("addCash.html")
        
        

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))




@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    ##return render_template("quote.html")
    if request.method == "POST":
        ##get quote from lookup helper
        quote = lookup(request.form.get("symbol"))
        ##give apology if quote returns None
        if quote == None:
            return apology("invalid symbol")
        ##otherwise render pricing with info from quote object
        return render_template("quotePricing.html", name=quote["name"], symbol=quote["symbol"], price=usd(quote["price"]))
    
    else:
        ##if not post then must be get, just render quote.html in this case
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    ##return apology("TODO")

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
            
        #ensure password confirmation was submitted
        elif not request.form.get("confirmPassword"):
            return apology("must confirm password")
            
        ##print(request.form.get("username"), request.form.get("password"), request.form.get("confirmPassword"))
        ##ensure that two passwords are the same
        elif request.form.get("password") != request.form.get("confirmPassword"):
            return apology("invalid password")
            
        ##insert into database
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :passwordHash)",
        username=request.form.get("username"), passwordHash=pwd_context.encrypt(request.form.get("password")))
        
        ##if db execute fails apologize
        if not result:
            return apology("registration failed")
            
        ##login user automatically
        session["user_id"] = result
        
        return redirect(url_for("index"))


    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        ##first check that quantity can be converted to a float. if it cannot or it is 0 or less,
        ##apologize with an invalid quantity error
        symbol = request.form.get("symbol")
        try:
            quantity = float(request.form.get("quantity"))
        except ValueError:
            return apology("invalid quantity")
            
        if quantity <= 0:
            return apology("invalid quantity")
        
        ##get quote with lookup helper
        quote = lookup(symbol)
        ##give apology if symbol was not valid
        if quote == None:
            return apology("invalid symbol")
        ##select cash from the account of the user that is currently logged in  
        cash = db.execute("SELECT cash FROM users WHERE id = :userId", userId=session["user_id"])[0]["cash"]
        
        ##get the total amount of shares of symbol
        transactions = db.execute("SELECT * FROM transactions WHERE userId = :userId AND symbol = :symbol",
        userId=session["user_id"], symbol=symbol)
        ##set userQuantity to 0
        userQuantity = 0
        for transaction in transactions:
            userQuantity += transaction["quantity"]
        
        ##now check to see if the user has enough stocks to sell
        if userQuantity < quantity:
            return apology("insufficient stocks for transaction")
            
        ##since user input is now verified and they have sufficient stocks, update portfolio
        negQuantity = -1 * quantity
        transaction = db.execute("INSERT INTO transactions (transType, userId, symbol, price, quantity) VALUES(:transType, :userId, :symbol, :price, :quantity)",
        transType="s", userId=session["user_id"], symbol=symbol, price=quote["price"], quantity=negQuantity)
        
        ##and update cash
        newCash = cash + quantity * quote["price"]
        updateCash = db.execute("UPDATE users SET cash = :cash WHERE id = :userId", cash=newCash, userId=session["user_id"])
        
        ##return to index
        return redirect(url_for("index"))
        
    else:
        ##otherwise method was just a get so simply render buy.html
        return render_template("sell.html")
