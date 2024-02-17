
beløp = 30_000
rente = 0.05

ønsket_avkastning = 2000

def f(x):
    return beløp * (1+rente)**x


nøyaktighet = 0.000001
def deriver(fn, x):
    return (fn(x + nøyaktighet) - fn(x)) / nøyaktighet

steg = 100
steg_faktor = 1/2

def approximate(f, iterations, dx, ddx, mx, debug=False):
    x = 0

    for i in range(iterations):
        dx *= ddx
        while deriver(f, x+dx) <= ønsket_avkastning:
            x += dx
        if debug: print(f"{i+1} iterations, ∆y={deriver(f, x)}")
    
    return x

if __name__ == "__main__":
    for i in range(1, 32):

        x = approximate(f, i+1, steg, steg_faktor, ønsket_avkastning)

        print(f"-- Resultater ved {i+1}x tilnærming --")
        print("x:", x)
        print("y:", f(x))
        print("∆y:", deriver(f, x))
