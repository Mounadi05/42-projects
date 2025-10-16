```markdown
# ComputorV1

ComputorV1 is a Python script that solves polynomial equations up to the second degree. It parses the input equation, reduces it to its simplest form, and then solves it based on its degree.

## Features

- Parses polynomial equations.
- Reduces the polynomial to its simplest form.
- Solves polynomial equations of degree 0, 1, and 2.
- Provides solutions for real and complex roots.

## Usage

To use ComputorV1, run the script with a polynomial equation as an argument:

./computor.py 'equation'
```

### Examples

#### Example 1: Degree 2 Equation

```sh
./computor.py '5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0'
```

Output:
```
Reduced form: 4.0 * X^0 + 4.0 * X^1 - 9.3 * X^2 = 0
Polynomial degree: 2
Discriminant is strictly positive, the two solutions are:
0.905239
-0.475131
```

#### Example 2: Degree 1 Equation

```sh
./computor.py '2 * X^0 + 4 * X^1 = 0'
```

Output:
```
Reduced form: 2.0 * X^0 + 4.0 * X^1 = 0
Polynomial degree: 1
The solution is:
-0.5
```

#### Example 3: Degree 0 Equation

```sh
./computor.py '42 = 42'
```

Output:
```
Reduced form: 0.0 * X^0 = 0
Polynomial degree: 0
All real numbers are solutions.
```

## Requirements

- Python 3.x

## Installation

Clone the repository and navigate to the project directory:

```sh
git clone https://github.com/Mounadi05/42-ComputorV1.git
cd ComputorV1
```
