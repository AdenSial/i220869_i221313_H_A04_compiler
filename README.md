
# JSON to Relational CSV Converter

This project converts structured JSON data into a set of normalized CSV files using a compiler-based approach. It utilizes **Flex** and **Bison** to parse JSON, analyzes its hierarchical structure via an **Abstract Syntax Tree (AST)**, and outputs relational-style CSV files. The tool supports complex nested structures and maintains proper **foreign key relationships** between tables.

---

##  Key Features

*  **Full JSON Support**: Handles JSON strings, numbers, booleans, nulls, objects, and arrays
*  **Relational Mapping (R1–R6)**:

  * **R1**: Converts JSON objects into flat rows
  * **R2**: Extracts arrays of objects into child tables
  * **R3**: Maps arrays of scalar values into junction tables
  * **R4**: Scalar values become columns in tables
  * **R5**: Every row gets a unique ID with proper foreign key links
  * **R6**: Each table is written as an individual CSV file

---

##  Build Instructions

Ensure the following are installed on your system:

* GCC (GNU Compiler Collection)
* Flex (Lexical analyzer generator)
* Bison (Parser generator)

Then, compile the project using:

```bash
make
```

---

## Run Instructions

1. Create or place your JSON input in a file (e.g., `input.json`)
2. Run the tool:

```bash
./json2relcsv < input.json
```

3. Output CSV files will be generated in the `output/` directory.

---

##  Design Overview

###  Lexical Analysis (`scanner.l`)

* Tokenizes JSON using Flex.
* Recognizes punctuation, keywords (`true`, `false`, `null`), strings, numbers, and ignores whitespace.

###  Parsing & AST Construction (`parser.y`)

* Parses JSON and builds an **Abstract Syntax Tree (AST)**.
* Supports nested structures: objects, arrays, pairs, values.

###  AST Structure (`ast.h` / `ast.c`)

* Core data structure used for analysis.
* Nodes can be:

  * Object
  * Array
  * String
  * Number
  * Boolean
  * Null

###  Semantic Analysis & CSV Generation (`csv_converter.c`)

* Walks the AST to extract relational schemas.
* Identifies repeated object structures (as tables).
* Maintains unique row IDs and generates foreign key fields.
* Writes structured data into separate CSV files.

---

##  Output

* Each inferred table is saved as a `.csv` file in the `output/` directory.
* Includes column headers and consistent row formatting.
* Table names are inferred from object keys or parent relations.

---

##  Authors

* **22i-0869**
* **22i-1313**

