#include "Sequences.hpp"
#include <cassert>
#include "Vectorhelpers.hpp"
namespace dscr 
{	
	

	lluint binomial(lluint n, lluint k)
	{
		if (k > n)
			return 0;
		if (k > n-k)
			k = n-k;
		
		static vector<vector<lluint>> B = { { 1 },
											{ 1 },
											{ 1,  2 },
											{ 1,  3 },
											{ 1,  4,  6 },
											{ 1,  5,  10 },
											{ 1,  6,  15,  20 },
											{ 1,  7,  21,  35 },
											{ 1,  8,  28,  56,  70 },
											{ 1,  9,  36,  84,  126 },
											{ 1,  10,  45,  120,  210,  252 },
											{ 1,  11,  55,  165,  330,  462 },
											{ 1,  12,  66,  220,  495,  792,  924 },
											{ 1,  13,  78,  286,  715,  1287,  1716 },
											{ 1,  14,  91,  364,  1001,  2002,  3003,  3432 },
											{ 1,  15,  105,  455,  1365,  3003,  5005,  6435 },
											{ 1,  16,  120,  560,  1820,  4368,  8008,  11440,  12870 },
											{ 1,  17,  136,  680,  2380,  6188,  12376,  19448,  24310 },
											{ 1,  18,  153,  816,  3060,  8568,  18564,  31824,  43758,  48620 },
											{ 1,  19,  171,  969,  3876,  11628,  27132,  50388,  75582,  92378 },
											{ 1,  20,  190,  1140,  4845,  15504,  38760,  77520,  125970,  167960,  184756 },
											{ 1,  21,  210,  1330,  5985,  20349,  54264,  116280,  203490,  293930,  352716 }};
		size_t m = B.size();
		if (n < m)
			return B[n][k];
		
		B.resize(n+1,{1});
		
		for ( ; m < n+1; ++m)
		{
			auto last = (m+2)/2-1;
			B[m].resize(last+1);
			for (size_t r = 1; r < last; ++r)
			{
				B[m][r] = B[m-1][r-1]+B[m-1][r];
			}
			if (m % 2 == 0)
				B[m][last] = 2*B[m-1][last-1];
			else
				B[m][last] = B[m-1][last]+B[m-1][last-1];
		}
// 		for (int i = 0; i < B.size(); ++i)
// 			cout << i << ":  " << B[i] << endl;
		return B[n][k];
		
	}
	
	lluint catalan(lluint n)
	{
		static const vector<lluint> C = {1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786, 208012, 742900, 2674440, 9694845, 35357670, 129644790, 477638700, 1767263190, 6564120420, 24466267020, 91482563640, 343059613650, 1289904147324, 4861946401452};
		if (n < C.size())
			return C[n];
		
		return binomial(2*n,n)/(n+1); // May be useful someday
	}
	
	lluint motzkin(lluint n)
	{
		static const vector<lluint> M = {1, 1, 2, 4, 9, 21, 51, 127, 323, 835, 2188, 5798, 15511, 41835, 113634, 310572, 853467, 2356779, 6536382, 18199284, 50852019, 142547559, 400763223, 1129760415, 3192727797, 9043402501, 25669818476, 73007772802, 208023278209, 593742784829};
		if (n < M.size())
			return M[n];
		return ((2*n+1)*motzkin(n-1) + (3*n-3)*motzkin(n-2))/(n+2); // May be useful someday
	}
	
