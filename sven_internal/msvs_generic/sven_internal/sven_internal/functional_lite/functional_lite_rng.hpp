/**
 * Copyright - xWhitey, 2023-2024.
 * functional_lite_rng.hpp - Our own weird implementation of a Random Number Generator.
 *
 * functional_lite (Various functional implementations (like stb_*)) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef FUNCTIONAL_LITE_RNG_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in functional_lite_rng.hpp
#else //FUNCTIONAL_LITE_RNG_HPP_RECURSE_GUARD

#define FUNCTIONAL_LITE_RNG_HPP_RECURSE_GUARD

#ifndef FUNCTIONAL_LITE_RNG_HPP_GUARD
#define FUNCTIONAL_LITE_RNG_HPP_GUARD
#pragma once

#ifdef __cplusplus

#ifdef Q_ARRAYSIZE
#undef Q_ARRAYSIZE
#endif //Q_ARRAYSIZE
#define Q_ARRAYSIZE(_Array) (sizeof(_Array) / sizeof(_Array[0]))

#define Q_RAND_MAX 32767

#define Q_labsi(_Value) (_Value < 0 ? _Value * -1 : _Value)

typedef struct CTrustedRandom {
	CTrustedRandom() {
		_m_iSeed = _m_iInvocationCounter = 0;
		_m_p_iSeedTable = nullptr;
	}

	~CTrustedRandom() {
		if (_m_p_iSeedTable != nullptr) {
			Q_free(_m_p_iSeedTable);
		}
	}

	void FeedRandWithTime(long long _Time) {
		typedef union LargeInteger {
			struct {
				unsigned long m_dwLowPart;
				long m_lHighPart;
			} Parts;
			long long m_ullQuadPart;
		} LargeInteger;

		LargeInteger largeInteger;
		largeInteger.m_ullQuadPart = _Time;
		_m_iSeed = Rand(largeInteger.Parts.m_dwLowPart, largeInteger.Parts.m_lHighPart);
	}

	void Seed(int _Position) {
		_Position = Q_labsi(_Position);

		while (_Position > Q_ARRAYSIZE(_m_a_iDefaultSeedTable) - 1) {
			_Position = Q_labsi(Q_ARRAYSIZE(_m_a_iDefaultSeedTable) - 1 - _Position);
		}

		if (_m_p_iSeedTable == nullptr) {
			_m_iSeed = _m_a_iDefaultSeedTable[_Position];
		}
		else {
			_m_iSeed = _m_p_iSeedTable[_Position];
		}
	}

	void GenerateNewSeedTable() {
		if (!_m_p_iSeedTable) {
			_m_p_iSeedTable = reinterpret_cast<int*>(Q_malloc(sizeof(int) * Q_ARRAYSIZE(_m_a_iDefaultSeedTable)));
		}

		for (int idx = 0; idx < Q_ARRAYSIZE(_m_a_iDefaultSeedTable); idx++) {
			_m_p_iSeedTable[idx] = Rand(0, 70000);
		}

		_m_iSeed = GenerateSeed();
	}

	int SeedRand(int _Seed, int _Min, int _Max) {
		return (Worker(_Seed, 0, Q_RAND_MAX) % Worker(_Seed + 1, Q_RAND_MAX / 2, Q_RAND_MAX) + Worker(_Seed / 2, _Min, _Max) + Worker(_m_iInvocationCounter, _Min, _Max)) % (_Max + 1 - _Min) + _Min;
	}

	int Rand(int _Min, int _Max) {
		int iTheSeed = SeedRand(_m_iInvocationCounter, 0, Q_ARRAYSIZE(_m_a_iDefaultSeedTable) - 1);
		int iSecondSeed = SeedRand(iTheSeed, 0, Q_ARRAYSIZE(_m_a_iDefaultSeedTable) - 1);
		return SeedRand(iTheSeed ^ iSecondSeed, _Min, _Max);
	}
private:
	int GenerateSeed() {
		//_m_iSeed *= (_m_iSeed % 2 == 0 ? 2 : 4);
		if (_m_p_iSeedTable == nullptr) {
			_m_iSeed += _m_a_iDefaultSeedTable[_m_iSeed & 0xFF] + _m_iInvocationCounter;
		}
		else {
			_m_iSeed += _m_p_iSeedTable[_m_iSeed & 0xFF] + _m_iInvocationCounter;
		}

		return _m_iSeed & 0x7FFFFFFF;
	}

	int Worker(int _Seed, int _Min, int _Max) {
		if (_m_iInvocationCounter >= Q_ARRAYSIZE(_m_a_iDefaultSeedTable) - 1) {
			_m_iInvocationCounter = 0;
		}

		_m_iInvocationCounter++;

		Seed(_Seed - (_m_iSeed & 0xFF) + 1);

		if (_Max == _Min)
			return _Min;

		return GenerateSeed() % (_Max + 1 - _Min) + _Min;
	}

	int _m_iSeed, _m_iInvocationCounter;

	//Hardcoded to be with capacity of Q_ARRAYSIZE(_m_a_iDefaultSeedTable) integers!!!
	int* _m_p_iSeedTable = nullptr;

	const static int _m_a_iDefaultSeedTable[1024];
} CTrustedRandom;

inline const int CTrustedRandom::_m_a_iDefaultSeedTable[1024] = {
	9883, 27502, 13369, 10559, 23661, 19154, 27586, 30916, 9657, 30876, 12698, 29917, 5843, 2659, 3405, 28142, 18780, 21198, 26677, 17408, 2316, 27467, 16469, 24084, 15791, 1540, 645, 11191, 13934, 7183, 20432, 25815, 14928, 19428, 12826, 31701, 513, 31765, 1029, 30171, 32465, 5689, 4118, 7536, 4863, 28201, 11028, 17226, 7992, 10608, 19889, 355, 2395, 15100, 29037, 13391, 8319, 18202, 13494, 7884, 567, 929, 2644, 27547, 25877, 26732, 30724, 398, 27756, 17422, 10339, 24035, 26970, 2242, 21518, 26832, 11526, 26017, 17928, 30815, 20519, 24, 32604, 30076, 5062, 24984, 15875, 22348, 16196, 28047, 27399, 26817, 8094, 29592, 13289, 3180, 3056, 12782, 21313, 29330, 4920, 23591, 19620, 30081, 14173, 31263, 8138, 4132, 22896, 13106, 14064, 8918, 15153, 22958, 5244, 31348, 14805, 19079, 992, 19894, 9734, 30029, 20928, 9407, 23852, 8284, 20311, 24306, 2027, 15699, 9495, 19208, 8684, 25031, 14479, 1375, 12001, 14392, 10642, 4245, 16579, 32067, 10902, 23638, 13150, 23834, 14426, 11890, 16656, 17714, 29900, 8755, 21710, 32315, 10537, 12781, 15719, 8789, 13158, 32470, 9741, 17281, 25295, 19224, 30737, 22101, 2030, 23077, 2542, 13557, 15278, 11083, 16265, 20030, 8640, 17032, 32695, 21828, 10814, 29599, 8960, 1279, 5774, 25451, 16291, 19521, 17802, 13172, 30425, 30187, 2781, 3008, 10078, 8545, 5552, 30444, 10542, 7995, 10887, 17531, 30348, 1598, 13413, 25965, 26185, 5258, 28983, 30483, 23363, 12564, 15727, 28707, 3372, 8535, 17136, 9224, 20109, 18247, 3882, 4219, 29706, 31623, 21638, 10583, 16615, 22491, 23300, 5801, 31947, 1504, 25634, 6407, 5828, 26107, 1537, 2084, 31116, 8847, 16835, 8097, 29963, 883, 22839, 3621, 24860, 5539, 10168, 31407, 23956, 31541, 594, 29739, 21377, 30607, 25383, 32580, 7087, 3032, 25617, 13946, 28273, 14252, 19593, 12404, 157, 12820, 14024, 8343, 29145, 11957, 4139, 26071, 28693, 839, 30940, 31693, 13401, 16713, 18095, 522, 15553, 6131, 12106, 19884, 26566, 4717, 23945, 14639, 21694, 1598, 30241, 9861, 11686, 17927, 20355, 26472, 15904, 27761, 24835, 32751, 31672, 21348, 11577, 30559, 25451, 2938, 11175, 11076, 20586, 26547, 30109, 22721, 12315, 29034, 14938, 20517, 14177, 22663, 15988, 1726, 32029, 13303, 27258, 31474, 7669, 9817, 11479, 7819, 19796, 15098, 13049, 15187, 30641, 3844, 15795, 1505, 23061, 6898, 5344, 21170, 13015, 12359, 24138, 8572, 28586, 32640, 23498, 29374, 31963, 14919, 21680, 7068, 6611, 9705, 11876, 3050, 22243, 12810, 30798, 14983, 8257, 26520, 28404, 11244, 31309, 17405, 14946, 18180, 21273, 21657, 27599, 27711, 3369, 9839, 32381, 738, 10103, 24094, 24545, 28833, 9914, 3092, 17156, 12143, 9192, 4350, 17193, 30101, 9210, 9858, 6576, 9217, 7407, 24625, 27450, 28385, 19475, 14519, 28686, 29809, 16769, 32597, 31676, 13841, 17639, 14738, 10240, 10139, 30601, 19757, 9960, 18675, 22409, 1621, 57, 32224, 2149, 19996, 15888, 6170, 1217, 9514, 19653, 18340, 7527, 13998, 4628, 25947, 27020, 6043, 32639, 19408, 3157, 1552, 9198, 12883, 29593, 15639, 153, 15952, 29673, 12435, 9511, 16095, 26061, 17780, 22295, 18070, 10321, 23085, 18528, 6049, 11456, 13160, 8532, 5653, 28592, 8440, 25034, 13442, 830, 28431, 23932, 3784, 3739, 30641, 22446, 10027, 26312, 26728, 529, 31145, 9373, 4154, 25318, 22922, 23568, 18904, 26214, 19770, 22708, 22040, 8034, 23636, 896, 15013, 31527, 1023, 21970, 30965, 27812, 17789, 32179, 12668, 21199, 769, 5981, 1824, 7748, 7873, 8187, 22252, 31279, 4173, 1455, 6152, 31853, 25631, 30937, 30697, 8430, 22899, 25239, 1733, 5214, 4697, 9352, 2122, 4612, 2568, 1494, 15503, 22762, 17781, 29043, 13643, 31806, 29220, 27042, 2885, 4856, 26430, 21690, 2059, 2749, 15589, 1830, 6119, 8707, 28100, 11953, 6159, 19831, 14058, 769, 21788, 3227, 10712, 24416, 19901, 29108, 9432, 10691, 3501, 24803, 9617, 11012, 13538, 19090, 10713, 15475, 32520, 25873, 4810, 28301, 24365, 10625, 15896, 3339, 958, 6130, 18849, 9247, 22194, 1736, 13638, 27386, 19973, 355, 22266, 14225, 8296, 31231, 21784, 24481, 12855, 21208, 6906, 10992, 20139, 658, 31687, 15063, 337, 22489, 30604, 2813, 9146, 8261, 22159, 28517, 29556, 13952, 3808, 13227, 17466, 15274, 10288, 16266, 8808, 32764, 4582, 25762, 31154, 24236, 8048, 14712, 5140, 4369, 7895, 9663, 1292, 15624, 15767, 982, 21768, 17046, 12205, 31633, 282, 3279, 19697, 31292, 8947, 27126, 6310, 27613, 21775, 12005, 26448, 6586, 10029, 10671, 5829, 21019, 14082, 10117, 3207, 15697, 16686, 4566, 28241, 1015, 15983, 26298, 18672, 10872, 24853, 4241, 6484, 17874, 1718, 24807, 27623, 28057, 29218, 12956, 1566, 3902, 24985, 30529, 20045, 1681, 21820, 13252, 26047, 8069, 6594, 12368, 10502, 25907, 27500, 24431, 2704, 25122, 17723, 16498, 31611, 13766, 12384, 16658, 22186, 9030, 10228, 25175, 537, 32131, 25833, 9401, 14957, 5205, 9793, 32646, 15568, 1815, 18813, 26499, 17147, 8295, 18265, 10454, 1428, 7975, 26806, 6179, 14523, 4295, 18392, 24310, 14048, 8391, 29137, 26465, 19927, 375, 15421, 10982, 25134, 20994, 9008, 26774, 23742, 9642, 17261, 1659, 5760, 4181, 17625, 8756, 13986, 23625, 18735, 5343, 14042, 19764, 13044, 28220, 507, 5231, 22702, 11022, 24774, 23171, 30801, 10949, 10498, 493, 32506, 10331, 25295, 1822, 30943, 14972, 22583, 8646, 15750, 7913, 17158, 2898, 11318, 21790, 3291, 3763, 3244, 19507, 21888, 17762, 26638, 10240, 13175, 9633, 28197, 24964, 25301, 17923, 15046, 18056, 22100, 31278, 3985, 7295, 24227, 11488, 21983, 27313, 23552, 6960, 31989, 12184, 11785, 21783, 2174, 1418, 21967, 10030, 25400, 31385, 32301, 20921, 18925, 8120, 24152, 12517, 25253, 4895, 10934, 14926, 21530, 2544, 2036, 29636, 29036, 7450, 13395, 18255, 19990, 23933, 23362, 22640, 20001, 29520, 18226, 16042, 1687, 19648, 26022, 18753, 571, 8154, 20873, 12354, 15991, 17323, 22469, 22211, 29846, 9382, 10769, 620, 819, 22598, 13322, 11319, 3057, 5108, 18054, 20083, 6509, 2858, 3365, 21519, 24945, 30937, 553, 3349, 30571, 22536, 21639, 19573, 797, 4030, 29757, 4868, 20373, 5667, 14567, 27588, 18016, 20194, 22157, 23910, 11003, 10449, 5312, 5389, 10890, 16084, 31717, 11861, 5068, 32767, 27856, 15508, 19279, 8954, 22165, 31072, 20448, 26794, 32410, 15441, 25414, 29197, 3825, 8199, 11056, 11622, 24293, 30831, 24171, 7835, 10707, 32323, 21080, 1755, 10300, 21456, 31902, 12163, 16151, 18172, 15644, 23619, 14700, 29831, 1058, 10365, 16414, 12433, 10464, 21485, 1033, 22621, 1054, 17039, 20044, 29012, 9006, 13245, 27368, 14669, 10015, 29848, 8587, 3083, 9268, 4522, 12454, 16903, 5524, 19402, 32453, 27760, 14659, 21567, 30240, 4721, 26705, 5160, 15981, 21426, 16210, 795, 18681, 5153, 11022, 20748, 25335, 5362, 11641, 26480, 16920, 7908, 28389, 1443, 16220, 8647, 28534, 31879, 13351, 27963, 27862, 19823, 11671, 26298, 20523, 12347, 27157, 24793, 14406, 17604, 21802, 30666, 29725, 29562, 30100, 5926, 380, 16715, 410, 3305, 25591, 6855, 3484, 19903, 26659, 23170, 2286, 20094, 9990, 3770, 31975, 5875, 24011, 16954, 16948, 29153, 28090, 19582, 8306, 24135, 6248, 27928, 3069
};

#else //!__cplusplus
#error C++ compiler required to compile functional_lite_rng.hpp
#endif //__cplusplus

#endif //FUNCTIONAL_LITE_RNG_HPP_GUARD

#undef FUNCTIONAL_LITE_RNG_HPP_RECURSE_GUARD
#endif //FUNCTIONAL_LITE_RNG_HPP_RECURSE_GUARD