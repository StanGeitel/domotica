-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Gegenereerd op: 18 dec 2017 om 21:14
-- Serverversie: 10.1.26-MariaDB
-- PHP-versie: 7.1.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `domotica`
--

-- --------------------------------------------------------

--
-- Tabelstructuur voor tabel `dimlights`
--

CREATE TABLE `dimlights` (
  `id` int(11) NOT NULL,
  `room` varchar(30) NOT NULL,
  `intensity` int(3) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Gegevens worden geëxporteerd voor tabel `dimlights`
--

INSERT INTO `dimlights` (`id`, `room`, `intensity`) VALUES
(1, 'woonkamer', 100),
(2, 'Jenik', 100);

-- --------------------------------------------------------

--
-- Tabelstructuur voor tabel `lights`
--

CREATE TABLE `lights` (
  `id` int(3) NOT NULL,
  `room` varchar(30) NOT NULL,
  `onOff` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Gegevens worden geëxporteerd voor tabel `lights`
--

INSERT INTO `lights` (`id`, `room`, `onOff`) VALUES
(1, 'badkamer', 0),
(2, 'slaapkamer', 1),
(3, 'toilet', 0),
(4, 'schuur', 0),
(5, 'zolder', 0),
(6, 'zolder', 0),
(7, 'badkamer', 0),
(8, '5', 0),
(9, '4', 0);

-- --------------------------------------------------------

--
-- Tabelstructuur voor tabel `users`
--

CREATE TABLE `users` (
  `id` int(3) NOT NULL,
  `username` varchar(30) NOT NULL,
  `password` varchar(30) NOT NULL,
  `isAdmin` int(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Gegevens worden geëxporteerd voor tabel `users`
--

INSERT INTO `users` (`id`, `username`, `password`, `isAdmin`) VALUES
(1, 'admin', 'admin', 1),
(2, 'user', 'user', 0),
(1, 'admin', 'admin', 1),
(2, 'user', 'user', 0);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