	lluint partition_number(lluint n)
	{
		static const vector<lluint> P = {1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42, 56, 77, 101, 135, 176, 231, 
		297, 385, 490, 627, 792, 1002, 1255, 1575, 1958, 2436, 3010, 3718, 4565, 5604, 6842, 8349, 
		10143, 12310, 14883, 17977, 21637, 26015, 31185, 37338, 44583, 53174, 63261, 75175, 89134, 105558, 
		124754, 147273, 173525, 204226, 239943, 281589, 329931, 386155, 451276, 526823, 614154, 715220, 
		831820, 966467, 1121505, 1300156, 1505499, 1741630, 2012558, 
		2323520, 2679689, 3087735, 3554345, 4087968, 4697205, 5392783, 6185689, 7089500, 8118264, 9289091, 
		10619863, 12132164, 13848650, 15796476, 18004327, 20506255, 23338469, 26543660, 30167357, 34262962, 
		38887673, 44108109, 49995925, 56634173, 64112359, 72533807, 82010177, 92669720, 104651419, 118114304, 
		133230930, 150198136, 169229875, 190569292, 214481126, 241265379, 271248950, 304801365, 342325709, 
		384276336, 431149389, 483502844, 541946240, 607163746, 679903203, 761002156, 851376628, 952050665, 
		1064144451, 1188908248, 1327710076, 1482074143, 1653668665, 1844349560, 2056148051, 2291320912, 2552338241, 
		2841940500, 3163127352, 3519222692, 3913864295, 4351078600, 4835271870, 5371315400, 5964539504, 6620830889, 
		7346629512, 8149040695, 9035836076, 10015581680, 11097645016, 12292341831, 13610949895, 15065878135, 16670689208, 
		18440293320, 20390982757, 22540654445, 24908858009, 27517052599, 30388671978, 33549419497, 37027355200, 40853235313, 
		45060624582, 49686288421, 54770336324, 60356673280, 66493182097, 73232243759, 80630964769, 88751778802, 97662728555, 
		107438159466, 118159068427, 129913904637, 142798995930, 156919475295, 172389800255, 189334822579, 207890420102, 
		228204732751, 250438925115, 274768617130, 301384802048, 330495499613, 362326859895, 397125074750, 435157697830, 
		476715857290, 522115831195, 571701605655, 625846753120, 684957390936, 749474411781, 819876908323, 896684817527, 
		980462880430, 1071823774337, 1171432692373, 1280011042268, 1398341745571, 1527273599625, 1667727404093, 1820701100652, 
		1987276856363, 2168627105469, 2366022741845, 2580840212973, 2814570987591, 3068829878530, 3345365983698, 3646072432125, 
		3972999029388, 4328363658647, 4714566886083, 5134205287973, 5590088317495, 6085253859260, 6622987708040, 7206841706490, 
		7840656226137, 8528581302375, 9275102575355, 10085065885767, 10963707205259, 11916681236278, 12950095925895, 14070545699287, 
		15285151248481, 16601598107914, 18028182516671, 19573856161145, 21248279009367, 23061871173849, 25025873760111, 
		27152408925615, 29454549941750, 31946390696157, 34643126322519, 37561133582570, 40718063627362, 44132934884255, 
		47826239745920,	51820051838712, 56138148670947, 60806135438329, 65851585970275, 71304185514919, 77195892663512, 
		83561103925871, 90436839668817, 97862933703585, 105882246722733, 114540884553038, 123888443077259, 
		133978259344888, 144867692496445, 156618412527946, 169296722391554, 182973889854026, 197726516681672, 
		213636919820625, 230793554364681, 249291451168559, 269232701252579, 290726957916112, 313891991306665, 
		338854264248680, 365749566870782, 394723676655357, 425933084409356, 459545750448675, 495741934760846, 
		534715062908609, 576672674947168, 621837416509615, 670448123060170, 722760953690372, 779050629562167, 
		839611730366814, 904760108316360, 974834369944625, 1050197489931117, 1131238503938606, 1218374349844333, 
		1312051800816215, 1412749565173450, 1520980492851175, 1637293969337171, 1762278433057269, 1896564103591584, 
		2040825852575075, 2195786311682516, 2362219145337711, 2540952590045698, 2732873183547535, 2938929793929555, 
		3160137867148997, 3397584011986773, 3652430836071053, 3925922161489422, 4219388528587095, 4534253126900886, 
		4872038056472084, 5234371069753672, 5622992691950605, 6039763882095515, 6486674127079088, 6965850144195831, 
		7479565078510584, 8030248384943040, 8620496275465025, 9253082936723602, 9930972392403501, 10657331232548839, 
		11435542077822104, 12269218019229465, 13162217895057704, 14118662665280005, 15142952738857194, 
		16239786535829663, 17414180133147295, 18671488299600364, 20017426762576945, 21458096037352891, 
		23000006655487337, 24650106150830490, 26415807633566326, 28305020340996003, 30326181989842964, 
		32488293351466654, 34800954869440830, 37274405776748077, 39919565526999991, 42748078035954696, 
		45772358543578028, 49005643635237875, 52462044228828641, 56156602112874289, 
		60105349839666544, 64325374609114550, 68834885946073850, 73653287861850339, 78801255302666615, 
		84300815636225119, 90175434980549623, 96450110192202760, 103151466321735325, 110307860425292772, 
		117949491546113972, 126108517833796355, 134819180623301520, 144117936527873832, 154043597379576030, 
		164637479165761044, 175943559810422753, 188008647052292980, 200882556287683159, 214618299743286299, 
		229272286871217150, 244904537455382406, 261578907351144125, 279363328483702152, 298330063062758076, 
		318555973788329084, 340122810048577428, 363117512048110005, 387632532919029223, 413766180933342362, 
		441622981929358437, 471314064268398780, 502957566506000020, 536679070310691121, 572612058898037559, 
		610898403751884101, 651688879997206959, 695143713458946040, 741433159884081684, 790738119649411319, 
		843250788562528427, 899175348396088349, 958728697912338045, 1022141228367345362, 1089657644424399782, 
		1161537834849962850, 1238057794119125085, 1319510599727473500, 1406207446561484054, 1498478743590581081, 
		1596675274490756791, 1701169427975813525, 1812356499739472950, 1930656072350465812, 2056513475336633805, 
		2190401332423765131, 2332821198543892336, 2484305294265418180, 2645418340688763701, 2816759503217942792, 
		2998964447736452194, 3192707518433532826, 3398704041358160275, 3617712763867604423, 3850538434667429186, 
		4098034535626594791, 4361106170762284114, 4640713124699623515, 4937873096788191655, 5253665124416975163, 
		5589233202595404488, 5945790114707874597, 6324621482504294325, 6727090051741041926, 7154640222653942321, 
		7608802843339879269, 8091200276484465581, 8603551759348655060, 9147679068859117602};
		
		return P[n];
	}
	
