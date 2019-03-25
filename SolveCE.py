
def subpair(a, b, q):
    if len(a) != 2 or len(b) != 2:
        return
    else:
        c = [0, 0]
        c[0], c[1] = a[0] - q * b[0], a[1] - q * b[1]
        return c

def gcd(a, b):
    M, m = max(a, b), min(a, b)
    p = M
    q, r = 0, 0
    s = [[1, 0], [0, 1]]
    i = 0
    while True:
        q = int(M / m)
        r = M % m
        if r == 0:
            break
        else:
            M, m = m, r
            s.append(subpair(s[i], s[i + 1], q))
            i = i + 1
    if p == a:
        s_a, s_b = s[len(s) - 1][0], s[len(s) - 1][1]
    else:
        s_a, s_b = s[len(s) - 1][1], s[len(s) - 1][0]
    return m, (s_a, s_b)

def solveCE(a, c, m):
    g, (s_0, t_0) = gcd(a, m)
    if c % g != 0:
        print("NO solution!")
        return
    else:
        x = int(s_0 * c / g)
        t = int(m / g)
        while x < 0:
            x += t
        while x > t:
            x -= t
        s = []
        while x < m:
            s.append(x)
            x += t
        print(s)
        return

solveCE(4183, 5781, 15087)
