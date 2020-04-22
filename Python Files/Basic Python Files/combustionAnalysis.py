#this code is intended to take in some data from Aleks, specifically the combustion analysis problems and figure out the
#molecular structure based on the mass of the products of combustion


totalMass = float(input("enter the total mass of the sample(g):"))
molarMassOfCompound = input("enter the molar mass of compoundX(g/mol):")
molarMassOfCompoundInt = int(molarMassOfCompound)
massOfCO2 = float(input("enter the mass of CO2 in compoundX(g):"))
massOfH2O = float(input("enter the mass of H2O in compoundX(g):"))
molarMassOfCO2=44.01
molarMassOfH2O=18.01528
molarMassOfH=1.00794
molarMassOfO=15.994
molarMassOfC=12.0107

def compoundX():

   # print(totalMass, molarMassOfCompound,massOfCO2,massOfH2O)

    #finds the amount of moles of carbon, hydrogen, and oxygen in the products
    molC = massOfCO2/molarMassOfCO2
    massC = molC*molarMassOfC

    molH = 2 * (massOfH2O/molarMassOfH2O)
    massH = molH * molarMassOfH

    massO = totalMass - massH - massC
    molO = massO/molarMassOfO


    #determines the empirical formula of the substance(CXHY or CXHYOZ) by finding the ratio of moles to each other
    ratioHOverC = molH/molC

    if molO>0.08:
        ratioOOverC = molO/molC
    else:
        ratioOOverC = 0
    for x in range(1, 8):
        #print(round(ratioHOverC*x, 1), round(ratioOOverC*x, 1), x)



        if (round(ratioHOverC*x,1) %2 ==0 or round(ratioHOverC*x,1) %2 == 1) and (round(ratioOOverC*x, 1) %2==0 or round(ratioOOverC*x, 1) %2==1 or ratioOOverC == 0) :
            n = x
            break
    #below are the number of C,H,O in the empirical substance
    compoundXC = n
    compoundXH = int(round(ratioHOverC*n, 1))
    compoundXO = int(round(ratioOOverC*n, 1))

    print("empirical formula","C",compoundXC, "H",compoundXH, "O",compoundXO)

    #finds the molar mass of the empirical formula, and finds the scalar to add up to the molar mass of the compound to
    #determine it's molecular formula
    molarMassEmpirical = int((compoundXC * molarMassOfC) + (compoundXH * molarMassOfH) + (compoundXO * molarMassOfO))
    m=0
    for y in range(1, 20):

        if molarMassEmpirical*y == molarMassOfCompoundInt:
            m = y
            break

   # print(m)
    print("molecular formula","C",compoundXC*m, "H",compoundXH*m, "O",compoundXO*m)

compoundX()
