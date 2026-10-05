using System;

public class Market
{
    private bool _isRunning = true;
    private int _marketBalance = 0;
    private int _userBalance = 0;
    private Dictionary<string, (int Price, int Quantity)> _assortment = new()
    {
        ["Plush Pepe"] = (6500, 1),
        ["Heart Locket"] = (2000, 2),
        ["Durov's Cap"] = (900, 3),
        ["Heroic Helmet"] = (300, 4),
        ["Neko Helmet"] = (30, 5),
        ["Happy Brownie"] = (2, 6)
    };

    private const string AdminPassword = "01234";
    private bool _isAdminMode = false;

    public static void Main()
    {
        var market = new Market();
        market.Run();
    }

    public void Run()
    {
        Console.WriteLine("\nThe gift telegram store is up and running.");
        Console.WriteLine("Type 'help' for available commands.\n");
        while (_isRunning)
        {
            try
            {
                Console.Write("\n> ");
                string input = Console.ReadLine()?.Trim() ?? "";
                if (string.IsNullOrEmpty(input)) continue;
                ProcessCommand(input);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"\nError: {ex.Message}\n");
            }
        }
        Console.WriteLine("\nThe gift telegram store is closed.");
        Console.WriteLine("Freedom is more important than money\n");
    }

    private void ProcessCommand(string command)
    {
        string cmd = command.Split().First();
        switch (cmd)
        {
            case "quit":
                _isRunning = false;
                Console.WriteLine("\nClosing the market...\n");
                break;

            case "help":
                ShowHelp();
                break;

            case "deposit":
                DepositCoins();
                break;

            case "catalog":
                ShowCatalog();
                break;

            case "buy":
                BuyItem();
                break;

            case "backcoins":
                ReturnCoins();
                break;

            case "balance":
                ShowBalance();
                break;

            case "admin":
                EnableAdminMode();
                break;

            case "additem":
                if (CheckAdminAccess()) AddNewItem();
                break;

            case "restock":
                if (CheckAdminAccess()) RestockItem();
                break;

            case "collect":
                if (CheckAdminAccess()) CollectMoney();
                break;

            case "logout":
                LogoutAdmin();
                break;

            default:
                Console.WriteLine($"\nUnknown command: '{cmd}'. Type 'help' for available commands.\n");
                break;
        }
    }

    private void EnableAdminMode()
    {
        Console.Write("\nEnter admin password: ");
        string password = Console.ReadLine()?.Trim() ?? "";
        if (password == AdminPassword)
        {
            _isAdminMode = true;
            Console.WriteLine("\nAdministrator mode activated.");
        }
        else
        {
            Console.WriteLine("\nInvalid password.\n");
        }
    }

    private bool CheckAdminAccess()
    {
        if (!_isAdminMode)
        {
            Console.WriteLine("\nAccess denied. Use 'admin' command to enter administrator mode.\n");
            return false;
        }
        return true;
    }

    private void AddNewItem()
    {
        Console.Write("\nEnter item name: ");
        string name = Console.ReadLine()?.Trim() ?? "";
        if (string.IsNullOrEmpty(name))
        {
            Console.WriteLine("\nItem name cannot be empty.");
            return;
        }
        if (_assortment.ContainsKey(name))
        {
            Console.WriteLine($"\nItem '{name}' already exists.");
            return;
        }
        Console.Write("\nEnter item price: ");
        if (!int.TryParse(Console.ReadLine(), out int price) || price <= 0)
        {
            Console.WriteLine("\nInvalid price. Please enter a positive number.");
            return;
        }
        Console.Write("Enter initial quantity: ");
        if (!int.TryParse(Console.ReadLine(), out int quantity) || quantity < 0)
        {
            Console.WriteLine("\nInvalid quantity. Please enter a non-negative number.");
            return;
        }
        _assortment[name] = (price, quantity);
        Console.WriteLine($"\nItem '{name}' added successfully. Price: {price} coins, Quantity: {quantity}\n");
    }

    private void RestockItem()
    {
        Console.Write("\nEnter item name to restock: ");
        string name = Console.ReadLine()?.Trim() ?? "";
        if (!_assortment.ContainsKey(name))
        {
            Console.WriteLine($"\nItem '{name}' not found.");
            return;
        }
        Console.Write("\nEnter quantity to add: ");
        if (!int.TryParse(Console.ReadLine(), out int quantity) || quantity <= 0)
        {
            Console.WriteLine("\nInvalid quantity. Please enter a positive number.");
            return;
        }
        _assortment[name] = (_assortment[name].Price, _assortment[name].Quantity + quantity);
        Console.WriteLine($"\nItem '{name}' restocked. New quantity: {_assortment[name].Quantity}\n");
    }

    private void CollectMoney()
    {
        if (_marketBalance == 0)
        {
            Console.WriteLine("\nNo money to collect.");
            return;
        }
        Console.WriteLine($"\nCollected {_marketBalance} coins from the machine.\n");
        _marketBalance = 0;
    }

    private void LogoutAdmin()
    {
        if (_isAdminMode)
        {
            _isAdminMode = false;
            Console.WriteLine("\nAdministrator mode deactivated.\n");
        }
        else
        {
            Console.WriteLine("\nNot in administrator mode.\n");
        }
    }

    private void DepositCoins()
    {
        Console.WriteLine("\nDeposit coins (type 'end' to finish):");
        string coin;
        while (true)
        {
            Console.Write("\nCoin value: ");
            coin = Console.ReadLine()?.Trim() ?? "";
            if (coin == "end") break;
            if (int.TryParse(coin, out int coinValue) && coinValue > 0)
            {
                _userBalance += coinValue;
                Console.WriteLine($"\nAdded {coinValue} coins. Total: {_userBalance} coins");
            }
            else
            {
                Console.WriteLine("\nInvalid coin value. Please enter a positive number.");
            }
        }
        Console.WriteLine($"\nYour balance: {_userBalance} coins\n");
    }

    private void ShowCatalog()
    {
        Console.WriteLine("\n    AVAILABLE ITEMS \n");
        int index = 1;
        foreach (var item in _assortment)
        {
            Console.WriteLine($"{index}. {item.Key}");
            Console.WriteLine($"   Price: {item.Value.Price} coins");
            Console.WriteLine($"   Stock: {item.Value.Quantity} pcs");
            Console.WriteLine();
            index++;
        }
    }

    private void BuyItem()
    {
        ShowCatalog();
        string itemName;
        while (true)
        {
            Console.Write("\nEnter item number: ");
            string input = Console.ReadLine()?.Trim() ?? "";
            if (!int.TryParse(input, out int itemNumber))
            {
                Console.WriteLine("\nPlease enter a valid number.");
                continue;
            }
            if (itemNumber < 1 || itemNumber > _assortment.Count)
            {
                Console.WriteLine($"\nItem number {itemNumber} not found. Available: 1-{_assortment.Count}");
                continue;
            }
            itemName = _assortment.Keys.ElementAt(itemNumber - 1);
            break;
        }
        var item = _assortment[itemName];
        if (item.Quantity <= 0)
        {
            Console.WriteLine($"\n'{itemName}' is out of stock!");
            return;
        }
        if (_userBalance < item.Price)
        {
            int missing = item.Price - _userBalance;
            Console.WriteLine($"\nNot enough coins! You need {missing} more coins.");
            return;
        }

        Console.WriteLine($"\nYou selected: {itemName}");
        Console.WriteLine($"Price: {item.Price} coins");
        Console.WriteLine($"Your balance: {_userBalance} coins");
        Console.WriteLine($"Balance after purchase: {_userBalance - item.Price} coins");
        Console.Write("\nConfirm purchase? (yes/no): ");
        string answer = Console.ReadLine()?.Trim() ?? "";
        if (answer != "yes")
        {
            Console.WriteLine("Purchase cancelled.");
            return;
        }

        _assortment[itemName] = (item.Price, item.Quantity - 1);
        _marketBalance += item.Price;
        _userBalance -= item.Price;
        Console.WriteLine($"\nYou bought '{itemName}' for {item.Price} coins.");
        Console.WriteLine($"Remaining balance: {_userBalance} coins\n");

        Console.Write("\nWant to buy another item? (yes/no): ");
        answer = Console.ReadLine()?.Trim() ?? "";
        if (answer == "yes")
        {
            BuyItem();
            return;
        }
        Console.Write("\nReturn remaining coins? (yes/no): ");
        answer = Console.ReadLine()?.Trim() ?? "";
        if (answer == "yes")
        {
            ReturnCoins();
        }
    }

    private void ReturnCoins()
    {
        if (_userBalance == 0)
        {
            Console.WriteLine("\nYou have no coins to return.\n");
            return;
        }
        Console.WriteLine($"\nTake your coins: {_userBalance} coins\n");
        _userBalance = 0;
    }

    private void ShowBalance()
    {
        Console.WriteLine($"\nYour balance: {_userBalance} coins\n");
    }

    private void ShowHelp()
    {
        Console.WriteLine("\n AVAILABLE COMMANDS:\n");
        Console.WriteLine("  help      - Show this help");
        Console.WriteLine("  quit      - Exit the program");
        Console.WriteLine("  deposit   - Add coins to balance");
        Console.WriteLine("  catalog   - Show available items");
        Console.WriteLine("  buy       - Purchase an item");
        Console.WriteLine("  backcoins - Return coins");
        Console.WriteLine("  balance   - Check your balance");
        if (_isAdminMode)
        {
            Console.WriteLine("\n  ADMIN COMMANDS:");
            Console.WriteLine("  additem   - Add new item to assortment");
            Console.WriteLine("  restock   - Restock existing item");
            Console.WriteLine("  collect   - Collect money from machine");
            Console.WriteLine("  logout    - Exit administrator mode");
        }
        else
        {
            Console.WriteLine("\n  admin     - Enter administrator mode");
        }
        Console.WriteLine();
    }
}