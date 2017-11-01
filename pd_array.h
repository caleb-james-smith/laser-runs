#include <vector>

// CU             RBX            Run            pd_ch          uhtr_ch        shunt          max_adc        max_fc         result         
std::vector< std::vector<double>> pd_array = {
{1, 0, 1, 0, 0, 31, 0, -0.05000, 0},
{1, 0, 1, 1, 1, 31, 0, -0.05000, 0},
{2, 0, 1, 0, 0, 31, 0, -0.05000, 0},
{2, 0, 1, 1, 1, 31, 0, -0.05000, 0},
{2, 0, 2, 0, 0, 31, 0, -0.05000, 0},
{2, 0, 2, 1, 1, 31, 0, -0.05000, 0},
{2, 0, 3, 0, 0, 31, 0, -0.05000, 0},
{2, 0, 3, 1, 1, 31, 0, -0.05000, 0},
{2, 0, 4, 0, 2, 31, 0, -0.05000, 0},
{2, 0, 5, 0, 3, 31, 0, -0.05000, 0},
{2, 0, 6, 0, 4, 31, 0, -0.05000, 0},
{2, 0, 7, 0, 5, 31, 0, -0.05000, 0},
{4, 0, 1, 0, 0, 31, 0, -0.05000, 0},
{4, 0, 1, 1, 1, 31, 0, -0.05000, 0},
{4, 0, 2, 0, 0, 31, 0, -0.05000, 0},
{4, 0, 2, 1, 1, 31, 0, -0.05000, 0},
{4, 0, 3, 0, 0, 31, 0, -0.05000, 0},
{4, 0, 3, 1, 1, 31, 0, -0.05000, 0},
{6, 0, 1, 0, 0, 31, 185, 33393.60000, 1},
{6, 0, 1, 1, 1, 31, 169, 20264.80000, 1},
{6, 0, 2, 0, 0, 31, 185, 33393.60000, 1},
{6, 0, 2, 1, 1, 31, 170, 21058.40000, 1},
{6, 0, 3, 0, 0, 31, 186, 34980.80000, 1},
{6, 0, 3, 1, 1, 31, 171, 21852.00000, 1},
{6, 3, 4, 0, 2, 31, 210, 72236.00000, 1},
{6, 3, 5, 0, 3, 31, 187, 36568.00000, 1},
{6, 3, 6, 0, 4, 31, 30, 138.30000, 0},
{6, 3, 7, 0, 5, 31, 174, 24232.80000, 1},
{7, 0, 1, 0, 0, 31, 188, 38155.20000, 1},
{7, 0, 1, 1, 1, 31, 168, 19471.20000, 1},
{7, 0, 2, 0, 0, 31, 179, 28200.80000, 1},
{7, 0, 2, 1, 1, 31, 159, 14110.40000, 1},
{7, 0, 3, 0, 0, 31, 247, 251801.60000, 1},
{7, 0, 3, 1, 1, 31, 225, 119852.00000, 1},
{7, 0, 4, 0, 2, 0, 40, 228.20000, 0},
{7, 0, 4, 0, 2, 31, 12, 37.15000, 0},
{7, 0, 5, 0, 3, 0, 103, 2257.20000, 0},
{7, 0, 5, 0, 3, 31, 7, 21.65000, 0},
{7, 0, 6, 0, 4, 0, 105, 2455.60000, 0},
{7, 0, 6, 0, 4, 31, 7, 21.65000, 0},
{7, 0, 7, 0, 5, 0, 104, 2356.40000, 0},
{7, 0, 7, 0, 5, 31, 6, 18.55000, 0},
{8, 0, 1, 0, 0, 31, 196, 46042.40000, 1},
{8, 0, 1, 1, 1, 31, 178, 27407.20000, 1},
{8, 0, 2, 0, 0, 31, 188, 38155.20000, 1},
{8, 0, 2, 1, 1, 31, 172, 22645.60000, 1},
{8, 0, 3, 0, 0, 31, 189, 39742.40000, 1},
{8, 0, 3, 1, 1, 31, 174, 24232.80000, 1},
{8, 0, 4, 0, 2, 0, 8, 24.75000, 0},
{8, 0, 5, 0, 3, 0, 8, 24.75000, 0},
{8, 0, 6, 0, 4, 0, 9, 27.85000, 0},
{8, 0, 7, 0, 5, 0, 7, 21.65000, 0},
{9, 0, 1, 0, 0, 31, 8, 24.75000, 0},
{9, 0, 1, 1, 1, 31, 8, 24.75000, 0},
{9, 0, 2, 0, 0, 31, 7, 21.65000, 0},
{9, 0, 2, 1, 1, 31, 6, 18.55000, 0},
{9, 0, 3, 0, 0, 31, 153, 11729.60000, 1},
{9, 0, 3, 1, 1, 31, 141, 7458.40000, 1},
{9, 0, 4, 0, 2, 0, 102, 2158.00000, 0},
{9, 0, 4, 0, 2, 31, 44, 277.80000, 0},
{9, 0, 5, 0, 3, 31, 108, 2753.20000, 1},
{9, 0, 6, 0, 4, 0, 102, 2158.00000, 0},
{9, 0, 7, 0, 5, 0, 104, 2356.40000, 0},
{10, 0, 1, 0, 0, 31, 29, 132.10000, 0},
{10, 0, 1, 1, 1, 31, 21, 82.50000, 0},
{10, 0, 2, 0, 0, 31, 29, 132.10000, 0},
{10, 0, 2, 1, 1, 31, 21, 82.50000, 0},
{10, 0, 3, 0, 0, 31, 29, 132.10000, 0},
{10, 0, 3, 1, 1, 31, 21, 82.50000, 0},
{10, 0, 4, 0, 2, 31, 215, 88108.00000, 1},
{10, 0, 5, 0, 3, 31, 213, 81759.20000, 1},
{10, 0, 6, 0, 4, 31, 68, 628.60000, 0},
{10, 0, 7, 0, 5, 31, 215, 88108.00000, 1},
{10, 9, 1, 0, 0, 31, 4, 12.35000, 0},
{10, 9, 1, 1, 1, 31, 6, 18.55000, 0},
{10, 9, 2, 0, 0, 31, 4, 12.35000, 0},
{10, 9, 2, 1, 1, 31, 6, 18.55000, 0},
{10, 9, 3, 0, 0, 31, 4, 12.35000, 0},
{10, 9, 3, 1, 1, 31, 6, 18.55000, 0},
{10, 9, 4, 0, 2, 31, 7, 21.65000, 0},
{10, 9, 5, 0, 3, 31, 5, 15.45000, 0},
{10, 9, 6, 0, 4, 31, 9, 27.85000, 0},
{10, 9, 7, 0, 5, 31, 12, 37.15000, 0},
{11, 0, 1, 0, 0, 31, 195, 44455.20000, 1},
{11, 0, 1, 1, 1, 31, 181, 29788.00000, 1},
{11, 0, 2, 0, 0, 31, 195, 44455.20000, 1},
{11, 0, 2, 1, 1, 31, 181, 29788.00000, 1},
{11, 0, 3, 0, 0, 31, 195, 44455.20000, 1},
{11, 0, 3, 1, 1, 31, 182, 30581.60000, 1},
{11, 0, 4, 0, 2, 31, 214, 84933.60000, 1},
{11, 0, 5, 0, 3, 31, 215, 88108.00000, 1},
{11, 0, 6, 0, 4, 31, 62, 569.80000, 0},
{11, 4, 4, 0, 2, 31, 210, 72236.00000, 1},
{11, 4, 5, 0, 3, 31, 187, 36568.00000, 1},
{11, 4, 6, 0, 4, 31, 30, 138.30000, 0},
{11, 4, 7, 0, 5, 31, 174, 24232.80000, 1},
{12, 0, 1, 0, 0, 31, 200, 52391.20000, 1},
{12, 0, 1, 1, 1, 31, 198, 49216.80000, 1},
{12, 0, 2, 0, 0, 31, 200, 52391.20000, 1},
{12, 0, 2, 1, 1, 31, 198, 49216.80000, 1},
{12, 0, 3, 0, 0, 31, 200, 52391.20000, 1},
{12, 0, 3, 1, 1, 31, 197, 47629.60000, 1},
{12, 0, 4, 0, 2, 31, 212, 78584.80000, 1},
{12, 0, 5, 0, 3, 31, 14, 43.35000, 0},
{12, 0, 6, 0, 4, 31, 8, 24.75000, 0},
{12, 5, 4, 0, 2, 31, 210, 72236.00000, 1},
{12, 5, 5, 0, 3, 31, 187, 36568.00000, 1},
{12, 5, 6, 0, 4, 31, 30, 138.30000, 0},
{12, 5, 7, 0, 5, 31, 174, 24232.80000, 1},
{13, 0, 1, 0, 0, 31, 201, 53978.40000, 1},
{13, 0, 1, 1, 1, 31, 200, 52391.20000, 1},
{13, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{13, 0, 2, 1, 1, 31, 200, 52391.20000, 1},
{13, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{13, 0, 3, 1, 1, 31, 199, 50804.00000, 1},
{13, 0, 4, 0, 2, 31, 217, 94456.80000, 1},
{13, 0, 5, 0, 3, 31, 219, 100805.60000, 1},
{13, 0, 6, 0, 4, 31, 67, 603.80000, 0},
{13, 0, 7, 0, 5, 31, 219, 100805.60000, 1},
{13, 6, 4, 0, 2, 31, 210, 72236.00000, 1},
{13, 6, 5, 0, 3, 31, 187, 36568.00000, 1},
{13, 6, 6, 0, 4, 31, 30, 138.30000, 0},
{13, 6, 7, 0, 5, 31, 174, 24232.80000, 1},
{14, 0, 1, 0, 0, 31, 199, 50804.00000, 1},
{14, 0, 1, 1, 1, 31, 195, 44455.20000, 1},
{14, 0, 2, 0, 0, 31, 199, 50804.00000, 1},
{14, 0, 2, 1, 1, 31, 194, 42868.00000, 1},
{14, 0, 3, 0, 0, 31, 199, 50804.00000, 1},
{14, 0, 3, 1, 1, 31, 194, 42868.00000, 1},
{14, 0, 4, 0, 2, 31, 205, 60327.20000, 1},
{14, 0, 5, 0, 3, 31, 212, 78584.80000, 1},
{14, 8, 4, 0, 2, 31, 7, 21.65000, 0},
{14, 8, 5, 0, 3, 31, 5, 15.45000, 0},
{14, 8, 6, 0, 4, 31, 9, 27.85000, 0},
{14, 8, 7, 0, 5, 31, 12, 37.15000, 0},
{15, 0, 1, 0, 0, 31, 170, 21058.40000, 1},
{15, 0, 1, 1, 1, 31, 154, 12126.40000, 1},
{15, 0, 2, 0, 0, 31, 189, 39742.40000, 1},
{15, 0, 2, 1, 1, 31, 158, 13713.60000, 1},
{15, 0, 3, 0, 0, 31, 189, 39742.40000, 1},
{15, 0, 3, 1, 1, 31, 160, 14507.20000, 1},
{15, 2, 1, 0, 0, 0, 174, 24232.80000, 1},
{15, 2, 1, 1, 1, 0, 220, 103980.00000, 1},
{15, 2, 1, 0, 0, 31, 174, 24232.80000, 1},
{15, 2, 1, 1, 1, 31, 220, 103980.00000, 1},
{15, 2, 2, 0, 0, 31, 174, 24232.80000, 1},
{15, 2, 2, 1, 1, 31, 220, 103980.00000, 1},
{15, 2, 3, 0, 0, 31, 174, 24232.80000, 1},
{15, 2, 3, 1, 1, 31, 220, 103980.00000, 1},
{15, 2, 4, 0, 2, 31, 210, 72236.00000, 1},
{15, 2, 5, 0, 3, 31, 187, 36568.00000, 1},
{15, 2, 6, 0, 4, 31, 30, 138.30000, 0},
{15, 2, 7, 0, 5, 31, 174, 24232.80000, 1},
{16, 0, 1, 0, 0, 31, 21, 82.50000, 0},
{16, 0, 1, 1, 1, 31, 18, 63.90000, 0},
{16, 0, 2, 0, 0, 31, 20, 76.30000, 0},
{16, 0, 2, 1, 1, 31, 18, 63.90000, 0},
{16, 0, 3, 0, 0, 31, 20, 76.30000, 0},
{16, 0, 3, 1, 1, 31, 18, 63.90000, 0},
{16, 0, 4, 0, 2, 31, 7, 21.65000, 0},
{16, 0, 5, 0, 3, 31, 10, 30.95000, 0},
{16, 0, 6, 0, 4, 31, 9, 27.85000, 0},
{16, 7, 1, 0, 0, 31, 6, 18.55000, 0},
{16, 7, 1, 1, 1, 31, 7, 21.65000, 0},
{16, 7, 4, 0, 2, 31, 199, 50804.00000, 1},
{16, 7, 5, 0, 3, 31, 201, 53978.40000, 1},
{16, 7, 6, 0, 4, 31, 201, 53978.40000, 1},
{16, 7, 7, 0, 5, 31, 33, 156.90000, 0},
{17, 0, 1, 0, 0, 31, 203, 57152.80000, 1},
{17, 0, 1, 1, 1, 31, 203, 57152.80000, 1},
{17, 0, 2, 0, 0, 31, 203, 57152.80000, 1},
{17, 0, 2, 1, 1, 31, 203, 57152.80000, 1},
{17, 0, 3, 0, 0, 31, 203, 57152.80000, 1},
{17, 0, 3, 1, 1, 31, 203, 57152.80000, 1},
{17, 0, 4, 0, 2, 31, 219, 100805.60000, 1},
{17, 0, 5, 0, 3, 31, 218, 97631.20000, 1},
{17, 0, 6, 0, 4, 31, 66, 579.00000, 0},
{17, 10, 1, 0, 0, 31, 4, 12.35000, 0},
{17, 10, 1, 1, 1, 31, 6, 18.55000, 0},
{17, 10, 4, 0, 2, 31, 7, 21.65000, 0},
{17, 10, 5, 0, 3, 31, 5, 15.45000, 0},
{17, 10, 6, 0, 4, 31, 9, 27.85000, 0},
{17, 10, 7, 0, 5, 31, 12, 37.15000, 0},
{18, 0, 1, 0, 0, 31, 201, 53978.40000, 1},
{18, 0, 1, 1, 1, 31, 199, 50804.00000, 1},
{18, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{18, 0, 2, 1, 1, 31, 199, 50804.00000, 1},
{18, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{18, 0, 3, 1, 1, 31, 199, 50804.00000, 1},
{18, 1, 1, 0, 0, 0, 174, 24232.80000, 1},
{18, 1, 1, 1, 1, 0, 220, 103980.00000, 1},
{18, 1, 1, 0, 0, 31, 174, 24232.80000, 1},
{18, 1, 1, 1, 1, 31, 220, 103980.00000, 1},
{18, 1, 2, 0, 0, 0, 174, 24232.80000, 1},
{18, 1, 2, 1, 1, 0, 220, 103980.00000, 1},
{18, 1, 2, 0, 0, 31, 174, 24232.80000, 1},
{18, 1, 2, 1, 1, 31, 220, 103980.00000, 1},
{18, 1, 3, 0, 0, 0, 174, 24232.80000, 1},
{18, 1, 3, 1, 1, 0, 220, 103980.00000, 1},
{18, 1, 3, 0, 0, 31, 174, 24232.80000, 1},
{18, 1, 3, 1, 1, 31, 220, 103980.00000, 1},
{18, 1, 4, 0, 2, 0, 210, 72236.00000, 1},
{18, 1, 4, 0, 2, 31, 210, 72236.00000, 1},
{18, 1, 5, 0, 3, 0, 187, 36568.00000, 1},
{18, 1, 5, 0, 3, 31, 187, 36568.00000, 1},
{18, 1, 6, 0, 4, 0, 30, 138.30000, 0},
{18, 1, 6, 0, 4, 31, 30, 138.30000, 0},
{18, 1, 7, 0, 5, 0, 174, 24232.80000, 1},
{18, 1, 7, 0, 5, 31, 174, 24232.80000, 1},
{19, 0, 1, 0, 0, 0, 13, 40.25000, 0},
{19, 0, 1, 1, 1, 0, 10, 30.95000, 0},
{19, 0, 1, 0, 0, 31, 201, 53978.40000, 1},
{19, 0, 1, 1, 1, 31, 194, 42868.00000, 1},
{19, 0, 2, 0, 0, 0, 13, 40.25000, 0},
{19, 0, 2, 1, 1, 0, 10, 30.95000, 0},
{19, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{19, 0, 2, 1, 1, 31, 191, 42916.80000, 1},
{19, 0, 3, 0, 0, 0, 14, 43.35000, 0},
{19, 0, 3, 1, 1, 0, 10, 30.95000, 0},
{19, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{19, 0, 3, 1, 1, 31, 194, 42868.00000, 1},
{19, 0, 4, 0, 2, 0, 106, 2554.80000, 0},
{19, 0, 4, 0, 2, 31, 189, 39742.40000, 1},
{19, 0, 5, 0, 3, 0, 105, 2455.60000, 0},
{19, 0, 5, 0, 3, 31, 125, 4882.80000, 1},
{19, 0, 6, 0, 4, 0, 106, 2554.80000, 0},
{19, 0, 7, 0, 5, 0, 41, 240.60000, 0},
{20, 0, 1, 0, 0, 0, 8, 24.75000, 0},
{20, 0, 1, 1, 1, 0, 8, 24.75000, 0},
{20, 0, 1, 0, 0, 31, 189, 39742.40000, 1},
{20, 0, 1, 1, 1, 31, 157, 13316.80000, 1},
{20, 0, 2, 0, 0, 0, 8, 24.75000, 0},
{20, 0, 2, 1, 1, 0, 8, 24.75000, 0},
{20, 0, 2, 0, 0, 31, 189, 39742.40000, 1},
{20, 0, 2, 1, 1, 31, 158, 13713.60000, 1},
{20, 0, 3, 0, 0, 0, 8, 24.75000, 0},
{20, 0, 3, 1, 1, 0, 8, 24.75000, 0},
{20, 0, 3, 0, 0, 31, 189, 39742.40000, 1},
{20, 0, 3, 1, 1, 31, 158, 13713.60000, 1},
{20, 0, 4, 0, 2, 0, 104, 2356.40000, 0},
{20, 0, 5, 0, 3, 0, 105, 2455.60000, 0},
{20, 0, 6, 0, 4, 0, 108, 2753.20000, 0},
{20, 0, 7, 0, 5, 0, 104, 2356.40000, 0},
{22, 0, 1, 0, 0, 31, 17, 57.70000, 0},
{22, 0, 1, 1, 1, 31, 17, 57.70000, 0},
{22, 0, 2, 0, 0, 31, 20, 76.30000, 0},
{22, 0, 2, 1, 1, 31, 18, 63.90000, 0},
{22, 0, 3, 0, 0, 31, 15, 46.45000, 0},
{22, 0, 3, 1, 1, 31, 10, 30.95000, 0},
{22, 0, 4, 0, 2, 31, 70, 678.20000, 0},
{22, 0, 5, 0, 3, 31, 59, 495.40000, 0},
{22, 0, 6, 0, 4, 31, 6, 18.55000, 0},
{22, 0, 7, 0, 5, 31, 7, 21.65000, 0},
{22, 12, 1, 0, 0, 31, 4, 12.35000, 0},
{22, 12, 1, 1, 1, 31, 6, 18.55000, 0},
{22, 12, 2, 0, 0, 31, 4, 12.35000, 0},
{22, 12, 2, 1, 1, 31, 6, 18.55000, 0},
{22, 12, 3, 0, 0, 31, 4, 12.35000, 0},
{22, 12, 3, 1, 1, 31, 6, 18.55000, 0},
{22, 12, 4, 0, 2, 31, 7, 21.65000, 0},
{22, 12, 5, 0, 3, 31, 5, 15.45000, 0},
{22, 12, 6, 0, 4, 31, 9, 27.85000, 0},
{22, 12, 7, 0, 5, 31, 12, 37.15000, 0},
{23, 0, 1, 0, 0, 31, 201, 53978.40000, 1},
{23, 0, 1, 1, 1, 31, 199, 50804.00000, 1},
{23, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{23, 0, 2, 1, 1, 31, 200, 52391.20000, 1},
{23, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{23, 0, 3, 1, 1, 31, 200, 52391.20000, 1},
{23, 0, 4, 0, 2, 31, 211, 75410.40000, 1},
{23, 0, 5, 0, 3, 31, 212, 78584.80000, 1},
{23, 0, 6, 0, 4, 31, 67, 603.80000, 0},
{23, 0, 7, 0, 5, 31, 213, 81759.20000, 1},
{23, 11, 4, 0, 2, 31, 7, 21.65000, 0},
{23, 11, 5, 0, 3, 31, 5, 15.45000, 0},
{23, 11, 6, 0, 4, 31, 9, 27.85000, 0},
{23, 11, 7, 0, 5, 31, 12, 37.15000, 0},
{24, 0, 1, 0, 0, 31, 201, 53978.40000, 1},
{24, 0, 1, 1, 1, 31, 198, 49216.80000, 1},
{24, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{24, 0, 2, 1, 1, 31, 198, 49216.80000, 1},
{24, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{24, 0, 3, 1, 1, 31, 198, 49216.80000, 1},
{24, 0, 4, 0, 2, 31, 207, 63501.60000, 1},
{24, 0, 5, 0, 3, 31, 208, 65887.20000, 1},
{24, 0, 6, 0, 4, 31, 208, 65887.20000, 1},
{24, 0, 7, 0, 5, 31, 209, 69061.60000, 1},
{25, 0, 1, 0, 0, 31, 199, 50804.00000, 1},
{25, 0, 1, 1, 1, 31, 198, 49216.80000, 1},
{25, 0, 2, 0, 0, 31, 199, 50804.00000, 1},
{25, 0, 2, 1, 1, 31, 198, 49216.80000, 1},
{25, 0, 3, 0, 0, 31, 200, 52391.20000, 1},
{25, 0, 3, 1, 1, 31, 198, 49216.80000, 1},
{25, 0, 4, 0, 2, 31, 209, 69061.60000, 1},
{25, 0, 5, 0, 3, 31, 210, 72236.00000, 1},
{25, 0, 6, 0, 4, 31, 211, 75410.40000, 1},
{25, 0, 7, 0, 5, 31, 210, 72236.00000, 1},
{26, 0, 1, 0, 0, 31, 10, 30.95000, 0},
{26, 0, 1, 1, 1, 31, 9, 27.85000, 0},
{26, 0, 2, 0, 0, 31, 10, 30.95000, 0},
{26, 0, 2, 1, 1, 31, 11, 34.05000, 0},
{26, 0, 3, 0, 0, 31, 10, 30.95000, 0},
{26, 0, 3, 1, 1, 31, 9, 27.85000, 0},
{26, 0, 4, 0, 2, 31, 217, 94456.80000, 1},
{26, 0, 5, 0, 3, 31, 216, 91282.40000, 1},
{26, 0, 6, 0, 4, 31, 67, 603.80000, 0},
{26, 0, 7, 0, 5, 31, 214, 84933.60000, 1},
{26, 13, 1, 0, 0, 31, 4, 12.35000, 0},
{26, 13, 1, 1, 1, 31, 6, 18.55000, 0},
{26, 13, 2, 0, 0, 31, 4, 12.35000, 0},
{26, 13, 2, 1, 1, 31, 6, 18.55000, 0},
{26, 13, 3, 0, 0, 31, 4, 12.35000, 0},
{26, 13, 3, 1, 1, 31, 6, 18.55000, 0},
{26, 13, 4, 0, 2, 31, 7, 21.65000, 0},
{26, 13, 5, 0, 3, 31, 5, 15.45000, 0},
{26, 13, 6, 0, 4, 31, 9, 27.85000, 0},
{26, 13, 7, 0, 5, 31, 12, 37.15000, 0},
{27, 0, 1, 0, 0, 31, 27, 119.70000, 0},
{27, 0, 1, 1, 1, 31, 21, 82.50000, 0},
{27, 0, 2, 0, 0, 31, 17, 57.70000, 0},
{27, 0, 2, 1, 1, 31, 13, 40.25000, 0},
{27, 0, 3, 0, 0, 31, 17, 57.70000, 0},
{27, 0, 3, 1, 1, 31, 13, 40.25000, 0},
{27, 0, 4, 0, 2, 31, 169, 20264.80000, 1},
{27, 0, 5, 0, 3, 31, 218, 97631.20000, 1},
{27, 0, 6, 0, 4, 31, 66, 579.00000, 0},
{27, 0, 7, 0, 5, 31, 215, 88108.00000, 1},
{29, 0, 1, 0, 0, 31, 165, 17090.40000, 1},
{29, 0, 1, 1, 1, 31, 148, 9745.60000, 1},
{29, 0, 2, 0, 0, 31, 166, 17884.00000, 1},
{29, 0, 2, 1, 1, 31, 149, 10142.40000, 1},
{29, 0, 3, 0, 0, 31, 163, 15697.60000, 1},
{29, 0, 3, 1, 1, 31, 147, 9348.80000, 1},
{29, 0, 4, 0, 2, 31, 210, 72236.00000, 1},
{29, 0, 5, 0, 3, 31, 210, 72236.00000, 1},
{29, 0, 6, 0, 4, 31, 210, 72236.00000, 1},
{29, 0, 7, 0, 5, 31, 210, 72236.00000, 1},
{30, 0, 1, 0, 0, 31, 200, 52391.20000, 1},
{30, 0, 1, 1, 1, 31, 194, 42868.00000, 1},
{30, 0, 2, 0, 0, 31, 200, 52391.20000, 1},
{30, 0, 2, 1, 1, 31, 191, 42916.80000, 1},
{30, 0, 3, 0, 0, 31, 199, 50804.00000, 1},
{30, 0, 3, 1, 1, 31, 189, 39742.40000, 1},
{30, 0, 4, 0, 2, 31, 211, 75410.40000, 1},
{30, 0, 5, 0, 3, 31, 212, 78584.80000, 1},
{30, 0, 6, 0, 4, 31, 212, 78584.80000, 1},
{30, 0, 7, 0, 5, 31, 213, 81759.20000, 1},
{31, 0, 1, 0, 0, 0, 17, 57.70000, 0},
{31, 0, 1, 1, 1, 0, 14, 43.35000, 0},
{31, 0, 1, 0, 0, 31, 203, 57152.80000, 1},
{31, 0, 1, 1, 1, 31, 201, 53978.40000, 1},
{31, 0, 2, 0, 0, 0, 17, 57.70000, 0},
{31, 0, 2, 1, 1, 0, 14, 43.35000, 0},
{31, 0, 2, 0, 0, 31, 203, 57152.80000, 1},
{31, 0, 2, 1, 1, 31, 201, 53978.40000, 1},
{31, 0, 3, 0, 0, 0, 16, 51.50000, 0},
{31, 0, 3, 1, 1, 0, 13, 40.25000, 0},
{31, 0, 3, 0, 0, 31, 203, 57152.80000, 1},
{31, 0, 3, 1, 1, 31, 201, 53978.40000, 1},
{31, 0, 4, 0, 2, 0, 1, 3.05000, 0},
{31, 0, 5, 0, 3, 0, 107, 2654.00000, 0},
{31, 0, 6, 0, 4, 0, 105, 2455.60000, 0},
{31, 0, 7, 0, 5, 0, 105, 2455.60000, 0},
{32, 0, 1, 0, 0, 31, 166, 17884.00000, 1},
{32, 0, 1, 1, 1, 31, 148, 9745.60000, 1},
{32, 0, 2, 0, 0, 31, 165, 17090.40000, 1},
{32, 0, 2, 1, 1, 31, 147, 9348.80000, 1},
{32, 0, 3, 0, 0, 31, 168, 19471.20000, 1},
{32, 0, 3, 1, 1, 31, 149, 10142.40000, 1},
{32, 0, 4, 0, 2, 31, 209, 69061.60000, 1},
{32, 0, 5, 0, 3, 31, 213, 81759.20000, 1},
{32, 0, 6, 0, 4, 31, 212, 78584.80000, 1},
{32, 0, 7, 0, 5, 31, 211, 75410.40000, 1},
{33, 0, 1, 0, 0, 31, 170, 21058.40000, 1},
{33, 0, 1, 1, 1, 31, 158, 13713.60000, 1},
{33, 0, 2, 0, 0, 31, 9, 27.85000, 0},
{33, 0, 2, 1, 1, 31, 7, 21.65000, 0},
{33, 0, 3, 0, 0, 31, 9, 27.85000, 0},
{33, 0, 3, 1, 1, 31, 7, 21.65000, 0},
{33, 0, 4, 0, 2, 31, 7, 21.65000, 0},
{33, 0, 5, 0, 3, 31, 7, 21.65000, 0},
{33, 0, 6, 0, 4, 31, 66, 579.00000, 0},
{34, 0, 1, 0, 0, 31, 202, 55565.60000, 1},
{34, 0, 1, 1, 1, 31, 201, 53978.40000, 1},
{34, 0, 2, 0, 0, 31, 202, 55565.60000, 1},
{34, 0, 2, 1, 1, 31, 202, 55565.60000, 1},
{34, 0, 3, 0, 0, 31, 202, 55565.60000, 1},
{34, 0, 3, 1, 1, 31, 201, 53978.40000, 1},
{34, 0, 4, 0, 2, 0, 103, 2257.20000, 0},
{34, 0, 4, 0, 2, 31, 214, 84933.60000, 1},
{34, 0, 5, 0, 3, 0, 105, 2455.60000, 0},
{34, 0, 5, 0, 3, 31, 15, 46.45000, 0},
{34, 0, 6, 0, 4, 0, 105, 2455.60000, 0},
{34, 0, 6, 0, 4, 31, 8, 24.75000, 0},
{34, 0, 7, 0, 5, 0, 106, 2554.80000, 0},
{35, 0, 1, 0, 0, 31, 203, 57152.80000, 1},
{35, 0, 1, 1, 1, 31, 202, 55565.60000, 1},
{35, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{35, 0, 2, 1, 1, 31, 199, 50804.00000, 1},
{35, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{35, 0, 3, 1, 1, 31, 199, 50804.00000, 1},
{35, 0, 4, 0, 2, 0, 9, 27.85000, 0},
{35, 0, 5, 0, 3, 0, 9, 27.85000, 0},
{35, 0, 6, 0, 4, 0, 104, 2356.40000, 0},
{35, 0, 7, 0, 5, 0, 8, 24.75000, 0},
{36, 0, 4, 0, 2, 31, 213, 81759.20000, 1},
{36, 0, 5, 0, 3, 31, 213, 81759.20000, 1},
{36, 0, 6, 0, 4, 31, 214, 84933.60000, 1},
{36, 0, 7, 0, 5, 31, 212, 78584.80000, 1},
{37, 0, 1, 0, 0, 31, 188, 38155.20000, 1},
{37, 0, 1, 1, 1, 31, 173, 23439.20000, 1},
{37, 0, 2, 0, 0, 31, 189, 39742.40000, 1},
{37, 0, 2, 1, 1, 31, 173, 23439.20000, 1},
{37, 0, 3, 0, 0, 31, 189, 39742.40000, 1},
{37, 0, 3, 1, 1, 31, 173, 23439.20000, 1},
{37, 0, 4, 0, 2, 31, 211, 75410.40000, 1},
{37, 0, 5, 0, 3, 31, 210, 72236.00000, 1},
{37, 0, 6, 0, 4, 31, 211, 75410.40000, 1},
{37, 0, 7, 0, 5, 31, 211, 75410.40000, 1},
{38, 0, 1, 0, 0, 31, 200, 52391.20000, 1},
{38, 0, 1, 1, 1, 31, 200, 52391.20000, 1},
{38, 0, 2, 0, 0, 31, 200, 52391.20000, 1},
{38, 0, 2, 1, 1, 31, 201, 53978.40000, 1},
{38, 0, 3, 0, 0, 31, 200, 52391.20000, 1},
{38, 0, 3, 1, 1, 31, 200, 52391.20000, 1},
{38, 0, 4, 0, 2, 0, 105, 2455.60000, 0},
{38, 0, 4, 0, 2, 31, 213, 81759.20000, 1},
{38, 0, 5, 0, 3, 0, 106, 2554.80000, 0},
{38, 0, 5, 0, 3, 31, 213, 81759.20000, 1},
{38, 0, 6, 0, 4, 0, 106, 2554.80000, 0},
{38, 0, 6, 0, 4, 31, 68, 628.60000, 0},
{38, 0, 7, 0, 5, 0, 107, 2654.00000, 0},
{39, 0, 1, 0, 0, 31, 202, 55565.60000, 1},
{39, 0, 1, 1, 1, 31, 200, 52391.20000, 1},
{39, 0, 2, 0, 0, 31, 202, 55565.60000, 1},
{39, 0, 2, 1, 1, 31, 201, 53978.40000, 1},
{39, 0, 3, 0, 0, 31, 202, 55565.60000, 1},
{39, 0, 3, 1, 1, 31, 200, 52391.20000, 1},
{39, 0, 4, 0, 2, 0, 108, 2753.20000, 0},
{39, 0, 4, 0, 2, 31, 212, 78584.80000, 1},
{39, 0, 5, 0, 3, 0, 108, 2753.20000, 0},
{39, 0, 5, 0, 3, 31, 213, 81759.20000, 1},
{39, 0, 6, 0, 4, 0, 107, 2654.00000, 0},
{39, 0, 6, 0, 4, 31, 66, 579.00000, 0},
{39, 0, 7, 0, 5, 0, 105, 2455.60000, 0},
{39, 0, 7, 0, 5, 31, 212, 78584.80000, 1},
{40, 0, 1, 0, 0, 31, 196, 46042.40000, 1},
{40, 0, 1, 1, 1, 31, 182, 30581.60000, 1},
{40, 0, 2, 0, 0, 31, 196, 46042.40000, 1},
{40, 0, 2, 1, 1, 31, 182, 30581.60000, 1},
{40, 0, 3, 0, 0, 31, 195, 44455.20000, 1},
{40, 0, 3, 1, 1, 31, 181, 29788.00000, 1},
{40, 0, 4, 0, 2, 0, 8, 24.75000, 0},
{40, 0, 4, 0, 2, 31, 8, 24.75000, 0},
{40, 0, 5, 0, 3, 0, 105, 2455.60000, 0},
{40, 0, 5, 0, 3, 31, 14, 43.35000, 0},
{40, 0, 6, 0, 4, 0, 105, 2455.60000, 0},
{40, 0, 6, 0, 4, 31, 5, 15.45000, 0},
{40, 0, 7, 0, 5, 0, 105, 2455.60000, 0},
{41, 0, 1, 0, 0, 31, 171, 21852.00000, 1},
{41, 0, 1, 1, 1, 31, 155, 12523.20000, 1},
{41, 0, 2, 0, 0, 31, 170, 21058.40000, 1},
{41, 0, 2, 1, 1, 31, 155, 12523.20000, 1},
{41, 0, 3, 0, 0, 31, 169, 20264.80000, 1},
{41, 0, 3, 1, 1, 31, 153, 11729.60000, 1},
{41, 0, 4, 0, 2, 31, 212, 78584.80000, 1},
{41, 0, 5, 0, 3, 31, 213, 81759.20000, 1},
{41, 0, 6, 0, 4, 31, 67, 603.80000, 0},
{41, 0, 7, 0, 5, 31, 212, 78584.80000, 1},
{42, 0, 1, 0, 0, 31, 106, 2554.80000, 1},
{42, 0, 1, 1, 1, 31, 95, 1683.80000, 0},
{42, 0, 2, 0, 0, 31, 168, 19471.20000, 1},
{42, 0, 2, 1, 1, 31, 156, 12920.00000, 1},
{42, 0, 3, 0, 0, 31, 199, 50804.00000, 1},
{42, 0, 3, 1, 1, 31, 195, 44455.20000, 1},
{42, 0, 4, 0, 2, 31, 216, 91282.40000, 1},
{42, 0, 5, 0, 3, 31, 215, 88108.00000, 1},
{42, 0, 6, 0, 4, 31, 214, 84933.60000, 1},
{42, 0, 7, 0, 5, 31, 215, 88108.00000, 1},
{43, 0, 1, 0, 0, 31, 202, 55565.60000, 1},
{43, 0, 1, 1, 1, 31, 201, 53978.40000, 1},
{43, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{43, 0, 2, 1, 1, 31, 200, 52391.20000, 1},
{43, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{43, 0, 3, 1, 1, 31, 200, 52391.20000, 1},
{43, 0, 4, 0, 2, 31, 215, 88108.00000, 1},
{43, 0, 5, 0, 3, 31, 214, 84933.60000, 1},
{43, 0, 6, 0, 4, 31, 213, 81759.20000, 1},
{43, 0, 7, 0, 5, 31, 212, 78584.80000, 1},
{44, 0, 1, 0, 0, 31, 201, 53978.40000, 1},
{44, 0, 1, 1, 1, 31, 195, 44455.20000, 1},
{44, 0, 2, 0, 0, 31, 201, 53978.40000, 1},
{44, 0, 2, 1, 1, 31, 194, 42868.00000, 1},
{44, 0, 3, 0, 0, 31, 201, 53978.40000, 1},
{44, 0, 3, 1, 1, 31, 194, 42868.00000, 1},
{44, 0, 4, 0, 2, 31, 212, 78584.80000, 1},
{44, 0, 5, 0, 3, 31, 213, 81759.20000, 1},
{44, 0, 6, 0, 4, 31, 214, 84933.60000, 1},
{44, 0, 7, 0, 5, 31, 213, 81759.20000, 1}
};

