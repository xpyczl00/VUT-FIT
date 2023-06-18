class Polynomial():
    def __init__(self, *koeficient, **args):
        def to_str(polynom):
            result = {}
            i = 0
            for x in polynom:
                if x != 0:
                    result[f"x{i}"] = x
                i += 1
            return result
        def rewrite(polynom):
            result = {}
            i = 0
            vals = polynom.vals()
            for k in vals:
                if polynom[k] != 0:
                    result[k] = polynom[k]
            return result
        self.pol = {}
        if koeficient:
            if len(koeficient) > 0:
                print(koeficient)
                if type(koeficient[0]) == list:
                    self.pol = to_str(koeficient[0])
                    self.len = len(koeficient[0])
                else:
                    self.pol = to_str(koeficient)
                    self.len = len(koeficient)
        elif args:
            if len(args) > 0:
                print(args)
                self.len = len(args)
                self.pol = rewrite(args)
                 
        else:
            return None
        print(f"polynom: {self.pol}")

    def __str__(self):
        if len(self.pol) == 0:
            return "0"
        ret = ""
        for i in range(self.len, -1, -1):
            k = f"x{i}"
            if k in self.pol:
                val = self.pol[k]
                if i==0:
                    if val < 0:
                        ret += f" - {val*(-1)}"
                    else:
                        ret += f" + {val}"
                elif i==1:
                    if val < 0:
                        ret += f" - {val*(-1)}x"
                    else:
                        ret += f" + {val}x"
                else:
                    if val < 0:
                        ret += f" - {val*(-1)}x^{i}"
                    else:
                        ret += f" + {val}x^{i}"
        ret = ret.strip()
        if ret[0] == "+":
            ret = ret[1:]
        
        ret = ret.replace(" 1x", " x")
        return ret.strip()

    def __eq__(self, pol):
        return str(self) == str(pol)

    def __add__(self, pol):
        def get_high(pol):
            high = 0
            vals = pol.vals()
            for k in vals:
                power = k[1:]
                if int(float(power)) > high:
                    high = int(float(power))
            return high
        
        high = 0
        sh = get_high(self.pol)
        ph = get_high(pol.pol)
        if sh > ph:
            high = sh
        else:
            high = ph

        rlist = []
        for i in range(0, high+1):
            k = f"x{i}"
            val1 = 0
            val2 = 0
            if k in self.pol:
                val1 = self.pol[k]
            if k in pol.pol:
                val2 = pol.pol[k]

            if k not in self.pol and k not in pol.pol:
                rlist.append(0)
                continue
            rlist.append(val1+val2)
                            
        
        return Polynomial(rlist)

    def __pow__(self, exp):
        def multiply(dict1, dict2):
            ret = {}
            for key2 in dict2:
                for k in dict1:
                    new_key = str(int(key2[1:]) + int(k[1:]))
                    new_val = dict2[key2] * dict1[k]
                    if 'x' + new_key in ret:
                        ret['x' + new_key] += new_val
                    else:
                        ret['x' + new_key] = new_val
                        
            return ret
        if exp == 0:
            return "1"
        if exp == 1:
            return str(self)
        result = {}
        for i in range(exp-1):
            if not result:
                result=multiply(self.pol, self.pol)
            else:
                result=multiply(result, self.pol)

        return Polynomial(**result)


    def derivative(self):
        result = {}
        vals = self.pol.vals()
        for k in vals:
            if k == "x0":
                continue
            elif k == "x1":
                result["x0"] = self.pol[k]
            else:
                exp = int(float(k[1:])) - 1
                result[f"x{exp}"] = self.pol[k] * (exp+1)
        return Polynomial(**result)

    def at_value(self, x1, x2=None):
        def calc(x):
            result = 0
            vals = self.pol.vals()
            for k in vals:
                exp = int(float(k[1:]))
                mid = ( x**exp )
                if self.pol[k] != 0 or self.pol[k] != 1:
                    mid = mid * self.pol[k]
                result += mid
            return result

        if x2 is None:
            result = calc(x1)
        else:
            result = calc(x2) - calc(x1)

        return result


def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()