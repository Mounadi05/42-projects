#!/usr/bin/python3
import sys,re

def reduce_polynomial(polynomial):
    terms = []
    first = 0
    for power in sorted(polynomial.keys()):
        coeff = polynomial[power]
        sign = ''
        if coeff >= 0 and first:
             sign = '+ ' if terms else ''
        terms.append(f'{sign}{coeff} * X^{power}')
        first = 1
    reduced_str = 'Reduced form: ' + ' '.join(terms) + ' = 0'
    return reduced_str

def parse_term(term):
    term = term.replace(' ', '')
    match = re.match(r'([+-]?\s*\d+(?:\.\d+)?)(?:\s*\*\s*[xX]\s*\^\s*(\d+))?', term)
    if match:
        coeff = float(match.group(1))
        power = int(match.group(2)) if match.group(2) else 0
        return coeff, power
    else:
        raise ValueError(f"Invalid term: {term}")

def parse_input(equation):
    ls, rs = equation.split('=')
    term_pattern = r'([+-]?\s*\d+(?:\.\d+)?(?:\s*\*\s*[Xx]\s*\^\s*\d+)?)'
    ls_terms = re.findall(term_pattern, ls)
    rs_terms = re.findall(term_pattern, rs)
    polynomial = {}
    for term in ls_terms:
        coeff, power = parse_term(term)        
        polynomial[power] = polynomial.get(power, 0) + coeff
    for term in rs_terms:
        coeff, power = parse_term(term)
        polynomial[power] = polynomial.get(power, 0) - coeff
    return polynomial
  
def calculate_degree(polynomial):
    if not polynomial:
        return 0
    degree = max(polynomial.keys())
    return degree
    
def get_coefficient(polynomial):
    a = polynomial.get(2, 0)
    b = polynomial.get(1, 0)
    c = polynomial.get(0, 0)
    return a, b, c

def solve_degree_0(polynomial):
    c = polynomial.get(0, 1)
    if c == 0:
        print("All real numbers are solutions.")
    else:
        print("No solution.")

def solve_degree_1(polynomial):
    a, b, c = get_coefficient(polynomial)
    x = -c / b
    print("The solution is:")
    print(x)
    
def solve_degree_2(polynomial):
    a, b, c = get_coefficient(polynomial)
    delta = b ** 2 - 4 * a * c
    if delta > 0:
        x1 = (-b + delta ** 0.5) / (2 * a)
        x2 = (-b - delta ** 0.5) / (2 * a)
        print("Discriminant is strictly positive, the two solutions are:")
        print(x1)
        print(x2)
    if delta == 0:
        x = -b / (2 * a)
        print("Discriminant is zero, the solution is:")
        print(x)
    if delta < 0:
        x1r = -b / (2 * a)
        x1i = ((-delta) ** 0.5) / (2 * a)
        x2r = x1r
        x2i = -x1i
        print("Discriminant is strictly negative, the two complex solutions are:")
        print(f"{x1r} + {x1i}i")
        print(f"{x2r} - {abs(x2i)}i")



def main():
    if len(sys.argv) != 2:
        print("Usage: ./computor.py 'equation'")
        sys.exit(1)
    else:
        equation = sys.argv[1]
        try:
            polynomial = parse_input(equation)
            reduced_str = reduce_polynomial(polynomial)
            print(reduced_str)
            degree = calculate_degree(polynomial)
            print("Polynomial degree:", degree)
            if degree > 2:
                print("The polynomial degree is stricly greater than 2, I can't solve.")
            elif degree > 1:
                solve_degree_2(polynomial)
            elif degree > 0:
                solve_degree_1(polynomial)
            else:
                solve_degree_0(polynomial)

        except Exception as e:
            print(e)
            print("Invalid equation format.")
       
if __name__ == "__main__":
    main()