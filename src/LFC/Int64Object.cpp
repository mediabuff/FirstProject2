#include "stdafx.h"
#include "LFC.h"

namespace System
{

Int64Object Int64Object::s_values[258] =
{
	Int64Object(-1),
	Int64Object(0),
	Int64Object(1),
	Int64Object(2),
	Int64Object(3),
	Int64Object(4),
	Int64Object(5),
	Int64Object(6),
	Int64Object(7),
	Int64Object(8),
	Int64Object(9),
	Int64Object(10),
	Int64Object(11),
	Int64Object(12),
	Int64Object(13),
	Int64Object(14),
	Int64Object(15),
	Int64Object(16),
	Int64Object(17),
	Int64Object(18),
	Int64Object(19),
	Int64Object(20),
	Int64Object(21),
	Int64Object(22),
	Int64Object(23),
	Int64Object(24),
	Int64Object(25),
	Int64Object(26),
	Int64Object(27),
	Int64Object(28),
	Int64Object(29),
	Int64Object(30),
	Int64Object(31),
	Int64Object(32),
	Int64Object(33),
	Int64Object(34),
	Int64Object(35),
	Int64Object(36),
	Int64Object(37),
	Int64Object(38),
	Int64Object(39),
	Int64Object(40),
	Int64Object(41),
	Int64Object(42),
	Int64Object(43),
	Int64Object(44),
	Int64Object(45),
	Int64Object(46),
	Int64Object(47),
	Int64Object(48),
	Int64Object(49),
	Int64Object(50),
	Int64Object(51),
	Int64Object(52),
	Int64Object(53),
	Int64Object(54),
	Int64Object(55),
	Int64Object(56),
	Int64Object(57),
	Int64Object(58),
	Int64Object(59),
	Int64Object(60),
	Int64Object(61),
	Int64Object(62),
	Int64Object(63),
	Int64Object(64),
	Int64Object(65),
	Int64Object(66),
	Int64Object(67),
	Int64Object(68),
	Int64Object(69),
	Int64Object(70),
	Int64Object(71),
	Int64Object(72),
	Int64Object(73),
	Int64Object(74),
	Int64Object(75),
	Int64Object(76),
	Int64Object(77),
	Int64Object(78),
	Int64Object(79),
	Int64Object(80),
	Int64Object(81),
	Int64Object(82),
	Int64Object(83),
	Int64Object(84),
	Int64Object(85),
	Int64Object(86),
	Int64Object(87),
	Int64Object(88),
	Int64Object(89),
	Int64Object(90),
	Int64Object(91),
	Int64Object(92),
	Int64Object(93),
	Int64Object(94),
	Int64Object(95),
	Int64Object(96),
	Int64Object(97),
	Int64Object(98),
	Int64Object(99),
	Int64Object(100),
	Int64Object(101),
	Int64Object(102),
	Int64Object(103),
	Int64Object(104),
	Int64Object(105),
	Int64Object(106),
	Int64Object(107),
	Int64Object(108),
	Int64Object(109),
	Int64Object(110),
	Int64Object(111),
	Int64Object(112),
	Int64Object(113),
	Int64Object(114),
	Int64Object(115),
	Int64Object(116),
	Int64Object(117),
	Int64Object(118),
	Int64Object(119),
	Int64Object(120),
	Int64Object(121),
	Int64Object(122),
	Int64Object(123),
	Int64Object(124),
	Int64Object(125),
	Int64Object(126),
	Int64Object(127),
	Int64Object(128),
	Int64Object(129),
	Int64Object(130),
	Int64Object(131),
	Int64Object(132),
	Int64Object(133),
	Int64Object(134),
	Int64Object(135),
	Int64Object(136),
	Int64Object(137),
	Int64Object(138),
	Int64Object(139),
	Int64Object(140),
	Int64Object(141),
	Int64Object(142),
	Int64Object(143),
	Int64Object(144),
	Int64Object(145),
	Int64Object(146),
	Int64Object(147),
	Int64Object(148),
	Int64Object(149),
	Int64Object(150),
	Int64Object(151),
	Int64Object(152),
	Int64Object(153),
	Int64Object(154),
	Int64Object(155),
	Int64Object(156),
	Int64Object(157),
	Int64Object(158),
	Int64Object(159),
	Int64Object(160),
	Int64Object(161),
	Int64Object(162),
	Int64Object(163),
	Int64Object(164),
	Int64Object(165),
	Int64Object(166),
	Int64Object(167),
	Int64Object(168),
	Int64Object(169),
	Int64Object(170),
	Int64Object(171),
	Int64Object(172),
	Int64Object(173),
	Int64Object(174),
	Int64Object(175),
	Int64Object(176),
	Int64Object(177),
	Int64Object(178),
	Int64Object(179),
	Int64Object(180),
	Int64Object(181),
	Int64Object(182),
	Int64Object(183),
	Int64Object(184),
	Int64Object(185),
	Int64Object(186),
	Int64Object(187),
	Int64Object(188),
	Int64Object(189),
	Int64Object(190),
	Int64Object(191),
	Int64Object(192),
	Int64Object(193),
	Int64Object(194),
	Int64Object(195),
	Int64Object(196),
	Int64Object(197),
	Int64Object(198),
	Int64Object(199),
	Int64Object(200),
	Int64Object(201),
	Int64Object(202),
	Int64Object(203),
	Int64Object(204),
	Int64Object(205),
	Int64Object(206),
	Int64Object(207),
	Int64Object(208),
	Int64Object(209),
	Int64Object(210),
	Int64Object(211),
	Int64Object(212),
	Int64Object(213),
	Int64Object(214),
	Int64Object(215),
	Int64Object(216),
	Int64Object(217),
	Int64Object(218),
	Int64Object(219),
	Int64Object(220),
	Int64Object(221),
	Int64Object(222),
	Int64Object(223),
	Int64Object(224),
	Int64Object(225),
	Int64Object(226),
	Int64Object(227),
	Int64Object(228),
	Int64Object(229),
	Int64Object(230),
	Int64Object(231),
	Int64Object(232),
	Int64Object(233),
	Int64Object(234),
	Int64Object(235),
	Int64Object(236),
	Int64Object(237),
	Int64Object(238),
	Int64Object(239),
	Int64Object(240),
	Int64Object(241),
	Int64Object(242),
	Int64Object(243),
	Int64Object(244),
	Int64Object(245),
	Int64Object(246),
	Int64Object(247),
	Int64Object(248),
	Int64Object(249),
	Int64Object(250),
	Int64Object(251),
	Int64Object(252),
	Int64Object(253),
	Int64Object(254),
	Int64Object(255),
	Int64Object(256),
};

Int64Object Int64Object::s_minValue(_I64_MIN);
Int64Object Int64Object::s_maxValue(_I64_MAX);

String Int64Object::ToString()
{
	return String::FromNumber(m_value);
}

Int64Object* Int64Object::GetObject(long long value)
{
	if (value >= -1 && value <= 256)
	{
		ASSERT(s_values[value+1].m_value == value);
		return &s_values[value+1];
	}
	else if (value == _I64_MIN)
	{
		return &s_minValue;
	}
	else if (value == _I64_MAX)
	{
		return &s_maxValue;
	}

	return new Int64Object(value);
}

}	// System
