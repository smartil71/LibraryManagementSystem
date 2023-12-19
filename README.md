# Library Management System

Libraries curate and store potentially thousands of books. This program is designed to
manage an arbitrary amount of books with titles, isbns, and the current status of the book
with a simple UI to manage the library's books.

## Description

The program tracks three items for the library management system: the book title, the 13-digit
ISBN number, and the checkout status of the library entry. The program utilizes vectors, file IO, 
and string manipulation to function properly.
The library mangement system allows a multitude of funcionalities:
* Add items to the library
* Clear the library of all entries
* Display all library entries
* List invalid library entries
* Load a library from a file
* Output a library to a file
* Print out checkout status of the library
* Remove an entry from the library
* Search for a library entry

## Getting Started

### Dependencies

* Windows 10/11
* Any IDE utilizing C++20 or newer
* Recommend using VSCode or similar
* WSL
* C/C++ Themes

### Installing

* Extract to IDE of choice, ensure all files are exported
* OPTIONAL - Manually import external csv files of the same format (title, isbn, status) to file directory

### Executing program

* Before running, ensure to compile the program with the included makefile, executing
```make build```
* To run the Library Management Program
```make run```
* The user will be met with the library management system's user interface
* Enter the designated letter for the desired function

## Authors

Sean Martil email@ smart71@uic.edu

## Acknowledgments

### Citations & Resources:
* Julie Zelenski - Stanford University
* Joe Hummel, PhD - Northwestern University
* Shannon Reckinger, PhD - University of Illinois Chicago

### Copyright:
* Copyright 2023 Adam T Koehler, PhD - University of Illinois Chicago