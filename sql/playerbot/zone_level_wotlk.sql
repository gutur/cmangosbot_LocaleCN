-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.7.26 - MySQL Community Server (GPL)
-- Server OS:                    Win32
-- HeidiSQL Version:             10.1.0.5464
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table wotlkplayerbots.ai_playerbot_zone_level
DROP TABLE IF EXISTS `ai_playerbot_zone_level`;
CREATE TABLE IF NOT EXISTS `ai_playerbot_zone_level` (
  `id` bigint(20) NOT NULL,
  `level` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table wotlkplayerbots.ai_playerbot_zone_level: ~2.307 rows (approximately)
/*!40000 ALTER TABLE `ai_playerbot_zone_level` DISABLE KEYS */;
INSERT INTO `ai_playerbot_zone_level` (`id`, `level`) VALUES
	(1, 7),
	(2, 15),
	(3, 39),
	(4, 51),
	(7, 39),
	(8, 39),
	(9, 2),
	(10, 24),
	(11, 24),
	(12, 6),
	(13, 24),
	(14, 7),
	(15, 34),
	(16, 50),
	(17, 15),
	(18, 7),
	(19, 40),
	(20, 14),
	(21, -1),
	(22, 60),
	(23, 6),
	(24, 6),
	(25, -1),
	(26, 14),
	(28, 51),
	(30, -1),
	(32, 24),
	(33, 39),
	(34, 3),
	(35, 42),
	(36, 32),
	(37, 36),
	(38, 12),
	(40, 14),
	(41, 57),
	(42, 25),
	(43, 42),
	(44, 19),
	(45, 32),
	(46, 52),
	(47, 44),
	(49, 60),
	(51, 47),
	(53, 6),
	(54, 8),
	(55, 6),
	(56, 10),
	(57, 5),
	(59, 3),
	(60, 8),
	(61, 9),
	(62, 8),
	(63, 6),
	(64, 6),
	(65, 63),
	(66, 76),
	(67, 77),
	(68, 16),
	(69, 15),
	(70, 24),
	(71, 20),
	(72, 63),
	(73, 50),
	(74, 43),
	(75, 37),
	(76, 37),
	(77, 7),
	(80, 10),
	(81, 60),
	(82, 60),
	(83, 60),
	(84, 60),
	(85, 7),
	(86, 8),
	(87, 5),
	(88, 6),
	(89, 7),
	(91, 8),
	(92, 8),
	(93, 28),
	(94, 24),
	(95, 20),
	(96, 25),
	(97, 19),
	(98, 17),
	(99, 30),
	(100, 31),
	(101, 32),
	(102, 41),
	(103, 40),
	(104, 37),
	(105, 42),
	(106, 39),
	(107, 14),
	(108, 15),
	(109, 15),
	(111, 12),
	(113, 15),
	(115, 17),
	(116, 39),
	(117, 35),
	(118, 22),
	(120, 8),
	(121, 28),
	(122, 37),
	(123, 34),
	(125, 35),
	(126, 33),
	(127, 40),
	(128, 36),
	(129, 37),
	(130, 14),
	(131, 5),
	(132, 2),
	(133, 10),
	(134, 10),
	(135, 7),
	(136, 5),
	(137, 7),
	(138, 7),
	(139, 56),
	(141, 7),
	(142, 15),
	(143, 18),
	(144, 10),
	(145, 18),
	(146, 15),
	(147, 15),
	(148, 14),
	(149, 12),
	(150, 20),
	(151, -1),
	(152, 9),
	(153, 10),
	(154, 2),
	(155, 3),
	(156, 6),
	(157, 8),
	(158, 7),
	(159, 5),
	(160, 12),
	(161, 10),
	(162, 7),
	(163, 8),
	(164, 7),
	(165, 5),
	(166, 5),
	(167, 8),
	(168, 9),
	(169, 8),
	(170, -1),
	(172, 18),
	(173, 7),
	(186, 5),
	(187, 7),
	(188, 2),
	(189, 5),
	(190, 56),
	(192, 55),
	(193, 55),
	(195, 51),
	(196, 30),
	(197, 50),
	(198, 55),
	(199, 50),
	(200, 54),
	(201, 57),
	(202, 46),
	(203, 51),
	(204, 15),
	(205, 28),
	(206, 56),
	(207, -1),
	(208, -1),
	(209, 16),
	(210, 80),
	(211, 7),
	(212, 8),
	(213, 13),
	(214, 63),
	(215, 6),
	(219, 15),
	(220, 2),
	(221, 2),
	(222, 5),
	(223, 5),
	(224, 7),
	(225, 9),
	(226, 11),
	(227, 11),
	(228, 10),
	(229, 12),
	(230, 18),
	(231, 20),
	(232, 18),
	(233, 15),
	(235, 14),
	(236, 16),
	(237, 16),
	(238, 11),
	(239, 11),
	(240, 12),
	(241, 26),
	(242, 28),
	(243, 28),
	(244, 24),
	(245, 25),
	(246, 48),
	(247, 45),
	(249, 50),
	(250, 54),
	(251, 37),
	(252, 57),
	(253, 56),
	(254, 55),
	(255, 59),
	(256, 2),
	(257, 7),
	(258, 7),
	(259, 5),
	(260, 6),
	(261, 8),
	(262, 7),
	(263, 7),
	(264, 9),
	(265, 11),
	(266, 9),
	(267, 25),
	(268, -1),
	(269, 18),
	(271, 22),
	(272, 20),
	(275, 21),
	(276, -1),
	(277, 32),
	(278, 32),
	(279, 30),
	(280, 34),
	(281, 36),
	(282, 35),
	(283, 35),
	(284, 27),
	(285, 21),
	(286, 22),
	(287, 25),
	(288, 27),
	(289, 26),
	(290, 30),
	(293, 30),
	(294, 30),
	(295, 30),
	(296, -1),
	(297, 50),
	(298, 28),
	(299, 25),
	(300, 41),
	(301, 36),
	(302, 43),
	(303, 39),
	(305, 18),
	(306, 15),
	(307, 48),
	(308, -1),
	(309, 24),
	(310, 41),
	(311, 44),
	(312, 46),
	(313, 31),
	(314, 33),
	(315, 31),
	(316, 35),
	(317, 33),
	(318, 36),
	(320, 30),
	(321, 30),
	(322, 32),
	(323, 32),
	(324, 36),
	(325, 32),
	(326, 32),
	(327, 40),
	(328, 38),
	(330, -1),
	(331, 24),
	(332, -1),
	(333, 38),
	(334, 38),
	(335, 38),
	(336, 38),
	(337, 36),
	(338, 39),
	(339, 45),
	(340, 38),
	(341, 36),
	(342, 39),
	(343, 39),
	(344, 43),
	(345, 39),
	(346, 36),
	(347, 43),
	(348, 41),
	(349, 44),
	(350, 45),
	(351, 48),
	(352, 41),
	(353, 44),
	(354, 45),
	(355, 46),
	(356, 41),
	(357, 44),
	(358, 4),
	(359, 25),
	(360, 7),
	(361, 49),
	(362, 5),
	(363, 2),
	(364, 7),
	(365, 7),
	(366, 7),
	(367, 6),
	(368, 7),
	(369, 9),
	(370, 8),
	(371, 7),
	(372, 6),
	(373, 5),
	(374, 8),
	(375, 8),
	(377, 9),
	(378, 10),
	(379, 10),
	(380, 15),
	(381, 17),
	(382, 15),
	(383, 14),
	(384, 11),
	(385, 15),
	(386, 12),
	(387, 12),
	(388, 15),
	(390, 20),
	(391, 12),
	(392, 15),
	(393, 6),
	(394, 74),
	(395, 74),
	(396, 6),
	(397, 7),
	(398, 8),
	(399, 6),
	(400, 29),
	(401, 1),
	(403, 34),
	(404, 8),
	(405, 33),
	(406, 18),
	(407, 10),
	(408, -1),
	(409, -1),
	(410, 8),
	(411, 24),
	(412, 18),
	(413, 19),
	(414, 20),
	(415, 20),
	(416, 23),
	(417, 25),
	(418, 23),
	(419, 28),
	(420, 25),
	(421, 25),
	(422, 26),
	(424, 22),
	(425, 24),
	(426, 24),
	(427, 23),
	(428, 25),
	(429, 28),
	(430, 28),
	(431, 25),
	(432, 30),
	(433, 26),
	(434, 29),
	(435, 25),
	(436, 30),
	(437, 22),
	(438, 20),
	(439, 35),
	(440, 45),
	(441, 20),
	(442, 12),
	(443, 19),
	(444, 16),
	(445, 14),
	(446, 11),
	(447, 11),
	(448, 15),
	(449, 16),
	(450, 19),
	(452, 14),
	(453, 11),
	(454, 12),
	(455, 17),
	(456, 14),
	(457, -1),
	(458, 10),
	(459, 10),
	(460, 20),
	(461, 20),
	(463, 15),
	(464, 21),
	(465, 25),
	(466, 18),
	(467, 25),
	(468, 18),
	(469, 13),
	(470, 10),
	(471, 6),
	(472, 6),
	(473, 6),
	(474, 6),
	(475, 6),
	(476, 6),
	(477, 43),
	(478, 8),
	(479, 34),
	(480, 30),
	(481, 24),
	(482, 29),
	(483, 30),
	(484, 26),
	(485, 25),
	(486, 29),
	(487, 29),
	(488, 29),
	(489, 44),
	(490, 52),
	(491, 25),
	(492, 26),
	(493, 20),
	(495, 70),
	(496, 36),
	(497, 4),
	(498, 37),
	(499, 35),
	(500, 34),
	(501, 37),
	(502, 36),
	(503, 34),
	(504, 1),
	(505, 34),
	(506, 37),
	(507, 36),
	(508, 37),
	(509, 38),
	(510, 32),
	(511, 43),
	(512, 36),
	(513, 36),
	(514, 34),
	(515, 34),
	(516, 33),
	(517, 33),
	(518, 36),
	(536, 24),
	(537, 55),
	(538, 49),
	(539, 53),
	(540, 51),
	(541, 64),
	(542, 51),
	(543, 53),
	(556, 12),
	(576, 25),
	(596, 35),
	(597, 36),
	(598, 32),
	(599, 30),
	(600, 33),
	(602, 39),
	(603, 31),
	(604, 36),
	(606, 34),
	(607, 37),
	(608, 30),
	(609, 32),
	(616, -1),
	(618, 56),
	(636, 19),
	(637, 1),
	(638, 7),
	(639, 1),
	(640, 12),
	(656, 15),
	(657, 37),
	(676, -1),
	(696, 7),
	(697, 7),
	(698, 7),
	(699, 7),
	(700, 7),
	(701, 7),
	(702, 11),
	(716, 9),
	(717, 23),
	(718, 15),
	(719, 22),
	(720, 15),
	(721, 27),
	(722, 28),
	(736, 8),
	(796, 33),
	(797, 8),
	(798, 8),
	(799, 10),
	(800, 4),
	(801, 8),
	(802, 8),
	(803, 7),
	(804, 8),
	(805, 4),
	(806, 12),
	(807, 6),
	(808, 11),
	(809, 7),
	(810, 7),
	(811, 5),
	(812, 10),
	(813, 52),
	(814, 10),
	(815, 11),
	(816, 8),
	(817, 10),
	(818, 5),
	(819, 9),
	(820, 5),
	(821, 8),
	(836, 20),
	(837, 8),
	(838, 1),
	(839, 12),
	(856, 19),
	(876, -1),
	(877, 15),
	(878, 47),
	(879, 29),
	(880, 30),
	(881, 29),
	(896, 30),
	(916, 9),
	(917, 18),
	(918, 14),
	(919, 14),
	(920, 17),
	(921, 16),
	(922, 18),
	(923, 13),
	(924, 11),
	(925, 12),
	(926, 14),
	(927, 14),
	(928, 13),
	(936, 12),
	(956, -1),
	(976, 40),
	(977, 40),
	(978, 45),
	(979, 44),
	(980, 49),
	(981, 48),
	(982, 48),
	(983, 47),
	(984, 47),
	(985, 44),
	(986, 45),
	(987, 48),
	(988, 43),
	(989, 48),
	(990, 50),
	(991, 45),
	(992, 47),
	(996, 22),
	(997, 20),
	(998, 25),
	(999, 21),
	(1000, 22),
	(1001, 16),
	(1002, 16),
	(1016, 28),
	(1017, 30),
	(1018, 25),
	(1019, 56),
	(1020, 22),
	(1021, 22),
	(1022, 24),
	(1023, 25),
	(1024, 20),
	(1025, 21),
	(1036, 28),
	(1037, 25),
	(1038, 25),
	(1039, 25),
	(1056, 31),
	(1057, 25),
	(1076, 21),
	(1097, 19),
	(1098, 30),
	(1099, 42),
	(1100, 41),
	(1101, 44),
	(1102, 44),
	(1103, 42),
	(1104, 44),
	(1105, 44),
	(1106, 46),
	(1107, 44),
	(1108, 49),
	(1109, 44),
	(1110, 44),
	(1111, 60),
	(1112, 61),
	(1113, 60),
	(1114, 49),
	(1115, 47),
	(1116, 43),
	(1117, 42),
	(1118, 44),
	(1119, 48),
	(1120, 43),
	(1121, 44),
	(1136, 42),
	(1137, 42),
	(1156, 19),
	(1157, 20),
	(1176, 44),
	(1196, 69),
	(1216, 49),
	(1217, 50),
	(1218, 50),
	(1219, 52),
	(1220, 52),
	(1221, 48),
	(1222, 54),
	(1223, 50),
	(1224, 48),
	(1225, 49),
	(1226, 55),
	(1227, 50),
	(1228, 52),
	(1229, 53),
	(1230, 51),
	(1231, 52),
	(1232, 54),
	(1233, 49),
	(1234, 52),
	(1235, 46),
	(1236, 45),
	(1237, 49),
	(1256, 42),
	(1276, 22),
	(1277, 20),
	(1296, 12),
	(1297, 5),
	(1316, 33),
	(1336, 45),
	(1337, 38),
	(1338, 16),
	(1339, 23),
	(1357, 36),
	(1377, 56),
	(1397, -1),
	(1417, -1),
	(1437, 47),
	(1438, 50),
	(1439, 55),
	(1440, 51),
	(1441, 54),
	(1442, 48),
	(1443, 47),
	(1444, 56),
	(1445, 43),
	(1446, 48),
	(1457, 47),
	(1477, 47),
	(1497, 10),
	(1517, 39),
	(1518, 14),
	(1519, 10),
	(1537, 10),
	(1557, 29),
	(1577, 43),
	(1578, 41),
	(1579, -1),
	(1580, -1),
	(1581, 18),
	(1582, 18),
	(1583, 55),
	(1584, 52),
	(1597, 15),
	(1598, 15),
	(1599, 15),
	(1600, 15),
	(1601, 15),
	(1602, 15),
	(1603, 15),
	(1617, 10),
	(1637, 10),
	(1638, 10),
	(1639, 47),
	(1640, 10),
	(1641, 10),
	(1657, 10),
	(1658, 8),
	(1659, 4),
	(1660, 50),
	(1661, 2),
	(1662, 1),
	(1677, 33),
	(1678, 33),
	(1679, 33),
	(1680, 34),
	(1681, 31),
	(1682, 39),
	(1683, 34),
	(1684, 30),
	(1697, 18),
	(1698, 20),
	(1699, 13),
	(1700, 19),
	(1701, 20),
	(1702, 18),
	(1703, 17),
	(1704, 11),
	(1717, 24),
	(1718, 15),
	(1737, 40),
	(1738, 43),
	(1739, 42),
	(1740, 36),
	(1741, 43),
	(1742, 41),
	(1757, 39),
	(1758, 39),
	(1759, 39),
	(1760, 35),
	(1761, 47),
	(1762, 54),
	(1763, 51),
	(1764, 51),
	(1765, 52),
	(1766, 53),
	(1767, 53),
	(1768, 49),
	(1769, 54),
	(1770, 49),
	(1771, 49),
	(1777, 36),
	(1778, 41),
	(1779, 39),
	(1780, 36),
	(1797, 41),
	(1798, 36),
	(1817, 39),
	(1837, 32),
	(1857, 32),
	(1858, 32),
	(1877, 42),
	(1878, 39),
	(1879, 40),
	(1880, 42),
	(1881, 44),
	(1882, 45),
	(1883, 48),
	(1884, 46),
	(1885, 42),
	(1886, 47),
	(1887, 43),
	(1897, 36),
	(1898, 39),
	(1917, 49),
	(1937, 42),
	(1938, 45),
	(1939, 45),
	(1940, 49),
	(1941, -1),
	(1942, 48),
	(1943, 50),
	(1957, 47),
	(1958, 43),
	(1959, 45),
	(1977, 60),
	(1978, 41),
	(1997, 27),
	(1998, 53),
	(2017, 57),
	(2037, -1),
	(2057, 50),
	(2077, 15),
	(2078, 16),
	(2079, 61),
	(2097, 26),
	(2098, 24),
	(2099, 19),
	(2100, 40),
	(2101, 12),
	(2102, 7),
	(2103, 24),
	(2104, 24),
	(2117, 7),
	(2118, 7),
	(2119, 7),
	(2137, 6),
	(2138, 15),
	(2157, 15),
	(2158, 34),
	(2159, 80),
	(2160, 18),
	(2161, 24),
	(2177, 39),
	(2197, 10),
	(2198, 30),
	(2217, 33),
	(2237, 28),
	(2238, 29),
	(2239, 29),
	(2240, 32),
	(2241, 57),
	(2242, 58),
	(2243, 55),
	(2244, 57),
	(2245, 56),
	(2246, 55),
	(2247, 56),
	(2248, 58),
	(2249, 60),
	(2250, 56),
	(2251, 56),
	(2252, 55),
	(2253, 55),
	(2254, 56),
	(2255, 55),
	(2256, 60),
	(2257, 15),
	(2258, 58),
	(2259, 56),
	(2260, 54),
	(2261, 54),
	(2262, 55),
	(2263, 55),
	(2264, 57),
	(2265, 54),
	(2266, 55),
	(2267, 56),
	(2268, 58),
	(2269, 59),
	(2270, 58),
	(2271, 56),
	(2272, 58),
	(2273, 57),
	(2274, 56),
	(2275, 56),
	(2276, 59),
	(2277, 55),
	(2278, 56),
	(2279, 55),
	(2280, -1),
	(2297, 55),
	(2298, 55),
	(2299, 56),
	(2300, 50),
	(2301, 23),
	(2302, 39),
	(2303, 27),
	(2317, 45),
	(2318, 35),
	(2319, 15),
	(2320, 7),
	(2321, 50),
	(2322, 7),
	(2323, 44),
	(2324, 34),
	(2325, 24),
	(2326, 14),
	(2337, 7),
	(2338, 39),
	(2339, 39),
	(2357, 27),
	(2358, 38),
	(2359, 24),
	(2360, 22),
	(2361, 15),
	(2362, 20),
	(2363, 31),
	(2364, 14),
	(2365, 24),
	(2366, 48),
	(2367, 54),
	(2368, 58),
	(2369, 38),
	(2370, 67),
	(2371, 54),
	(2372, 54),
	(2373, 54),
	(2374, 54),
	(2375, 54),
	(2376, 54),
	(2377, 54),
	(2378, 54),
	(2379, 54),
	(2397, 25),
	(2398, 14),
	(2399, 13),
	(2400, 44),
	(2401, 1),
	(2402, 24),
	(2403, 43),
	(2404, 30),
	(2405, 32),
	(2406, 35),
	(2407, 32),
	(2408, 33),
	(2417, 54),
	(2418, 55),
	(2419, 52),
	(2420, 52),
	(2421, 54),
	(2437, 14),
	(2457, 25),
	(2477, 56),
	(2478, 47),
	(2479, 48),
	(2480, 50),
	(2481, 51),
	(2497, 53),
	(2517, 46),
	(2518, 42),
	(2519, 41),
	(2520, 43),
	(2521, -1),
	(2522, 45),
	(2537, 15),
	(2538, 15),
	(2539, 15),
	(2540, 16),
	(2541, 18),
	(2557, 53),
	(2558, 56),
	(2559, 57),
	(2560, 57),
	(2561, 57),
	(2562, 60),
	(2563, 69),
	(2577, 44),
	(2597, 54),
	(2617, 29),
	(2618, 50),
	(2619, 55),
	(2620, 56),
	(2621, 54),
	(2622, 55),
	(2623, 59),
	(2624, 58),
	(2625, 56),
	(2626, 56),
	(2627, 55),
	(2637, 24),
	(2657, 38),
	(2677, 58),
	(2697, 50),
	(2717, 61),
	(2737, 55),
	(2738, 55),
	(2739, 55),
	(2740, 55),
	(2741, 56),
	(2742, 55),
	(2743, 55),
	(2744, 55),
	(2757, 13),
	(2777, 25),
	(2797, 24),
	(2817, 77),
	(2837, 57),
	(2838, 47),
	(2839, 32),
	(2857, 45),
	(2877, 60),
	(2897, 20),
	(2917, -1),
	(2918, -1),
	(2937, 57),
	(2938, 50),
	(2957, 58),
	(2958, 61),
	(2959, 54),
	(2960, 54),
	(2961, 54),
	(2962, 54),
	(2963, 53),
	(2964, 54),
	(2977, 54),
	(2978, 54),
	(2979, 9),
	(3017, 54),
	(3037, 29),
	(3038, 31),
	(3039, 33),
	(3057, 54),
	(3058, 54),
	(3077, 55),
	(3097, 58),
	(3098, 57),
	(3099, 59),
	(3100, 58),
	(3117, 44),
	(3137, 50),
	(3138, 54),
	(3139, 56),
	(3140, 50),
	(3157, 19),
	(3177, 20),
	(3197, 53),
	(3217, 53),
	(3237, 53),
	(3257, 57),
	(3277, 2147483647),
	(3297, 54),
	(3298, 54),
	(3299, 54),
	(3300, 54),
	(3301, 54),
	(3302, 54),
	(3303, 54),
	(3304, 54),
	(3305, 54),
	(3306, 54),
	(3317, 44),
	(3318, 54),
	(3319, 24),
	(3320, -1),
	(3321, -1),
	(3337, 54),
	(3338, 54),
	(3357, 39),
	(3358, 102261124),
	(3377, 60),
	(3378, 60),
	(3379, 60),
	(3380, 60),
	(3381, 60),
	(3382, 60),
	(3383, 60),
	(3384, 60),
	(3397, 60),
	(3398, 61),
	(3417, 715827881),
	(3418, 715827881),
	(3419, 60),
	(3420, 715827881),
	(3421, 715827881),
	(3422, 2147483647),
	(3423, -1),
	(3424, -1),
	(3425, 55),
	(3426, 59),
	(3427, 58),
	(3428, 60),
	(3429, 60),
	(3430, 6),
	(3431, 6),
	(3432, 6),
	(3433, 13),
	(3434, 60),
	(3435, 60),
	(3436, 60),
	(3437, 60),
	(3438, 60),
	(3439, 60),
	(3440, 60),
	(3441, 60),
	(3442, 60),
	(3443, 60),
	(3444, 60),
	(3445, 60),
	(3446, 56),
	(3447, 56),
	(3448, 61),
	(3449, 61),
	(3450, 60),
	(3451, 60),
	(3452, 61),
	(3453, 62),
	(3454, 56),
	(3455, 9),
	(3456, 80),
	(3457, 70),
	(3459, -1),
	(3460, 6),
	(3461, 7),
	(3462, 6),
	(3463, 5),
	(3464, 7),
	(3465, 5),
	(3466, 6),
	(3467, 7),
	(3468, 7),
	(3469, 5),
	(3470, 6),
	(3471, 9),
	(3472, 10),
	(3473, 8),
	(3474, 9),
	(3475, 10),
	(3476, 10),
	(3477, 73),
	(3478, -1),
	(3479, 9),
	(3480, 9),
	(3481, 10),
	(3482, 5),
	(3483, 61),
	(3484, 6),
	(3485, 6),
	(3486, 30),
	(3487, 10),
	(3488, 10),
	(3489, 10),
	(3490, 11),
	(3491, 14),
	(3492, 16),
	(3493, 12),
	(3494, 13),
	(3495, 14),
	(3496, 14),
	(3497, 10),
	(3498, 13),
	(3499, 16),
	(3500, 19),
	(3501, 18),
	(3502, 18),
	(3503, 14),
	(3504, 10),
	(3505, 13),
	(3506, 15),
	(3507, 11),
	(3508, 18),
	(3509, 10),
	(3510, 15),
	(3511, 20),
	(3512, 11),
	(3513, 13),
	(3514, 10),
	(3515, 11),
	(3516, 17),
	(3517, 18),
	(3518, 64),
	(3519, 64),
	(3520, 68),
	(3521, 61),
	(3522, 65),
	(3523, 58),
	(3524, 7),
	(3525, 14),
	(3526, 7),
	(3527, 7),
	(3528, 7),
	(3529, 7),
	(3530, 5),
	(3531, 5),
	(3532, 5),
	(3533, 5),
	(3534, 5),
	(3535, -1),
	(3536, 61),
	(3537, 70),
	(3538, 70),
	(3539, 58),
	(3540, -1),
	(3541, 61),
	(3542, 58),
	(3543, 61),
	(3544, 61),
	(3545, 62),
	(3546, 59),
	(3547, 70),
	(3548, 60),
	(3549, 60),
	(3550, 58),
	(3551, 62),
	(3552, 61),
	(3553, 61),
	(3554, 61),
	(3555, 61),
	(3556, 62),
	(3557, 10),
	(3558, 7),
	(3559, 7),
	(3560, 7),
	(3561, 4),
	(3562, 60),
	(3563, 60),
	(3564, 7),
	(3565, 61),
	(3566, 10),
	(3567, 6),
	(3568, 6),
	(3569, 7),
	(3570, 6),
	(3571, 8),
	(3572, 7),
	(3573, 6),
	(3574, 7),
	(3575, 10),
	(3576, 5),
	(3577, 6),
	(3578, 8),
	(3579, 9),
	(3580, 9),
	(3581, 8),
	(3582, 60),
	(3583, 10),
	(3584, 10),
	(3585, 14),
	(3586, 19),
	(3587, 15),
	(3588, 16),
	(3589, 13),
	(3590, 12),
	(3591, 13),
	(3592, 13),
	(3593, 16),
	(3594, 12),
	(3595, 17),
	(3596, 16),
	(3597, 17),
	(3598, 17),
	(3599, 17),
	(3600, 10),
	(3601, 14),
	(3602, 14),
	(3603, 14),
	(3604, 9),
	(3605, -1),
	(3606, 68),
	(3607, 71),
	(3608, 14),
	(3609, 64),
	(3610, 64),
	(3611, 64),
	(3612, 16),
	(3613, 64),
	(3614, 64),
	(3615, 65),
	(3616, 64),
	(3617, 66),
	(3618, 64),
	(3619, 64),
	(3620, 64),
	(3621, 65),
	(3622, 65),
	(3623, 64),
	(3624, 66),
	(3625, 66),
	(3626, 64),
	(3627, 65),
	(3628, 66),
	(3629, 65),
	(3630, 65),
	(3631, 65),
	(3632, 64),
	(3633, 66),
	(3634, 64),
	(3635, 64),
	(3636, 69),
	(3637, 65),
	(3638, 64),
	(3639, 9),
	(3640, 62),
	(3641, 60),
	(3642, 61),
	(3643, 63),
	(3644, 61),
	(3645, 61),
	(3646, 63),
	(3647, 62),
	(3648, 61),
	(3649, 63),
	(3650, 62),
	(3651, 63),
	(3652, 61),
	(3653, 62),
	(3654, 63),
	(3655, 60),
	(3656, 62),
	(3657, 63),
	(3658, 62),
	(3659, 60),
	(3660, 61),
	(3661, 61),
	(3662, 6),
	(3663, 6),
	(3664, 6),
	(3665, 6),
	(3666, 64),
	(3667, 62),
	(3668, 62),
	(3669, 61),
	(3670, 61),
	(3671, 58),
	(3672, 64),
	(3673, 64),
	(3674, 62),
	(3675, 62),
	(3676, 62),
	(3677, 62),
	(3678, 62),
	(3679, 70),
	(3680, 70),
	(3681, 63),
	(3682, 64),
	(3683, 64),
	(3684, 64),
	(3685, 63),
	(3686, 64),
	(3687, 62),
	(3688, 65),
	(3689, 63),
	(3690, 70),
	(3691, 62),
	(3692, 62),
	(3693, 62),
	(3694, 62),
	(3695, 62),
	(3696, 62),
	(3697, 64),
	(3698, -1),
	(3699, 65),
	(3700, 64),
	(3701, 64),
	(3702, -1),
	(3703, 72),
	(3704, 10),
	(3705, 64),
	(3706, 68),
	(3707, 68),
	(3708, 1),
	(3709, 68),
	(3710, 68),
	(3711, 76),
	(3712, 58),
	(3713, 62),
	(3714, 66),
	(3715, 70),
	(3716, 63),
	(3717, 62),
	(3718, 62),
	(3719, 63),
	(3720, 62),
	(3721, 66),
	(3722, 67),
	(3723, 67),
	(3724, 67),
	(3725, 67),
	(3726, 67),
	(3727, 68),
	(3728, 68),
	(3729, 67),
	(3730, 68),
	(3731, 69),
	(3732, 69),
	(3733, 58),
	(3734, 69),
	(3735, 68),
	(3736, 69),
	(3737, 69),
	(3738, 69),
	(3739, 68),
	(3740, 68),
	(3741, 68),
	(3742, 70),
	(3743, 67),
	(3744, 68),
	(3745, 68),
	(3746, 67),
	(3747, 67),
	(3748, 68),
	(3749, 68),
	(3750, 68),
	(3751, 67),
	(3752, 67),
	(3753, 67),
	(3754, 70),
	(3755, 69),
	(3756, 67),
	(3757, 69),
	(3758, 67),
	(3759, 70),
	(3760, 64),
	(3761, 64),
	(3762, 64),
	(3763, 64),
	(3764, 70),
	(3765, 61),
	(3766, 61),
	(3767, 65),
	(3768, 69),
	(3769, 65),
	(3770, 65),
	(3771, 65),
	(3772, 65),
	(3773, 67),
	(3774, 67),
	(3775, 66),
	(3776, 66),
	(3777, 67),
	(3778, 66),
	(3779, 67),
	(3780, 67),
	(3781, 67),
	(3782, 65),
	(3783, 67),
	(3784, 70),
	(3785, 70),
	(3786, 66),
	(3787, 70),
	(3788, 64),
	(3789, 66),
	(3790, 66),
	(3791, 67),
	(3792, 64),
	(3793, 58),
	(3794, 58),
	(3795, 61),
	(3796, 60),
	(3797, 62),
	(3798, 62),
	(3799, 60),
	(3800, 62),
	(3801, 62),
	(3802, 61),
	(3803, 70),
	(3804, 59),
	(3805, 70),
	(3806, 61),
	(3807, 61),
	(3808, 61),
	(3809, 61),
	(3810, 61),
	(3811, 58),
	(3812, 61),
	(3813, 58),
	(3814, 58),
	(3815, 61),
	(3816, 61),
	(3817, -1),
	(3818, 63),
	(3819, 62),
	(3820, -1),
	(3821, 68),
	(3822, 67),
	(3823, 10),
	(3824, 65),
	(3825, 65),
	(3826, 65),
	(3827, 66),
	(3828, 66),
	(3829, 67),
	(3830, 65),
	(3831, 67),
	(3832, 69),
	(3833, 65),
	(3834, 69),
	(3835, 62),
	(3836, 66),
	(3837, 69),
	(3838, 60),
	(3839, 66),
	(3840, 70),
	(3841, 61),
	(3842, 70),
	(3844, 65),
	(3845, 70),
	(3846, 14),
	(3847, 69),
	(3848, 65),
	(3849, 69),
	(3850, 68),
	(3851, 68),
	(3852, 58),
	(3854, 58),
	(3855, 65),
	(3856, 10),
	(3857, 5),
	(3858, 63),
	(3859, 63),
	(3860, 63),
	(3861, 20),
	(3862, 67),
	(3863, 67),
	(3864, 70),
	(3865, 70),
	(3866, 66),
	(3867, 66),
	(3868, 67),
	(3869, -1),
	(3870, -1),
	(3871, -1),
	(3872, -1),
	(3873, 58),
	(3874, 68),
	(3875, 68),
	(3876, 68),
	(3877, 68),
	(3878, 69),
	(3879, 69),
	(3880, 69),
	(3881, 69),
	(3882, 68),
	(3883, 67),
	(3884, 60),
	(3885, 65),
	(3886, 65),
	(3887, 65),
	(3888, 65),
	(3889, 65),
	(3890, 65),
	(3891, 65),
	(3892, 65),
	(3893, 65),
	(3894, 65),
	(3895, 61),
	(3896, 72),
	(3897, 72),
	(3898, 72),
	(3899, 70),
	(3900, 69),
	(3901, 63),
	(3902, 63),
	(3903, 67),
	(3904, 70),
	(3905, 63),
	(3906, 16),
	(3907, 18),
	(3908, 13),
	(3909, 11),
	(3910, 13),
	(3911, 7),
	(3912, 8),
	(3913, 9),
	(3914, 9),
	(3915, 9),
	(3916, 6),
	(3917, 70),
	(3918, 65),
	(3919, 67),
	(3920, 58),
	(3921, 67),
	(3922, 67),
	(3923, 72),
	(3924, 65),
	(3925, 66),
	(3926, 67),
	(3927, 67),
	(3928, 67),
	(3929, 68),
	(3930, 69),
	(3931, 66),
	(3932, 67),
	(3933, 67),
	(3934, 68),
	(3935, 68),
	(3936, 67),
	(3937, 67),
	(3938, 68),
	(3939, 69),
	(3940, 67),
	(3941, 68),
	(3942, 66),
	(3943, 68),
	(3944, 70),
	(3945, 69),
	(3946, 64),
	(3947, 70),
	(3948, -1),
	(3949, 67),
	(3950, 70),
	(3951, 67),
	(3952, 67),
	(3953, 66),
	(3954, 66),
	(3955, 58),
	(3956, 64),
	(3957, 64),
	(3958, 65),
	(3959, 71),
	(3960, 67),
	(3961, 70),
	(3962, -1),
	(3963, -1),
	(3964, 65),
	(3965, 70),
	(3966, 70),
	(3967, 70),
	(3968, -1),
	(3969, 72),
	(3970, 36),
	(3971, 65),
	(3972, 71),
	(3973, 64),
	(3974, -1),
	(3975, -1),
	(3976, -1),
	(3977, -1),
	(3978, -1),
	(3979, 70),
	(3980, 70),
	(3981, 70),
	(3982, 70),
	(3983, 70),
	(3984, 70),
	(3985, 70),
	(3986, 70),
	(3987, 70),
	(3988, 70),
	(3989, 70),
	(3990, 70),
	(3991, 70),
	(3992, 70),
	(3993, 70),
	(3994, 70),
	(3995, 70),
	(3996, 70),
	(3997, 70),
	(3998, 70),
	(3999, 70),
	(4000, 70),
	(4001, 70),
	(4002, 70),
	(4003, 70),
	(4004, 70),
	(4005, 70),
	(4006, 70),
	(4007, 70),
	(4008, 70),
	(4009, 70),
	(4010, 38),
	(4018, 70),
	(4019, -1),
	(4020, 70),
	(4021, 70),
	(4022, 70),
	(4023, 70),
	(4024, 70),
	(4025, 70),
	(4026, 70),
	(4027, 70),
	(4028, 70),
	(4029, 70),
	(4030, 70),
	(4031, 70),
	(4032, 70),
	(4033, 70),
	(4034, 70),
	(4035, 70),
	(4036, 70),
	(4037, 70),
	(4038, 70),
	(4039, 70),
	(4040, 70),
	(4041, 70),
	(4042, 70),
	(4043, 70),
	(4046, 37),
	(4047, 35),
	(4048, 71),
	(4049, 36),
	(4050, 68),
	(4051, 70),
	(4052, 70),
	(4053, 70),
	(4054, 70),
	(4055, 70),
	(4056, 70),
	(4057, 70),
	(4058, 70),
	(4059, 70),
	(4060, 68),
	(4061, 70),
	(4062, 70),
	(4063, 70),
	(4064, 70),
	(4065, 71),
	(4066, 71),
	(4067, 56),
	(4068, 70),
	(4069, 70),
	(4070, 71),
	(4071, 70),
	(4072, -1),
	(4073, 70),
	(4074, 70),
	(4075, 70),
	(4076, -1),
	(4077, 70),
	(4078, 70),
	(4079, 70),
	(4080, 70),
	(4081, 70),
	(4082, 70),
	(4083, 69),
	(4084, 70),
	(4085, 70),
	(4086, 69),
	(4087, 69),
	(4088, 70),
	(4089, 69),
	(4090, 66),
	(4091, 70),
	(4092, 70),
	(4093, 70),
	(4094, 70),
	(4095, 70),
	(4096, -1),
	(4097, 71),
	(4098, 70),
	(4099, 71),
	(4100, 80),
	(4101, 70),
	(4102, 70),
	(4103, 70),
	(4104, 71),
	(4105, 70),
	(4106, 70),
	(4107, 70),
	(4108, 70),
	(4109, 70),
	(4110, 70),
	(4111, 70),
	(4112, 70),
	(4113, 70),
	(4114, 71),
	(4115, 70),
	(4116, 70),
	(4117, 70),
	(4118, 71),
	(4119, 71),
	(4120, 71),
	(4121, 71),
	(4122, 71),
	(4123, 71),
	(4124, 71),
	(4125, 71),
	(4126, 71),
	(4127, 71),
	(4128, 72),
	(4129, 70),
	(4130, 70),
	(4131, 70),
	(4132, 71),
	(4133, 70),
	(4134, 70),
	(4135, 71),
	(4136, 71),
	(4137, 71),
	(4138, 70),
	(4139, 67),
	(4140, 71),
	(4141, 70),
	(4142, 70),
	(4143, 70),
	(4144, 70),
	(4145, 70),
	(4146, 70),
	(4147, 70),
	(4148, 71),
	(4149, 70),
	(4150, 71),
	(4151, 72),
	(4152, 71),
	(4153, 71),
	(4154, 71),
	(4155, 73),
	(4156, 71),
	(4157, 71),
	(4158, 71),
	(4159, 74),
	(4160, 71),
	(4161, 73),
	(4162, 72),
	(4163, 72),
	(4164, 72),
	(4165, 72),
	(4166, 72),
	(4167, 72),
	(4168, 72),
	(4169, 72),
	(4170, 72),
	(4171, 72),
	(4172, 72),
	(4173, 73),
	(4174, 73),
	(4175, 73),
	(4176, 73),
	(4177, 73),
	(4178, 73),
	(4179, 73),
	(4180, 73),
	(4181, 73),
	(4182, 73),
	(4183, 72),
	(4184, 73),
	(4185, 73),
	(4186, 73),
	(4187, 73),
	(4188, 74),
	(4189, 74),
	(4190, 74),
	(4191, 74),
	(4192, 73),
	(4193, 73),
	(4194, 74),
	(4195, 74),
	(4196, 74),
	(4197, -1),
	(4198, 72),
	(4199, 74),
	(4200, 74),
	(4201, 75),
	(4202, 74),
	(4203, 74),
	(4204, 73),
	(4205, 75),
	(4206, 74),
	(4207, 74),
	(4208, 73),
	(4209, 74),
	(4210, 73),
	(4211, 74),
	(4212, 74),
	(4213, 74),
	(4214, 74),
	(4215, 74),
	(4216, 74),
	(4217, 74),
	(4218, 74),
	(4219, 74),
	(4220, 74),
	(4221, 74),
	(4222, 74),
	(4223, 72),
	(4224, 71),
	(4225, 71),
	(4226, 71),
	(4227, 73),
	(4228, 80),
	(4229, 75),
	(4230, 74),
	(4231, 74),
	(4232, 72),
	(4233, 72),
	(4234, 80),
	(4235, 74),
	(4236, 74),
	(4237, 74),
	(4238, 74),
	(4239, 74),
	(4240, 74),
	(4241, 72),
	(4242, 74),
	(4243, 72),
	(4244, 74),
	(4245, 75),
	(4246, 73),
	(4247, 74),
	(4248, 73),
	(4249, 75),
	(4250, 75),
	(4251, 74),
	(4252, 72),
	(4253, 73),
	(4254, 74),
	(4255, 74),
	(4256, 74),
	(4257, 74),
	(4258, 70),
	(4259, 74),
	(4260, 70),
	(4261, 70),
	(4262, 74),
	(4263, 70),
	(4264, 77),
	(4265, 71),
	(4266, 75),
	(4267, 74),
	(4268, 74),
	(4269, 70),
	(4270, 75),
	(4271, 71),
	(4272, 80),
	(4273, 80),
	(4275, 75),
	(4276, 75),
	(4277, 76),
	(4278, 75),
	(4279, 75),
	(4280, 75),
	(4281, 55),
	(4282, 78),
	(4283, 77),
	(4284, 75),
	(4285, 77),
	(4286, 77),
	(4287, 78),
	(4288, 78),
	(4289, 76),
	(4290, 76),
	(4291, 76),
	(4292, 76),
	(4293, 76),
	(4294, 75),
	(4295, 77),
	(4296, 77),
	(4297, 77),
	(4298, 45),
	(4299, 75),
	(4300, 78),
	(4302, 77),
	(4303, 77),
	(4304, 77),
	(4306, 77),
	(4307, 77),
	(4308, 77),
	(4309, 74),
	(4310, 74),
	(4311, 74),
	(4312, 74),
	(4313, 74),
	(4314, 75),
	(4315, 75),
	(4316, 75),
	(4317, 75),
	(4318, 75),
	(4319, 75),
	(4320, 75),
	(4321, 76),
	(4322, 76),
	(4323, 76),
	(4324, 76),
	(4325, 76),
	(4326, 76),
	(4327, 76),
	(4328, 76),
	(4329, 77),
	(4342, 55),
	(4343, 55),
	(4344, 55),
	(4345, 55),
	(4346, 55),
	(4347, 55),
	(4348, 55),
	(4349, 55),
	(4350, 55),
	(4351, 55),
	(4352, 55),
	(4353, 55),
	(4354, 55),
	(4355, 55),
	(4356, 55),
	(4357, 58),
	(4358, 55),
	(4359, 55),
	(4360, 55),
	(4361, 58),
	(4362, 55),
	(4363, 55),
	(4364, 55),
	(4365, 55),
	(4366, 74),
	(4367, 77),
	(4368, 77),
	(4369, 77),
	(4371, 77),
	(4372, 77),
	(4373, 77),
	(4374, 75),
	(4375, 77),
	(4376, 77),
	(4377, 55),
	(4378, 1),
	(4379, 70),
	(4380, 70),
	(4381, 52),
	(4382, 78),
	(4383, 77),
	(4384, -1),
	(4385, 77),
	(4386, 77),
	(4387, 77),
	(4388, 77),
	(4389, 77),
	(4390, 75),
	(4391, 77),
	(4392, 80),
	(4393, 77),
	(4394, 55),
	(4395, 75),
	(4396, 73),
	(4399, 70),
	(4400, 77),
	(4401, 71),
	(4402, 70),
	(4403, 70),
	(4404, 70),
	(4405, 73),
	(4406, -1),
	(4407, 54),
	(4408, 54),
	(4411, 10),
	(4412, 77),
	(4413, 77),
	(4414, 74),
	(4415, 76),
	(4416, 76),
	(4417, 80),
	(4418, 77),
	(4419, 77),
	(4420, 77),
	(4421, 77),
	(4422, 77),
	(4423, 78),
	(4424, 78),
	(4425, 78),
	(4426, 77),
	(4427, 77),
	(4428, 78),
	(4429, 78),
	(4430, 80),
	(4431, 78),
	(4432, 78),
	(4433, 78),
	(4434, 79),
	(4435, 79),
	(4436, 79),
	(4437, 79),
	(4438, 80),
	(4439, 80),
	(4440, 80),
	(4441, 80),
	(4442, 80),
	(4443, 80),
	(4444, 80),
	(4445, 80),
	(4446, 80),
	(4447, 80),
	(4448, 80),
	(4449, 80),
	(4450, 80),
	(4451, 77),
	(4452, 77),
	(4453, 77),
	(4454, 78),
	(4455, 79),
	(4456, 74),
	(4457, 77),
	(4458, 77),
	(4459, 77),
	(4460, 77),
	(4461, 77),
	(4462, 77),
	(4463, 80),
	(4464, 80),
	(4465, 80),
	(4466, 80),
	(4467, 80),
	(4468, 75),
	(4469, 75),
	(4470, 80),
	(4471, -1),
	(4472, 77),
	(4473, 78),
	(4474, 70),
	(4475, 74),
	(4476, 77),
	(4477, 80),
	(4478, 71),
	(4479, 71),
	(4480, 73),
	(4481, 74),
	(4482, 73),
	(4483, 76),
	(4484, 78),
	(4485, 78),
	(4486, 80),
	(4487, 80),
	(4488, 80),
	(4489, 75),
	(4490, 78),
	(4491, 74),
	(4492, 80),
	(4493, 80),
	(4494, 73),
	(4495, 80),
	(4496, 80),
	(4497, 80),
	(4498, 80),
	(4499, 78),
	(4500, 80),
	(4501, 77),
	(4502, 80),
	(4503, 77),
	(4504, 77),
	(4505, 77),
	(4506, 77),
	(4507, 77),
	(4508, 78),
	(4509, 78),
	(4510, 78),
	(4511, 80),
	(4512, 80),
	(4513, 80),
	(4514, 80),
	(4515, 80),
	(4516, 80),
	(4517, 80),
	(4518, 80),
	(4519, 80),
	(4520, 80),
	(4521, 80),
	(4522, 80),
	(4523, 79),
	(4524, 80),
	(4525, 80),
	(4526, 80),
	(4527, 80),
	(4528, 80),
	(4529, 80),
	(4530, 80),
	(4531, 80),
	(4532, 70),
	(4533, 78),
	(4534, 71),
	(4535, 78),
	(4536, 80),
	(4537, 78),
	(4538, 80),
	(4539, 80),
	(4540, 79),
	(4541, 77),
	(4542, 80),
	(4543, 80),
	(4544, 55),
	(4545, 55),
	(4546, 55),
	(4547, 80),
	(4548, 80),
	(4549, 80),
	(4550, 80),
	(4551, 80),
	(4552, 80),
	(4553, 80),
	(4554, 80),
	(4555, 80),
	(4556, 80),
	(4557, 80),
	(4558, 80),
	(4559, 80),
	(4560, 75),
	(4564, 75),
	(4567, 75),
	(4568, 75),
	(4569, 75),
	(4570, 75),
	(4571, 2147483647),
	(4572, 2147483647),
	(4575, 80),
	(4576, 80),
	(4577, 80),
	(4578, 80),
	(4579, 77),
	(4580, 80),
	(4581, 80),
	(4582, 80),
	(4583, 80),
	(4584, 80),
	(4585, 80),
	(4586, 80),
	(4587, 80),
	(4588, 80),
	(4589, 80),
	(4590, 80),
	(4591, 80),
	(4592, 80),
	(4593, 79),
	(4594, 80),
	(4595, 80),
	(4596, 80),
	(4597, 80),
	(4598, 75),
	(4599, 75),
	(4601, 75),
	(4602, -1),
	(4603, 81),
	(4604, -1),
	(4605, -1),
	(4606, -1),
	(4607, -1),
	(4608, -1),
	(4609, -1),
	(4610, -1),
	(4611, -1),
	(4612, -1),
	(4613, 75),
	(4614, 75),
	(4615, 80),
	(4616, 75),
	(4617, -1),
	(4618, 75),
	(4619, 75),
	(4620, 75),
	(4621, 80),
	(4622, 80),
	(4623, 72),
	(4624, 73),
	(4625, 74),
	(4626, 76),
	(4627, 74),
	(4628, 60),
	(4629, 77),
	(4630, -1),
	(4631, 79),
	(4632, 75),
	(4633, -1),
	(4635, 73),
	(4636, 45),
	(4637, 75),
	(4638, 75),
	(4639, 70),
	(4640, 70),
	(4641, 70),
	(4642, 70),
	(4646, 74),
	(4650, -1),
	(4652, -1),
	(4653, -1),
	(4654, -1),
	(4655, -1),
	(4656, -1),
	(4657, -1),
	(4658, 74),
	(4665, -1),
	(4666, 80),
	(4667, 80),
	(4668, 14),
	(4669, 80),
	(4670, 80),
	(4671, 80),
	(4672, 80),
	(4673, 80),
	(4674, 80),
	(4676, 80),
	(4677, 80),
	(4679, 10),
	(4688, -1),
	(4692, 80),
	(4710, -1),
	(4722, 81),
	(4723, -1),
	(4739, 75),
	(4740, 75),
	(4741, -1),
	(4742, 80),
	(4743, 80),
	(4747, -1),
	(4748, -1),
	(4749, 68),
	(4750, -1),
	(4751, -1),
	(4752, -1),
	(4753, -1),
	(4760, 80),
	(4763, -1),
	(4764, -1),
	(4769, 80),
	(4809, -1),
	(4812, -1),
	(4813, -1),
	(4820, -1),
	(4832, -1),
	(4833, -1),
	(4859, -1),
	(4862, 80),
	(4889, -1),
	(4890, -1),
	(4891, -1),
	(4892, -1),
	(4893, -1),
	(4894, -1),
	(4895, -1),
	(4896, -1),
	(4897, -1),
	(4898, -1),
	(4904, -1),
	(4905, -1),
	(4906, -1),
	(4908, -1),
	(4910, -1),
	(4987, 81);
/*!40000 ALTER TABLE `ai_playerbot_zone_level` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
