


## ✅ Requirements

Ensure you have:

* **GCC**
* **Flex**
* **Bison**

To install on Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential flex bison
```

---

## 🔨 Build Instructions

Run the following command in the project directory (where `Makefile` is):

```bash
make
```

This will:

* Generate `lex.yy.c` and `parser.tab.c/h`
* Compile all `.c` files
* Produce an executable (likely named `json2relcsv` or `main`)

---

## 🚀 Run Instructions

Assuming your output executable is `main`, run:

```bash
./main test.json
```

Or if your executable is named `json2relcsv`:

```bash
./json2relcsv test.json
```

If the program supports printing the AST (check `main.c`), you can use:

```bash
./main --print-ast test.json
```

Output CSV files will be generated in the current directory.

---

## 🧪 Test Input

Use any JSON file like:

```bash
./main test/sample1.json
```

---

## 🛑 Troubleshooting

* **Command not found**: Ensure `make` and `./main` are run from the correct directory.
* **Flex or Bison errors**: Confirm installation with `flex --version` and `bison --version`.
* **Segfaults**: Check your JSON format or memory handling in `main.c`.
