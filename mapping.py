# Mapping for CU Runs

# RBX : CU
rbxCU = {
"01"  : "19",  
"02"  : "43",
"03"  : "20",
"04"  : "42",
"05"  : "07",
"06"  : "35",
"07"  : "31",
"08"  : "09",
"09"  : "37",
"10"  : "08",
"11"  : "32",
"12"  : "33",
"13"  : "34",
"14"  : "40",
"15"  : "38",
"16"  : "41",
"17"  : "44",
"18"  : "39",
}

# First uHTR for RBX (smaller slot number)
# uHTR channels 96 - 143 (last RM for uHTR)
# RBX to RM for Run 4
rbxRM = {
"01"  : "3",  
"02"  : "2",
"03"  : "1",
"04"  : "3",
"05"  : "2",
"06"  : "1",
"07"  : "3",
"08"  : "2",
"09"  : "1",
"10"  : "3",
"11"  : "2",
"12"  : "1",
"13"  : "3",
"14"  : "2",
"15"  : "1",
"16"  : "3",
"17"  : "2",
"18"  : "1",
}


def rbxPindiodes(rbx):
    pd0 = int(rbxPD[rbx][0].split('h')[1])
    return list('h' + str(i) for i in xrange(pd0, pd0 + 6))

def chs(minCh, maxCh):
    return list('h' + str(i) for i in xrange(minCh, maxCh + 1))


# RBX : Pindiode Channel List (for channels 0 and 1 only)
rbxPD = {
"00"  : chs(0,5), 
"01"  : chs(72,77), 
"02"  : chs(84,89),
"03"  : chs(96,101), 
"04"  : chs(108,113),
"05"  : chs(120,125),
"06"  : chs(132,137),
"07"  : chs(0,5),
"08"  : chs(12,17),
"09"  : chs(24,29),
"10"  : chs(36,41),
"11"  : chs(48,53),
"12"  : chs(60,65),
"13"  : chs(72,77), 
"14"  : chs(84,89),
"15"  : chs(96,101), 
"16"  : chs(108,113),
"17"  : chs(120,125),
"18"  : chs(132,137)
}

# Each RBX has 192 channels
# Each uHTR has 144 channels
# Here is the mapping for 3 RBXs, 4 uHTRs
# 0-143   : 0-47  
# 48-143  : 0-95 
# 96-143  : 0-143

# RBX : SiPM Channel List. Note that each RBX has two lists for two uHTRs.
rbxSIPM = {
"00"  : [chs(12, 59) + chs(84, 131), chs(12, 59) + chs(84, 131)],
"01"  : [chs(0, 143), chs(0,47)],
"02"  : [chs(48,143), chs(0,95)],
"03"  : [chs(96,143), chs(0,143)],
"04"  : [chs(0, 143), chs(0,47)],
"05"  : [chs(48,143), chs(0,95)],
"06"  : [chs(96,143), chs(0,143)],
"07"  : [chs(0, 143), chs(0,47)],
"08"  : [chs(48,143), chs(0,95)],
"09"  : [chs(96,143), chs(0,143)],
"10"  : [chs(0, 143), chs(0,47)],
"11"  : [chs(48,143), chs(0,95)],
"12"  : [chs(96,143), chs(0,143)],
"13"  : [chs(0, 143), chs(0,47)],
"14"  : [chs(48,143), chs(0,95)],
"15"  : [chs(96,143), chs(0,143)],
"16"  : [chs(0, 143), chs(0,47)],
"17"  : [chs(48,143), chs(0,95)],
"18"  : [chs(96,143), chs(0,143)],
}

# There are 4 dark SiPM channels per RBX that do not receive data.
# This notation is RM (1-4) and SiPM (0-47)
darkSipms = [
("2","29"), 
("2","37"), 
("3","7"), 
("3","18"), 
]