	lluint partition_number(lluint n, lluint k)
	{
		static vector<vector<lluint>> PNK = 
			{	{1},
				{0, 1},
				{0, 1, 1},
				{0, 1, 1, 1},
				{0, 1, 2, 1, 1},
				{0, 1, 2, 2, 1, 1},
				{0, 1, 3, 3, 2, 1, 1},
				{0, 1, 3, 4, 3, 2, 1, 1},
				{0, 1, 4, 5, 5, 3, 2, 1, 1},
				{0, 1, 4, 7, 6, 5, 3, 2, 1, 1},
				{0, 1, 5, 8, 9, 7, 5, 3, 2, 1, 1},
				{0, 1, 5, 10, 11, 10, 7, 5, 3, 2, 1, 1},
				{0, 1, 6, 12, 15, 13, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 6, 14, 18, 18, 14, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 7, 16, 23, 23, 20, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 7, 19, 27, 30, 26, 21, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 8, 21, 34, 37, 35, 28, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 8, 24, 39, 47, 44, 38, 29, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 9, 27, 47, 57, 58, 49, 40, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 9, 30, 54, 70, 71, 65, 52, 41, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 10, 33, 64, 84, 90, 82, 70, 54, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 10, 37, 72, 101, 110, 105, 89, 73, 55, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 11, 40, 84, 119, 136, 131, 116, 94, 75, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 11, 44, 94, 141, 163, 164, 146, 123, 97, 76, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 12, 48, 108, 164, 199, 201, 186, 157, 128, 99, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 12, 52, 120, 192, 235, 248, 230, 201, 164, 131, 100, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 13, 56, 136, 221, 282, 300, 288, 252, 212, 169, 133, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 13, 61, 150, 255, 331, 364, 352, 318, 267, 219, 172, 134, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 14, 65, 169, 291, 391, 436, 434, 393, 340, 278, 224, 174, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 14, 70, 185, 333, 454, 522, 525, 488, 423, 355, 285, 227, 175, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 15, 75, 206, 377, 532, 618, 638, 598, 530, 445, 366, 290, 229, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 15, 80, 225, 427, 612, 733, 764, 732, 653, 560, 460, 373, 293, 230, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 16, 85, 249, 480, 709, 860, 919, 887, 807, 695, 582, 471, 378, 295, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 16, 91, 270, 540, 811, 1009, 1090, 1076, 984, 863, 725, 597, 478, 381, 296, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 17, 96, 297, 603, 931, 1175, 1297, 1291, 1204, 1060, 905, 747, 608, 483, 383, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 17, 102, 321, 674, 1057, 1367, 1527, 1549, 1455, 1303, 1116, 935, 762, 615, 486, 384, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 18, 108, 351, 748, 1206, 1579, 1801, 1845, 1761, 1586, 1380, 1158, 957, 773, 620, 488, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 18, 114, 378, 831, 1360, 1824, 2104, 2194, 2112, 1930, 1686, 1436, 1188, 972, 780, 623, 489, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 19, 120, 411, 918, 1540, 2093, 2462, 2592, 2534, 2331, 2063, 1763, 1478, 1210, 983, 785, 625, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 19, 127, 441, 1014, 1729, 2400, 2857, 3060, 3015, 2812, 2503, 2164, 1819, 1508, 1225, 990, 788, 626, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 20, 133, 478, 1115, 1945, 2738, 3319, 3589, 3590, 3370, 3036, 2637, 2241, 1861, 1530, 1236, 995, 790, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 20, 140, 511, 1226, 2172, 3120, 3828, 4206, 4242, 4035, 3655, 3210, 2738, 2297, 1891, 1545, 1243, 998, 791, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 21, 147, 551, 1342, 2432, 3539, 4417, 4904, 5013, 4802, 4401, 3882, 3345, 2815, 2339, 1913, 1556, 1248, 1000, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 21, 154, 588, 1469, 2702, 4011, 5066, 5708, 5888, 5708, 5262, 4691, 4057, 3446, 2871, 2369, 1928, 1563, 1251, 1001, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 22, 161, 632, 1602, 3009, 4526, 5812, 6615, 6912, 6751, 6290, 5635, 4920, 4192, 3523, 2913, 2391, 1939, 1568, 1253, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 22, 169, 672, 1747, 3331, 5102, 6630, 7657, 8070, 7972, 7476, 6761, 5928, 5096, 4293, 3579, 2943, 2406, 1946, 1571, 1254, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 23, 176, 720, 1898, 3692, 5731, 7564, 8824, 9418, 9373, 8877, 8073, 7139, 6158, 5231, 4370, 3621, 2965, 2417, 1951, 1573, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 23, 184, 764, 2062, 4070, 6430, 8588, 10156, 10936, 11004, 10489, 9624, 8551, 7434, 6334, 5332, 4426, 3651, 2980, 2424, 1954, 1574, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 24, 192, 816, 2233, 4494, 7190, 9749, 11648, 12690, 12866, 12384, 11424, 10232, 8932, 7665, 6469, 5409, 4468, 3673, 2991, 2429, 1956, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 24, 200, 864, 2418, 4935, 8033, 11018, 13338, 14663, 15021, 14552, 13542, 12186, 10715, 9228, 7841, 6570, 5465, 4498, 3688, 2998, 2432, 1957, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 25, 208, 920, 2611, 5427, 8946, 12450, 15224, 16928, 17475, 17084, 15988, 14499, 12801, 11098, 9459, 7976, 6647, 5507, 4520, 3699, 3003, 2434, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 25, 217, 972, 2818, 5942, 9953, 14012, 17354, 19466, 20298, 19978, 18847, 17176, 15272, 13287, 11395, 9635, 8077, 6703, 5537, 4535, 3706, 3006, 2435, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 26, 225, 1033, 3034, 6510, 11044, 15765, 19720, 22367, 23501, 23334, 22142, 20325, 18148, 15892, 13671, 11626, 9770, 8154, 6745, 5559, 4546, 3711, 3008, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 26, 234, 1089, 3266, 7104, 12241, 17674, 22380, 25608, 27169, 27156, 25971, 23961, 21535, 18928, 16380, 13968, 11802, 9871, 8210, 6775, 5574, 4553, 3714, 3009, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 27, 243, 1154, 3507, 7760, 13534, 19805, 25331, 29292, 31316, 31570, 30366, 28212, 25469, 22518, 19551, 16765, 14199, 11937, 9948, 8252, 6797, 5585, 4558, 3716, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 27, 252, 1215, 3765, 8442, 14950, 22122, 28629, 33401, 36043, 36578, 35452, 33104, 30073, 26694, 23303, 20040, 17062, 14375, 12038, 10004, 8282, 6812, 5592, 4561, 3717, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 28, 261, 1285, 4033, 9192, 16475, 24699, 32278, 38047, 41373, 42333, 41269, 38797, 35401, 31603, 27684, 23928, 20425, 17293, 14510, 12115, 10046, 8304, 6823, 5597, 4563, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 28, 271, 1350, 4319, 9975, 18138, 27493, 36347, 43214, 47420, 48849, 47968, 45326, 41612, 37292, 32839, 28472, 24418, 20722, 17469, 14611, 12171, 10076, 8319, 6830, 5600, 4564, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 29, 280, 1425, 4616, 10829, 19928, 30588, 40831, 49037, 54218, 56297, 55610, 52888, 48772, 43951, 38837, 33834, 29098, 24803, 20953, 17604, 14688, 12213, 10098, 8330, 6835, 5602, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 29, 290, 1495, 4932, 11720, 21873, 33940, 45812, 55494, 61903, 64707, 64370, 61538, 57080, 51643, 45864, 40080, 34624, 29588, 25100, 21129, 17705, 14744, 12243, 10113, 8337, 6838, 5603, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 30, 300, 1575, 5260, 12692, 23961, 37638, 51294, 62740, 70515, 74287, 74331, 71509, 66634, 60603, 54012, 47420, 41078, 35251, 29973, 25331, 21264, 17782, 14786, 12265, 10124, 8342, 6840, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 30, 310, 1650, 5608, 13702, 26226, 41635, 57358, 70760, 80215, 85067, 85711, 82882, 77667, 70927, 63516, 55940, 48668, 41869, 35741, 30270, 25507, 21365, 17838, 14816, 12280, 10131, 8345, 6841, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 31, 320, 1735, 5969, 14800, 28652, 46031, 64015, 79725, 91058, 97299, 98609, 95943, 90316, 82898, 74506, 65907, 57503, 49668, 42496, 36126, 30501, 25642, 21442, 17880, 14838, 12291, 10136, 8347, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 31, 331, 1815, 6351, 15944, 31275, 50774, 71362, 89623, 103226, 111036, 113287, 110795, 104875, 96650, 87268, 77449, 67846, 58754, 50460, 42986, 36423, 30677, 25743, 21498, 17910, 14853, 12298, 10139, 8348, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 32, 341, 1906, 6747, 17180, 34082, 55974, 79403, 100654, 116792, 126560, 129883, 127786, 121510, 112540, 101982, 90889, 79855, 69414, 59755, 51087, 43371, 36654, 30812, 25820, 21540, 17932, 14864, 12303, 10141, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 32, 352, 1991, 7166, 18467, 37108, 61575, 88252, 112804, 131970, 143948, 148702, 147059, 140587, 130738, 119009, 106408, 93854, 81801, 70667, 60547, 51577, 43668, 36830, 30913, 25876, 21570, 17947, 14871, 12306, 10142, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 33, 363, 2087, 7599, 19858, 40340, 67696, 97922, 126299, 148847, 163540, 169919, 169027, 162331, 151685, 138579, 124418, 110059, 96271, 83372, 71669, 61174, 51962, 43899, 36965, 30990, 25918, 21592, 17958, 14876, 12308, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 33, 374, 2178, 8056, 21301, 43819, 74280, 108527, 141136, 167672, 185425, 193906, 193880, 187175, 175618, 161144, 145149, 128886, 113039, 98222, 84626, 72461, 61664, 52259, 44075, 37066, 31046, 25948, 21607, 17965, 14879, 12309, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 34, 385, 2280, 8529, 22856, 47527, 81457, 120092, 157564, 188556, 210005, 220877, 222118, 215415, 203067, 187013, 169120, 150614, 132559, 115463, 99795, 85628, 73088, 62049, 52490, 44210, 37143, 31088, 25970, 21618, 17970, 14881, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 34, 397, 2376, 9027, 24473, 51508, 89162, 132751, 175586, 211782, 237405, 251274, 253981, 247587, 234343, 216738, 196648, 175767, 155112, 135550, 117417, 101050, 86420, 73578, 62346, 52666, 44311, 37199, 31118, 25985, 21625, 17973, 14882, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 35, 408, 2484, 9542, 26207, 55748, 97539, 146520, 195491, 237489, 268079, 285373, 290071, 284054, 270105, 250723, 228364, 204725, 181274, 158800, 137979, 118991, 102052, 87047, 73963, 62577, 52801, 44388, 37241, 31140, 25996, 21630, 17975, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 35, 420, 2586, 10083, 28009, 60289, 106522, 161554, 217280, 266006, 302196, 323689, 330699, 325472, 310748, 289656, 264691, 238134, 211428, 185794, 161798, 139935, 120246, 102844, 87537, 74260, 62753, 52902, 44444, 37271, 31155, 26003, 21633, 17976, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 36, 432, 2700, 10642, 29941, 65117, 116263, 177884, 241279, 297495, 340293, 366566, 376577, 372311, 357075, 334051, 306421, 276493, 246288, 216965, 189493, 164230, 141510, 121248, 103471, 87922, 74491, 62888, 52979, 44486, 37293, 31166, 26008, 21635, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 36, 444, 2808, 11229, 31943, 70281, 126692, 195666, 267507, 332337, 382562, 414624, 428104, 425349, 409603, 384759, 354091, 320620, 286364, 253033, 221500, 192496, 166187, 142765, 122040, 103961, 88219, 74667, 62989, 53035, 44516, 37308, 31173, 26011, 21636, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 37, 456, 2928, 11835, 34085, 75762, 137977, 214944, 296320, 370733, 429636, 468273, 486133, 485184, 469300, 442442, 408687, 371153, 332557, 294574, 258592, 225206, 194930, 167762, 143767, 122667, 104346, 88450, 74802, 63066, 53077, 44538, 37319, 31178, 26013, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 37, 469, 3042, 12470, 36308, 81612, 150042, 235899, 327748, 413112, 481769, 528245, 551155, 552767, 536827, 508137, 470914, 429112, 385528, 342505, 301349, 263138, 228212, 196888, 169017, 144559, 123157, 104643, 88626, 74903, 63122, 53107, 44553, 37326, 31181, 26014, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 38, 481, 3169, 13125, 38677, 87816, 163069, 258569, 362198, 459718, 539672, 595056, 624188, 628822, 613370, 582691, 541971, 495332, 446405, 397566, 350757, 306923, 266849, 230647, 198463, 170019, 145186, 123542, 104874, 88761, 74980, 63164, 53129, 44564, 37331, 31183, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 38, 494, 3289, 13811, 41134, 94425, 176978, 283161, 399705, 511045, 603666, 669555, 705851, 714504, 699749, 667382, 622771, 571069, 516054, 460915, 407570, 357554, 311476, 269857, 232605, 199718, 170811, 145676, 123839, 105050, 88862, 75036, 63194, 53144, 44571, 37334, 31184, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 39, 507, 3422, 14518, 43752, 101423, 191964, 309729, 440725, 567377, 674585, 752368, 797341, 810726, 797402, 763265, 714802, 657395, 595872, 533523, 473030, 415852, 363139, 315190, 272293, 234180, 200720, 171438, 146061, 124070, 105185, 88939, 75078, 63216, 53155, 44576, 37336, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 39, 520, 3549, 15257, 46461, 108869, 207945, 338484, 485315, 629281, 752802, 844504, 899427, 918851, 907376, 871908, 819205, 755880, 686983, 616825, 548134, 483076, 422664, 367697, 318199, 274251, 235435, 201512, 171928, 146358, 124246, 105286, 88995, 75108, 63231, 53162, 44579, 37337, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 40, 533, 3689, 16019, 49342, 116742, 225132, 369499, 533975, 697097, 839286, 946708, 1013531, 1040014, 1031391, 994644, 937815, 867873, 791131, 712083, 634429, 560305, 491380, 428256, 371413, 320635, 275826, 236437, 202139, 172313, 146589, 124381, 105363, 89037, 75130, 63242, 53167, 44581, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 40, 547, 3822, 16814, 52327, 125104, 243434, 403016, 586779, 771464, 934502, 1060163, 1140588, 1175862, 1170752, 1133373, 1072093, 995318, 909742, 821104, 733212, 649117, 570381, 498203, 432817, 374423, 322593, 277081, 237229, 202629, 172610, 146765, 124482, 105419, 89067, 75145, 63249, 53170, 44582, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 41, 560, 3969, 17633, 55491, 133939, 263081, 439100, 644295, 852785, 1039543, 1185776, 1282281, 1327763, 1327547, 1289761, 1224262, 1139939, 1044986, 945483, 846435, 750917, 661330, 578700, 503800, 436534, 376859, 324168, 278083, 237856, 203014, 172841, 146900, 124559, 105461, 89089, 75156, 63254, 53172, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 41, 574, 4109, 18487, 58767, 143307, 283981, 478025, 706607, 941790, 1154981, 1324916, 1439757, 1497696, 1503381, 1466126, 1396169, 1304117, 1198693, 1087482, 975753, 867699, 765661, 671428, 585530, 508363, 439544, 378817, 325423, 278875, 238346, 203311, 173017, 147001, 124615, 105491, 89104, 75163, 63257, 53173, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 42, 588, 4263, 19366, 62239, 153192, 306376, 519880, 774345, 1038944, 1282083, 1478670, 1614987, 1687344, 1700763, 1664525, 1590544, 1490023, 1373531, 1249153, 1123608, 1001260, 885481, 777904, 679758, 591130, 512081, 441980, 380392, 326425, 279502, 238731, 203542, 173152, 147078, 124657, 105513, 89115, 75168, 63259, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 42, 602, 4410, 20282, 65827, 163662, 330170, 564945, 847628, 1145078, 1421506, 1648649, 1809369, 1899041, 1921687, 1887776, 1809674, 1700603, 1571824, 1433286, 1292139, 1154109, 1022625, 900267, 788017, 686593, 595694, 515091, 443938, 381647, 327217, 279992, 239028, 203718, 173253, 147134, 124687, 105528, 89122, 75171, 63260, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 43, 616, 4571, 21224, 69624, 174696, 355626, 613331, 927143, 1260740, 1574714, 1836130, 2025226, 2134841, 2169146, 2138425, 2056896, 1938560, 1796874, 1642491, 1484373, 1328562, 1179751, 1040463, 912532, 796354, 692195, 599412, 517527, 445513, 382649, 327844, 280377, 239259, 203853, 173330, 147176, 124709, 105539, 89127, 75173, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 43, 631, 4725, 22204, 73551, 186366, 382641, 665355, 1013036, 1386861, 1742509, 2042987, 2264234, 2397537, 2445589, 2419869, 2335073, 2207510, 2051599, 1880246, 1703038, 1527744, 1359239, 1201193, 1055279, 922656, 803192, 696760, 602422, 519485, 446768, 383441, 328334, 280674, 239435, 203954, 173386, 147206, 124724, 105546, 89130, 75174, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 44, 645, 4894, 23212, 77695, 198655, 411498, 721125, 1106080, 1524081, 1926533, 2270754, 2529120, 2689583, 2754612, 2735245, 2648233, 2510840, 2340069, 2149821, 1951915, 1754615, 1564398, 1384982, 1219073, 1067559, 930998, 808795, 700478, 604858, 521060, 447770, 384068, 328719, 280905, 239570, 204031, 173428, 147228, 124735, 105551, 89132, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 44, 660, 5055, 24260, 81979, 211634, 442089, 780997, 1206440, 1673457, 2127747, 2521589, 2821909, 3014304, 3099186, 3088663, 2999936, 2852958, 2665952, 2455532, 2234447, 2013089, 1798283, 1595210, 1406480, 1233911, 1077690, 937838, 813360, 703488, 606816, 522315, 448562, 384558, 329016, 281081, 239671, 204087, 173458, 147243, 124742, 105554, 89133, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 45, 675, 5231, 25337, 86499, 225286, 474715, 845105, 1314972, 1835721, 2348042, 2797302, 3145777, 3374676, 3483604, 3483945, 3395084, 3238070, 3034232, 2801502, 2555327, 2306908, 2065051, 1835113, 1621030, 1424390, 1246202, 1086035, 943442, 817078, 705924, 608391, 523317, 449189, 384943, 329247, 281216, 239748, 204129, 173480, 147254, 124747, 105556, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 45, 690, 5400, 26455, 91164, 239691, 509267, 913815, 1431884, 2012069, 2588523, 3100410, 3503153, 3774599, 3911503, 3926046, 3838036, 3671577, 3449498, 3193032, 2918919, 2640955, 2368572, 2108950, 1866026, 1642570, 1439243, 1256338, 1092876, 948007, 820088, 707882, 609646, 524109, 449679, 385240, 329423, 281317, 239804, 204159, 173495, 147261, 124750, 105557, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 46, 705, 5584, 27604, 96079, 254826, 546067, 987292, 1558110, 2203348, 2851355, 3433027, 3897751, 4217657, 4387969, 4419640, 4334733, 4158656, 3917865, 3635292, 3331011, 3019969, 2714043, 2420831, 2145915, 1891902, 1660502, 1451541, 1264685, 1098480, 951725, 822524, 709457, 610648, 524736, 450064, 385471, 329558, 281394, 239846, 204181, 173506, 147266, 124752, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 46, 721, 5760, 28796, 101155, 270775, 584996, 1065947, 1693899, 2410895, 3137850, 3798063, 4332454, 4708477, 4917406, 4970660, 4890554, 4705886, 4445020, 4134830, 3797090, 3450002, 3106389, 2776092, 2464906, 2176905, 1913472, 1675366, 1461680, 1271527, 1103045, 954735, 824482, 710712, 611440, 525226, 450361, 385647, 329659, 281450, 239876, 204196, 173513, 147269, 124753, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 47, 736, 5952, 30020, 106491, 287521, 626401, 1149941, 1840292, 2635689, 3450438, 4198013, 4811594, 5251305, 5505879, 5584788, 5512631, 5319666, 5038443, 4698053, 4324323, 3937025, 3552054, 3179967, 2828582, 2501972, 2202823, 1931419, 1687669, 1470028, 1277131, 1106763, 957171, 826057, 711714, 612067, 525611, 450592, 385782, 329736, 281492, 239898, 204207, 173518, 147271, 124754, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 47, 752, 6136, 31289, 111999, 305146, 670162, 1239732, 1997569, 2879236, 3790670, 4636214, 5338601, 5851608, 6158681, 6269144, 6207559, 6007963, 5705194, 5333017, 4919553, 4488553, 4057271, 3639101, 3242313, 2872792, 2533018, 2224415, 1946290, 1697810, 1476870, 1281696, 1109773, 959129, 827312, 712506, 612557, 525908, 450768, 385883, 329792, 281522, 239913, 204214, 173521, 147272, 124754, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 48, 768, 6336, 32591, 117788, 323633, 716644, 1335517, 2166875, 3142647, 4161319, 5115586, 5918495, 6514463, 6882999, 7030589, 6983946, 6778628, 6454368, 6047699, 5591609, 5112049, 4630063, 4160115, 3713064, 3294979, 2909935, 2558966, 2242373, 1958596, 1706159, 1482474, 1285414, 1112209, 960704, 828314, 713133, 612942, 526139, 450903, 385960, 329834, 281544, 239924, 204219, 173523, 147273, 124754, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 48, 784, 6528, 33940, 123755, 343074, 765722, 1437769, 2348553, 3427615, 4564153, 5639989, 6555343, 7246258, 7685215, 7877643, 7849794, 7641341, 7294682, 6851934, 6349048, 5816815, 5278236, 4751311, 4247652, 3775641, 3339290, 2941023, 2580573, 2257249, 1968738, 1713001, 1487039, 1288424, 1114167, 961959, 829106, 713623, 613239, 526315, 451004, 386016, 329864, 281559, 239931, 204222, 173524, 147273, 124754, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 49, 800, 6736, 35324, 130019, 363446, 817789, 1546719, 2543849, 3735414, 5002329, 6212802, 7254976, 8053039, 8573805, 8818588, 8815458, 8605674, 8237213, 7755597, 7202691, 6612186, 6011745, 5421001, 4854782, 4321912, 3828442, 3376489, 2966993, 2598538, 2269557, 1977087, 1718605, 1490757, 1290860, 1115742, 962961, 829733, 714008, 613470, 526450, 451081, 386058, 329886, 281570, 239936, 204224, 173525, 147273, 124754, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1},
				{0, 1, 49, 817, 6936, 36756, 136479, 384845, 872713, 1662894, 2753159, 4067930, 5477923, 6838459, 8022171, 8942320, 9556420, 9863566, 9890681, 9683331, 9292657, 8770736, 8163167, 7509614, 6840398, 6179507, 5543041, 4942704, 4384665, 3872830, 3407607, 2988611, 2613417, 2279700, 1983929, 1723170, 1493767, 1292818, 1116997, 963753, 830223, 714305, 613646, 526551, 451137, 386088, 329901, 281577, 239939, 204225, 173525, 147273, 124754, 105558, 89134, 75175, 63261, 53174, 44583, 37338, 31185, 26015, 21637, 17977, 14883, 12310, 10143, 8349, 6842, 5604, 4565, 3718, 3010, 2436, 1958, 1575, 1255, 1002, 792, 627, 490, 385, 297, 231, 176, 135, 101, 77, 56, 42, 30, 22, 15, 11, 7, 5, 3, 2, 1, 1}};
		assert(n < PNK.size());
		return PNK[n][k];
	}
	
	lluint stirling1(lluint n,lluint k)
	{
		// TODO
		return 0;
	}
	
	llint stirling2(lluint n,lluint k)
	{
		llint toReturn = 0;
		for (llint i = 0; i < k+1; ++i)
		{
			auto toadd = binomial(k,i)*pow((k-i),n);
			if (i % 2 == 0)
				toReturn += toadd;
			else
				toReturn -= toadd;
		}
		return toReturn/factorial(k);
	}

	
}