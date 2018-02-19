# Converts ADC to fC (Nate Chaverin's class)

import numpy as np
try:
    import matplotlib.pyplot as plt
except:
    print "matplotlib.pyplot cannot be imported"
try:
    from adc2fc_point5_dqm import adc2fc
except:
    print "adc2fc cannot be imported from adc2fc_point5_dqm"

class ADCConverter:
    # Bitmasks for 8-bit ADC input
    expMask = 192
    manMask = 63
    baseSensitivity = 3.1
    # Base charges for each subrange
    # Use array for which 0 ADC = 0 fC input charge
    inputCharge = [
        -1.6, 48.4, 172.4, 433.4, 606.4,
        517, 915, 1910, 3990, 5380,
        4780, 7960, 15900, 32600, 43700,
        38900, 64300, 128000, 261000, 350000
    ]

    #Defines the size of the ADC mantissa subranges
    adcBase = [0, 16, 36, 57, 64]

    # A class to convert ADC to fC
    fc = {}

    def __init__(self, unit="fc", shunt=0):
        self.unit = unit
        self.shunt = shunt
        self.shuntFactor = self.getShuntFactor()
        print "shunt : {0} factor: {1}".format(self.shunt, self.shuntFactor)
        # Loop over exponents, 0 - 3
        for exp in xrange(0, 4):
            # Loop over mantissas, 0 - 63
            for man in xrange(0, 64):
                subrange = -1
                # Find which subrange the mantissa is in
                for i in xrange(0, 4):
                    if man >= self.adcBase[i] and man < self.adcBase[i + 1]:
                        subrange = i

                if subrange == -1:
                    print 'Something has gone horribly wrong: subrange = -1 in ADCConverter.py (line 34)!'

                # Sensitivity = 3.1 * 8^exp * 2^subrange
                sensitivity = self.baseSensitivity * 8.0**float(exp) * 2.0**subrange
                # Add sensitivity * (location in subrange) to base charge
                #fc[exp * 64 + man] = (inputCharge[exp * 5 + subrange] + ((man - adcBase[subrange])) * sensitivity) / gain;
                self.fc[exp * 64 + man] = self.inputCharge[exp * 5 + subrange] + ((man - self.adcBase[subrange]) + .5) * sensitivity

    def linearize(self, adc, version=1):
        units = {"fc" : 10.0 ** 0,
                 "pc" : 10.0 ** 3,
                 "nc" : 10.0 ** 6}
        if self.unit not in units:
            print "The unit {0} is not in the list {1}".format(self.unit, units)
            return -1
        unitFactor = units[self.unit]
        adc = int(adc) # in case of string
        if adc > 255: adc = 255
        fc = 0.0
        if version == 0:
            fc = self.fc[adc]
        elif version == 1:
            fc = adc2fc[adc]
        return self.shuntFactor * fc / unitFactor
    
    def getBins(self):
        xbins = []
        bin_0   = (3*self.linearize(0)   - self.linearize(1))   / 2.0
        bin_256 = (3*self.linearize(255) - self.linearize(254)) / 2.0
        xbins.append(bin_0)
        b = bin_0
        for adc in xrange(1,256):
            a = b
            b = self.linearize(adc)
            xbins.append((b+a)/2.0)
        xbins.append(bin_256)
        return xbins

    def getShuntFactor(self):
        if self.shunt < 0 or self.shunt > 31:
            print "The shunt outside the available range 0 to 31."
            return -1
        cookies = [4, 3, 2, 1, 0.5]
        cake = 1 # factor
        crumbs = bin(self.shunt).split('0b')[-1] # bits
        for i in xrange(1, len(crumbs) + 1):
            cake += int(crumbs[-i]) * cookies[-i]
        return cake

    def plot(self):
        raw_colors = ["pinkish red","azure","bluish green","electric purple","tangerine","neon pink","dark sky blue","avocado"]
        colors = list("xkcd:{0}".format(c) for c in raw_colors)
        adc_ranges = list(list(i + j for j in xrange(64)) for i in xrange(0,256,64))
        
        # plot each range on separate plot
        for i, adc_range in enumerate(adc_ranges):
            fig, ax = plt.subplots()
            color = colors[i % len(colors)] # in case there are more ranges than colors
            name = "Range {0}".format(i)
            charge = []
            for adc in adc_range:
                charge.append(self.linearize(adc))
            ax.plot(adc_range, charge, 'o', c=color, label=name, alpha=0.5)
        
            legend = ax.legend(loc='upper left')
            ax.grid(True)
            plt.title("ADC to {0} using Shunt Factor {1}".format(self.shuntFactor, self.unit))
            plt.xlabel("ADC")
            plt.ylabel(self.unit)
            plt.savefig("ADC_to_{0}_range{1}.png".format(i, self.unit))
            plt.savefig("ADC_to_{0}_range{1}.pdf".format(i, self.unit))
            plt.clf()

        # plot all ranges on one plot
        fig, ax = plt.subplots()
        for i, adc_range in enumerate(adc_ranges):
            color = colors[i % len(colors)] # in case there are more ranges than colors
            name = "Range {0}".format(i)
            charge = []
            for adc in adc_range:
                charge.append(self.linearize(adc))
            ax.plot(adc_range, charge, 'o', c=color, label=name, alpha=0.5)
        
        legend = ax.legend(loc='upper left')
        ax.grid(True)
        plt.title("ADC to {0} using Shunt Factor {1}".format(self.shuntFactor, self.unit))
        plt.xlabel("ADC")
        plt.ylabel(self.unit)
        plt.savefig("ADC_to_{0}.png".format(self.unit))
        plt.savefig("ADC_to_{0}.pdf".format(self.unit))
        plt.clf()

if __name__ == "__main__":
    for s in xrange(32):
        ADCConverter("pc", s)
    converter = ADCConverter("fc", 0) # unit, shunt
    xbins = converter.getBins()
    cleanBins = []
    print "ADC : pC : xbin"
    for adc in xrange(257):
        print "%3d : %.4f : %.4f" % (adc, converter.linearize(adc), xbins[adc])
        cleanBins.append(float("%.2f" % xbins[adc]))
    converter.plot()


